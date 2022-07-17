#pragma once
#pragma comment(lib, "comctl32.lib")
#include "Framework.hpp"
#include "commctrl.h"
#include "richedit.h"

using namespace winapi::window;

namespace windows {

	block CreateEditor(const handleInstnace& process, const windowHandle& window) {

		RECT parentWindowSize;
		GetWindowRect(window, &parentWindowSize);

		windowHandle richText = CreateWindowExW(0, MSFTEDIT_CLASS, L"Type here",
			ES_MULTILINE | WS_VISIBLE | WS_CHILD | WS_TABSTOP, 0, 0,
			parentWindowSize.right - parentWindowSize.left - 16, parentWindowSize.bottom - parentWindowSize.top - 59, // dunno why these numbers prob becouse non client area. 
			window, NULL, process, NULL);

		SendMessage(richText, EM_SETBKGNDCOLOR, 0, themes::colorPalette->backgroundSelected); // background color
		CHARFORMAT2 cf { {0} };
		cf.cbSize = sizeof cf;
		cf.dwMask = CFM_BACKCOLOR | CFM_COLOR; // Text background color and text color
		cf.crBackColor = themes::colorPalette->backgroundSecondary;
		cf.crTextColor = themes::colorPalette->textPrimary;
		SendMessage(richText, EM_SETCHARFORMAT, 0, (LPARAM)&cf);

		// SetWindowSubclass(window, richText , 0, 0); // uneeded right now.
	}

	proceeded stdcall Editor(
		windowHandle window,
		uint32 message,
		messageW wArgument,
		messageL lArgument,
		UINT_PTR uid, DWORD_PTR RefData
	) {
		//switch (message) {
			//case EM_SETBKGNDCOLOR: {
			//	//MessageBoxEx(window, L"RICHEDIT", L"PaintCall", MB_OK, 0);
			//	displayContextHandle displayContext = (displayContextHandle)wArgument;
			//	SetTextColor(displayContext, (*themes::colorPalette).textPrimary);
			//	SetBkColor(displayContext, (*themes::colorPalette).backgroundPrimary);
			//	return ((INT_PTR)(themes::backgroundPrimary.Get()));
			//}
		//}
		return (proceeded)DefSubclassProc(window, (uint32)message, wArgument, lArgument);
	}

}