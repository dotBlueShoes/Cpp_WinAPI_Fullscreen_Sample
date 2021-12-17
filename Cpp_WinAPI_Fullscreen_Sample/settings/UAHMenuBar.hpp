#pragma once
#include "../framework.hpp"
#include <uxtheme.h>
#include <vsstyle.h>
#pragma comment(lib, "uxtheme.lib")

// window messages related to menu bar drawing
#define WM_UAHDESTROYWINDOW    0x0090	// handled by DefWindowProc
#define WM_UAHDRAWMENU         0x0091	// lParam is UAHMENU
#define WM_UAHDRAWMENUITEM     0x0092	// lParam is UAHDRAWMENUITEM
#define WM_UAHINITMENU         0x0093	// handled by DefWindowProc
#define WM_UAHMEASUREMENUITEM  0x0094	// lParam is UAHMEASUREMENUITEM
#define WM_UAHNCPAINTMENUPOPUP 0x0095	// handled by DefWindowProc

// describes the sizes of the menu bar or menu item
typedef union tagUAHMENUITEMMETRICS {
	// cx appears to be 14 / 0xE less than rcItem's width!
	// cy 0x14 seems stable, i wonder if it is 4 less than rcItem's height which is always 24 atm
	struct {
		DWORD cx;
		DWORD cy;
	} rgsizeBar[2];
	struct {
		DWORD cx;
		DWORD cy;
	} rgsizePopup[4];
} UAHMENUITEMMETRICS;

// not really used in our case but part of the other structures
typedef struct tagUAHMENUPOPUPMETRICS {
	DWORD rgcx[4];
	DWORD fUpdateMaxWidths : 2; // from kernel symbols, padded to full dword
} UAHMENUPOPUPMETRICS;

// hmenu is the main window menu; hdc is the context to draw in
typedef struct tagUAHMENU {
	HMENU hmenu;
	HDC hdc;
	DWORD dwFlags; // no idea what these mean, in my testing it's either 0x00000a00 or sometimes 0x00000a10
} UAHMENU;

// menu items are always referred to by iPosition here
typedef struct tagUAHMENUITEM {
	int iPosition; // 0-based position of menu item in menubar
	UAHMENUITEMMETRICS umim;
	UAHMENUPOPUPMETRICS umpm;
} UAHMENUITEM;

// the DRAWITEMSTRUCT contains the states of the menu items, as well as
// the position index of the item in the menu, which is duplicated in
// the UAHMENUITEM's iPosition as well
typedef struct UAHDRAWMENUITEM {
	DRAWITEMSTRUCT dis; // itemID looks uninitialized
	UAHMENU um;
	UAHMENUITEM umi;
} UAHDRAWMENUITEM;

// the MEASUREITEMSTRUCT is intended to be filled with the size of the item
// height appears to be ignored, but width can be modified
typedef struct tagUAHMEASUREMENUITEM {
	MEASUREITEMSTRUCT mis;
	UAHMENU um;
	UAHMENUITEM umi;
} UAHMEASUREMENUITEM;

static HTHEME g_menuTheme = nullptr;

static HBRUSH g_brBarBackground = CreateSolidBrush(RGB(0x0, 0x0, 0x0));

void UAHDrawMenuNCBottomLine(HWND hWnd) {
    MENUBARINFO mbi = { sizeof(mbi) };
    if (!GetMenuBarInfo(hWnd, OBJID_MENU, 0, &mbi)) {
        return;
    }

    RECT rcClient = { 0 };
    GetClientRect(hWnd, &rcClient);
    MapWindowPoints(hWnd, nullptr, (POINT*)&rcClient, 2);

    RECT rcWindow = { 0 };
    GetWindowRect(hWnd, &rcWindow);

    OffsetRect(&rcClient, -rcWindow.left, -rcWindow.top);

    // the rcBar is offset by the window rect
    RECT rcAnnoyingLine = rcClient;
    rcAnnoyingLine.bottom = rcAnnoyingLine.top;
    rcAnnoyingLine.top--;


    HDC hdc = GetWindowDC(hWnd);
    FillRect(hdc, &rcAnnoyingLine, g_brBarBackground);
    ReleaseDC(hWnd, hdc);
}

// processes messages related to UAH / custom menubar drawing.
// return true if handled, false to continue with normal processing in your wndproc
bool UAHWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* lr) {
    switch (message) {
        case WM_UAHDRAWMENU:
        {
            UAHMENU* pUDM = (UAHMENU*)lParam;
            RECT rc = { 0 };

            // get the menubar rect
            {
                MENUBARINFO mbi = { sizeof(mbi) };
                GetMenuBarInfo(hWnd, OBJID_MENU, 0, &mbi);

                RECT rcWindow;
                GetWindowRect(hWnd, &rcWindow);

                // the rcBar is offset by the window rect
                rc = mbi.rcBar;
                OffsetRect(&rc, -rcWindow.left, -rcWindow.top);
            }

            FillRect(pUDM->hdc, &rc, g_brBarBackground);

            return true;
        }
        case WM_UAHDRAWMENUITEM:
        {
            UAHDRAWMENUITEM* pUDMI = (UAHDRAWMENUITEM*)lParam;

            // ugly colors for illustration purposes
            static HBRUSH g_brItemBackground = CreateSolidBrush(RGB(0x00, 0x00, 0x00));
            static HBRUSH g_brItemBackgroundHot = CreateSolidBrush(RGB(0x4d, 0x4d, 0x4d));
            static HBRUSH g_brItemBackgroundSelected = CreateSolidBrush(RGB(0x67, 0x67, 0x67));

            HBRUSH* pbrBackground = &g_brItemBackground;

            // get the menu item string
            wchar_t menuString[256] = { 0 };
            MENUITEMINFO mii = { sizeof(mii), MIIM_STRING };
            {
                mii.dwTypeData = menuString;
                mii.cch = (sizeof(menuString) / 2) - 1;

                GetMenuItemInfo(pUDMI->um.hmenu, pUDMI->umi.iPosition, TRUE, &mii);
            }

            // get the item state for drawing

            DWORD dwFlags = DT_CENTER | DT_SINGLELINE | DT_VCENTER;

            int iTextStateID = 0;
            int iBackgroundStateID = 0;
            {
                if ((pUDMI->dis.itemState & ODS_INACTIVE) | (pUDMI->dis.itemState & ODS_DEFAULT)) {
                    // normal display
                    iTextStateID = MPI_NORMAL;
                    iBackgroundStateID = MPI_NORMAL;
                }
                if (pUDMI->dis.itemState & ODS_HOTLIGHT) {
                    // hot tracking
                    iTextStateID = MPI_HOT;
                    iBackgroundStateID = MPI_HOT;

                    pbrBackground = &g_brItemBackgroundHot;
                }
                if (pUDMI->dis.itemState & ODS_SELECTED) {
                    // clicked -- MENU_POPUPITEM has no state for this, though MENU_BARITEM does
                    iTextStateID = MPI_HOT;
                    iBackgroundStateID = MPI_HOT;

                    pbrBackground = &g_brItemBackgroundSelected;
                }
                if ((pUDMI->dis.itemState & ODS_GRAYED) || (pUDMI->dis.itemState & ODS_DISABLED)) {
                    // disabled / grey text
                    iTextStateID = MPI_DISABLED;
                    iBackgroundStateID = MPI_DISABLED;
                }
                if (pUDMI->dis.itemState & ODS_NOACCEL) {
                    dwFlags |= DT_HIDEPREFIX;
                }
            }

            if (!g_menuTheme) {
                g_menuTheme = OpenThemeData(hWnd, L"Menu");
            }

            DTTOPTS opts = { sizeof opts, DTT_TEXTCOLOR, iTextStateID != MPI_DISABLED ? RGB(0xff, 0xff, 0xff) : RGB(0x40, 0x40, 0x40) }; // RGB(0x00, 0x00, 0x20) : RGB(0x40, 0x40, 0x40)

            FillRect(pUDMI->um.hdc, &pUDMI->dis.rcItem, *pbrBackground);
            DrawThemeTextEx(g_menuTheme, pUDMI->um.hdc, MENU_BARITEM, MBI_NORMAL, menuString, mii.cch, dwFlags, &pUDMI->dis.rcItem, &opts);

            return true;
        }
        case WM_UAHMEASUREMENUITEM:
        {
            UAHMEASUREMENUITEM* pMmi = (UAHMEASUREMENUITEM*)lParam;

            // allow the default window procedure to handle the message
            // since we don't really care about changing the width
            *lr = DefWindowProc(hWnd, message, wParam, lParam);

            // but we can modify it here to make it 1/3rd wider for example
            pMmi->mis.itemWidth = (pMmi->mis.itemWidth * 4) / 3;

            return true;
        }
        case WM_THEMECHANGED:
        {
            if (g_menuTheme) {
                CloseThemeData(g_menuTheme);
                g_menuTheme = nullptr;
            }
            // continue processing in main wndproc
            return false;
        }
        case WM_NCPAINT:
        case WM_NCACTIVATE:
            *lr = DefWindowProc(hWnd, message, wParam, lParam);
            UAHDrawMenuNCBottomLine(hWnd);
            return true;
            break;
        default:
            return false;
    }
}