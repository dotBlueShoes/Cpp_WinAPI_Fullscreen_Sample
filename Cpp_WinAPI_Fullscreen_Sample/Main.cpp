// The entry of the application and it's main procedure.

#include "framework.hpp"
#include "windows/windowAbout.hpp"
#include "settings/darkmode.hpp"
#include "settings/uahmenubar.hpp"

// Enabling darkmode [runtime] ...
// For backwards compatibility first check whether we're running windows 10 and higher or not.
//  this however requires applications to be manifested for windows 10 to work properly.
//#include <versionhelpers.h>
//IsWindows10OrGreater()

using namespace winapi;

const size maxStringLength = 100;
handleInstnace mainProcessInstance;			// Wystąpienie tej aplikacji.

// what would i do now:
//  1. i know how to say to a user that a specifc feature is not available due to os he is using.
//   now i need to not ship the code that wont work under his version to do this i need macros
//   https://docs.microsoft.com/pl-pl/windows/win32/winprog/using-the-windows-headers?redirectedfrom=MSDN
//   so the darkmode api needs to be wraped in those and an alternative for other versions should exist.
//   Maybe i should create my own macros that i could turn on off and see what parts of code highlights then.
//   What about windows server versions?
//  2. Is IsWindows10OrGreater() needed? maybe if i pull out a macro that would say "NO_WINDOWS_VERSION_SPECIFIED"
//   that would mean that we're shipping the code with all versions code but we do those IsWindowsXOrGreater checks
//   why would someone like that... - i guess no intallation issues, no extra doing there.
//  3. flag64 variable, think about it, what it would cost, what it would optimise.
//   Would it be possible to create a enum class on top of that to have const masks for checks needed?
//   Iterestingly i belief that it's possible to override what function from dll returns and instead of BOOL return an uint32 readable value
//   tho it requires replacing in first place and after the calling of function is it worthit and acctually doable?
//  4. DialogBox also needs to be darkmoded..
//  5. UAHMenuBar and maybe other things are leaking !
//   https://codereview.stackexchange.com/questions/20181/correct-way-of-using-hbrushes

// flag64 darkmodeFlag

// enum class darkmodeFlag : flag64 {
//	isSupported : 0b0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000
// 
// };

#define WM_UAHDESTROYWINDOW    0x0090	// handled by DefWindowProc
#define WM_UAHDRAWMENU         0x0091	// lParam is UAHMENU
#define WM_UAHDRAWMENUITEM     0x0092	// lParam is UAHDRAWMENUITEM
#define WM_UAHINITMENU         0x0093	// handled by DefWindowProc
#define WM_UAHMEASUREMENUITEM  0x0094	// lParam is UAHMEASUREMENUITEM
#define WM_UAHNCPAINTMENUPOPUP 0x0095	// handled by DefWindowProc

namespace event {
	inline auto CreateMainWindow(const windowHandle& window) {

		#ifdef WINDOWS_VERSION_10

		// Load the needed .dll setup the function and deload the .dll.
		darkmode::Initialize();

		if (darkmode::isSupported) {
			darkmode::proxy::AllowDarkModeForWindow(window, darkmode::isEnabled);

			// Refresh titlebar theme color.
			BOOL dark = FALSE;

			if (darkmode::proxy::IsDarkModeAllowedForWindow(window) &&
				darkmode::proxy::ShouldAppsUseDarkMode() &&
				!darkmode::IsHighContrast()) {
				dark = TRUE;
			}

			//if (g_buildNumber < 18362)
			//SetPropW(window, L"UseImmersiveDarkModeColors", reinterpret_cast<HANDLE>(static_cast<INT_PTR>(dark)));

			if (darkmode::proxy::SetWindowCompositionAttribute) {
				darkmode::proxy::WINDOWCOMPOSITIONATTRIBDATA data = { darkmode::proxy::WCA_USEDARKMODECOLORS, &dark, sizeof dark };
				darkmode::proxy::SetWindowCompositionAttribute(window, &data);
			}

		}

		// See what we got.
		//array<wchar, 3> buffor;
		//Uint64ToWcharsTerminated<3>(buffor, darkmode::isEnabled, '\n');
		//MessageBoxEx(window, L"woah", buffor.Pointer(), MB_OK, 0);

		#endif
		
	}

	inline auto MessagePaint(const windowHandle& window) {
		windowDrawContext drawContext;
		displayContextHandle displayContext { BeginPaint(window, &drawContext) };
		EndPaint(window, &drawContext);
	}

	inline auto MessageAbout(const windowHandle& window) {
		DialogBox(mainProcessInstance, MAKEINTRESOURCE(resource.windowAboutId), window, (DLGPROC)windows::About);
	}

	inline auto Default(windowHandle window, uint32 message, messageW wArgument, messageL lArgument) {
		return DefaultWindowProcedure(window, message, wArgument, lArgument);
	}
}

int64 stdcall WindowMainProcedure(
	windowHandle window,
	uint32 message,
	messageW wArgument,
	messageL lArgument
) {

	// # HERE IS ALL WE NEED.
	LRESULT lr = 0;
	if (UAHWndProc(window, message, wArgument, lArgument, &lr)) {
		return lr;
	}

	switch (message) {

		case windowInput::Create:
			event::CreateMainWindow(window);
			break;

		case windowInput::Command:
			switch (GetMenuInput(wArgument)) {
				case mainMenuInput::About:
					event::MessageAbout(window);
					break;
				case mainMenuInput::Quit:
					DestroyWindow(window);
					break;
				default:
					return event::Default(window, message, wArgument, lArgument);
			}
			break;

		case windowInput::Paint:
			event::MessagePaint(window);
			break;

		case WM_UAHDRAWMENU:
			MessageBoxEx(window, L"woah", L"HERE1", MB_OK, 0);
			return event::Default(window, message, wArgument, lArgument);

		case WM_SETTINGCHANGE:
			MessageBoxEx(window, L"woah", L"HERE0", MB_OK, 0);
			break;

		case windowInput::Destroy:
			PostQuitMessage(0);
			break;

		default:
			return event::Default(window, message, wArgument, lArgument);

	}

	return 0;
}

int32 stdcall wWinMain(
	sal_in handleInstnace processInstance,	// The process we're given to run our program.
	sal_io handleInstnace hPrevInstance,	// Now has no meaing it's 0 always.
	sal_in wchar* cmdlineArgs,				// Contains command line arguments as a unicode string.
	sal_in int32 windowState				// flag that says whether the window should appear minimized, maximied, shown normally.
){
	resourceFile::Load(processInstance);	// Getting the resourceFiles loaded.

	// Parent window creation.
	window::Register(processInstance, resource.className.Pointer(), WindowMainProcedure, resource.iconId, resource.iconSmallId, resource.menuId);
	if (window::Initialize(processInstance, resource.className.Pointer(), resource.title.Pointer(), windowState))
		return 0;
	mainProcessInstance = processInstance;

	{	// Program's main loop.
		retrivedMessage message;
		while (message::Get(message)) {
			if (!accelerator::Translate(message.hwnd, resource.keys, message)) {
				message::Translate(message);
				message::Dispatch(message);
			}
		} 
		
		return (int32)message.wParam;
	}
}