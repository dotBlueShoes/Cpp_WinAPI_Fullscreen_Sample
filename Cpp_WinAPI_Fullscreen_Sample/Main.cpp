// The entry of the application and it's main procedure.

#include "framework.hpp"
#include "windows/windowAbout.hpp"

// Enabling darkmode [runtime] ...
// For backwards compatibility first check whether we're running windows 10 and higher or not.
//  this however requires applications to be manifested for windows 10 to work properly.
//#include <versionhelpers.h>
//IsWindows10OrGreater()

using namespace winapi;
using namespace window;

theme::solidBrush backgroundPrimary, backgroundSecondary, backgroundHovered, backgroundSelected;
const theme::theme* colorPalette { &theme::lightMode };

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


// 2. See why the move of DeleteLibrary worked and where should it be really positioned then.
// 3. Create a border for menu items using https://stackoverflow.com/questions/16159127/win32-how-to-draw-a-rectangle-around-a-text-string

// flag64 darkmodeFlag
// enum class darkmodeFlag : flag64 {
//	isSupported : 0b0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000
// };

// See what we got.
//array<wchar, 3> buffor;
//Uint64ToWcharsTerminated<3>(buffor, darkmode::isEnabled, '\n');
//MessageBoxEx(window, L"woah", buffor.Pointer(), MB_OK, 0);

namespace event {
	inline proceeded Create(const windowHandle& window) {

		#ifdef WINDOWS_VERSION_10
		if (darkmode::isSupported) {
			darkmode::proxy::AllowDarkModeForWindow(window, darkmode::isEnabled);

			// Refresh titlebar theme color.
			BOOL dark { FALSE };

			if (darkmode::proxy::IsDarkModeAllowedForWindow(window) &&
				darkmode::proxy::ShouldAppsUseDarkMode() &&
				!darkmode::IsHighContrast()) {
				dark = TRUE;
			}

			if (darkmode::proxy::SetWindowCompositionAttribute) {
				darkmode::proxy::WINDOWCOMPOSITIONATTRIBDATA data { darkmode::proxy::WINDOWCOMPOSITIONATTRIB::WCA_USEDARKMODECOLORS, &dark, sizeof dark };
				darkmode::proxy::SetWindowCompositionAttribute(window, &data);
			}

		}
		#endif

		return proceeded::True;
	}

	inline proceeded Destroy() {

		backgroundSecondary.Destroy();
		backgroundSelected.Destroy();
		backgroundHovered.Destroy();
		backgroundPrimary.Destroy();

		// This is a call to the thread queue itself that we're finished.
		PostQuitMessage(0);

		return proceeded::True;
	}

	inline proceeded Paint(const windowHandle& window) {
		windowDrawContext drawContext;
		displayContextHandle displayContext { BeginPaint(window, &drawContext) };
		EndPaint(window, &drawContext);

		return proceeded::True;
	}

	inline proceeded MessageAbout(const windowHandle& window) {
		DialogBox(mainProcessInstance, MAKEINTRESOURCE(resource.windowAboutId), window, (DLGPROC)windows::About);
		return proceeded::True;
	}

	
	// This calls whenever we switch from dark mode to light mode and reverse.
	inline proceeded SettingChange(const windowHandle& window, const messageW& parameterW, const messageL& parameterL) {

		// A message that is sent to all top-level windows when the SystemParametersInfo function 
		//  changes a system-wide setting or when policy settings have changed.
		//  https://docs.microsoft.com/en-us/windows/win32/winmsg/wm-settingchange

		#ifdef WINDOWS_VERSION_10
		if (parameterW == NULL) {
			// By that we know that it's either a
			// - result of a change in policy settings ( 0 == user policy )
			// - result of a change in locale settings
			// - when an application sends this message
			// All of these use parameterL as string therefore we can check that.
			{ // To check for ImmersiveColorSet
				const wchar* pointer = (wchar*)parameterL;
				const array<wchar, 18> function { L'I', L'm', L'm', L'e', L'r', L's', L'i', L'v', L'e', L'C', L'o', L'l', L'o', L'r', L'S', L'e', L't', L'\0' };
				bool equals = 0;

				for (int i = 0; pointer[i] != L'\0'; i++)
					equals = equals + (pointer[i] == function[i]);
				if (!equals) return proceeded::True;

				if (darkmode::proxy::IsDarkModeAllowedForWindow(window) &&
					darkmode::proxy::ShouldAppsUseDarkMode() &&
					!darkmode::IsHighContrast()) {

					MessageBoxEx(window, L"DARKMODE", L"SettingChange", MB_OK, 0);
					// A switchero. Have in mind that if there would be other themes in future it should be handled here.
					//darkmode::isEnabled = !darkmode::isEnabled;
					//if (darkmode::isEnabled) colorPalette = &lightMode;
					//else colorPalette = &lightMode;
					// A redraw logic

				} else
					MessageBoxEx(window, L"LIGHTMODE", L"SettingChange", MB_OK, 0);

				darkmode::proxy::RefreshImmersiveColorPolicyState();
			}
		} // else MessageBoxEx(window, L"SettingChange", L"Not NULL!", MB_OK, 0);
		#endif

		return proceeded::True;
	}

	inline proceeded Default(windowHandle window, uint32 message, messageW wArgument, messageL lArgument) {
		return (proceeded)DefaultWindowProcedure(window, message, wArgument, lArgument);
	}
}

proceeded stdcall WindowMainProcedure(
	windowHandle window,
	input message,
	messageW wArgument,
	messageL lArgument
) {
	switch (message) {

		case input::Command:
			switch (GetMenuInput(wArgument)) {

				case mainMenuInput::About:	
					return event::MessageAbout(window);

				case mainMenuInput::Quit:
					DestroyWindow(window);
					return proceeded::True;

				default:
					return event::Default(window, (uint32)message, wArgument, lArgument);

			} break;

		case input::Create: 
			return event::Create(window);

		case input::Destroy:
			return event::Destroy();

		case input::Paint: 
			return event::Paint(window);

		case input::SettingChange: 
			return event::SettingChange(window, wArgument, lArgument);

		#ifdef WINDOWS_VERSION_10
		case (input)uahmenubar::UAHMenuEvent::DrawItem: 
			uahmenubar::DrawMenuItem(window, *(uahmenubar::UAHDRAWMENUITEM*)lArgument, backgroundSecondary, backgroundSelected, backgroundHovered, (*colorPalette).textPrimary); 
			return proceeded::True;;

		case (input)uahmenubar::UAHMenuEvent::Draw:
			uahmenubar::DrawMenu(window, *(uahmenubar::UAHMENU*)lArgument, backgroundSecondary);
			return proceeded::True;
		#endif

		case input::NonClientAreaPaint:
		case input::NonClientAreaFocus: {
			displayContextHandle drawContext { GetWindowDC(window) };

			// We need to get throuh some other paints first.
			event::Default(window, (uint32)message, wArgument, lArgument);

			// UAHMENU
			#ifdef WINDOWS_VERSION_10
			uahmenubar::DrawBottomLine(window, drawContext, backgroundPrimary.Get());
			#endif
			return proceeded::True;
		}

		case input::ThemeChange:
			// UAHMENU
			#ifdef WINDOWS_VERSION_10
			if (uahmenubar::menuTheme) { 
				CloseThemeData(uahmenubar::menuTheme);
				uahmenubar::menuTheme = nullptr;
			} 
			#endif
			break;

		default:
			return event::Default(window, (uint32)message, wArgument, lArgument);

		// Keeping it just to get u know thats the option.
		//case (proceeded)uahmenubar::UAHMenuEvent::MeasureItem:
		//	event::Default(window, message, wArgument, lArgument); // Allow the default window procedure to handle the message. So we have something and we do actions on top of that.
		//	{ using namespace uahmenubar;
		//		uint32 height = (uint32)((*(UAHMEASUREMENUITEM*)lArgument).mis.itemHeight * 4.0 / 3.0),
		//			width = (uint32)((*(UAHMEASUREMENUITEM*)lArgument).mis.itemWidth * 4.0 / 3.0);
		//		MeasureMenuItem((*(UAHMEASUREMENUITEM*)lArgument), height, width);
		//	} return proceeded::True;
	}

	return proceeded::False;
}

int32 stdcall wWinMain(
	sal_in handleInstnace processInstance,	// The process we're given to run our program.
	sal_io handleInstnace hPrevInstance,	// Now has no meaing it's 0 always.
	sal_in wchar* cmdlineArgs,				// Contains command line arguments as a unicode string.
	sal_in int32 windowState				// flag that says whether the window should appear minimized, maximied, shown normally.
){
	resourceFile::Load(processInstance);	// Getting the resourceFiles loaded.

	// Darkmode initialization.
	#ifdef WINDOWS_VERSION_10
	darkmode::Initialize();
	if (darkmode::isEnabled) colorPalette = &window::theme::darkMode;
	#endif
	
	backgroundSecondary.Create((*colorPalette).backgroundSecondary);
	backgroundSelected.Create((*colorPalette).backgroundSelected);
	backgroundHovered.Create((*colorPalette).backgroundHovered);
	backgroundPrimary.Create((*colorPalette).backgroundPrimary);

	// Main window creation.
	Register(processInstance, resource.className.Pointer(), (windowProcedure)WindowMainProcedure, resource.iconId, resource.iconSmallId, resource.menuId, backgroundPrimary.Get());
	if (Initialize(processInstance, resource.className.Pointer(), resource.title.Pointer(), windowState))
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