// The entry of the application and it's main procedure.

// HERE! DLL Library
// https://docs.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-freelibrary
// It might be possible to dynamically asociate the window procedure.
// https://stackoverflow.com/questions/31648180/c-changing-hwnd-window-procedure-in-runtime

// TODO
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
//  5. See why the move of DeleteLibrary worked and where should it be really positioned then.
//  6. Create a border for menu items using https://stackoverflow.com/questions/16159127/win32-how-to-draw-a-rectangle-around-a-text-string

// flag64 darkmodeFlag
// enum class darkmodeFlag : flag64 {
//	isSupported : 0b0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000
// };
// See what we got.
//array<wchar, 3> buffor;
//Uint64ToWcharsTerminated<3>(buffor, darkmode::isEnabled, '\n');
//MessageBoxEx(window, L"woah", buffor.Pointer(), MB_OK, 0);
// handle leaks
// https://codereview.stackexchange.com/questions/20181/correct-way-of-using-hbrushes

#include "framework.hpp"
#include "windows/windowAbout.hpp"
using namespace winapi::window;

handleInstnace mainProcess; // Wystąpienie tej aplikacji.
uint64 messageCounter { 0 }; // Anti queue overflow. Whenever we know how many msgs we get and how we want to respond to them.

namespace windowMain::event {
	inline proceeded Create(const windowHandle& window) {

		// Refresh titlebar theme color.
		#ifdef WINDOWS_VERSION_10
		if (darkmode::isSupported) {
			darkmode::AllowDarkModeForWindow(window);
			darkmode::RefreshTitleBarTheme(window);
		}
		#endif

		return proceeded::True;
	}

	inline proceeded Destroy() {
		themes::Destroy();
		PostQuitMessage(0); // This is a call to the thread queue itself that we're finished.

		return proceeded::True;
	}

	// Calls whenever the window is being moved called via other, initialized and much more.
	inline proceeded Paint(const windowHandle& window) {
		//MessageBoxEx(window, L"Main", L"PaintCall", MB_OK, 0);
		windowDrawContext drawContext;
		displayContextHandle displayContext { BeginPaint(window, &drawContext) };

		rect clientArea { 0 };
		GetClientRect(window, &clientArea);
		FillRect(displayContext, &clientArea, themes::backgroundPrimary.Get());

		EndPaint(window, &drawContext);

		return proceeded::True;
	}

	inline proceeded MessageAbout(const windowHandle& window) {
		DialogBox(mainProcess, MAKEINTRESOURCE(resource.windowAboutId), window, (DLGPROC)windows::About);
		return proceeded::True;
	}


	// This calls whenever we switch from dark mode to light mode and reverse.
	inline proceeded SettingChange(const windowHandle& window, const messageW& parameterW, const messageL& parameterL) {

		// For some reason the changing from lightmode to darkmode and vice versa
		// always take 10 calls in here with only wParam chaning.
		// This wParam holds information obaut "SystemParametersInfo"
		// I tried to decode it however those are not bit flags.
		// Instead they have a hex meaning and they are added to each other.

		// I found the a solution to discard unnecesery for me messages.
		// Which would be done with "PeekMessage" and flags PM_NOREMOVE, PM_REMOVE
		// however the actual working solution is to applay a counter and hope it will lock at 10 msgs.

		/*
		#ifdef WINDOWS_VERSION_10
		// By that we know that it's either a
		// - result of a change in policy settings ( 0 == user policy )
		// - result of a change in locale settings
		// - when an application sends this message
		// All of these use parameterL as string therefore we can check that.
		if (parameterW == NULL) {
			// To check for ImmersiveColorSet
			const wchar* pointer = (wchar*)parameterL;
			const array<wchar, 18> function { L'I', L'm', L'm', L'e', L'r', L's', L'i', L'v', L'e', L'C', L'o', L'l', L'o', L'r', L'S', L'e', L't', L'\0' };
			bool equals = 0;

			for (int i = 0; pointer[i] != L'\0'; i++)
				equals = equals + (pointer[i] == function[i]);
			if (!equals) return proceeded::True;

			if (darkmode::isSupported && darkmode::IsColorSchemeChangedMessage(parameterL)) {
				MessageBoxEx(window, pointer, L"Main", MB_OK, 0);
				darkmode::isEnabled = darkmode::proxy::ShouldAppsUseDarkMode() && !darkmode::IsHighContrast();

				darkmode::RefreshTitleBarTheme(window);
				SendMessageW(window, WM_THEMECHANGED, 0, 0); // ... its called like at least 7 times..
				return proceeded::True;
			}
		} 
		#endif
		return proceeded::True;
		//MessageBoxEx(window, L"Main", L"SettingChange", MB_OK, 0);
		*/

		if (darkmode::isSupported && darkmode::IsColorSchemeChangedMessage(parameterL)) {
			if (++messageCounter == 10) {
				messageCounter = 0;

				darkmode::isEnabled = darkmode::proxy::ShouldAppsUseDarkMode() && !darkmode::IsHighContrast();

				//if (darkmode::isEnabled)
				//	MessageBoxEx(window, L"Enabled", L"Main", MB_OK, 0);
				//else
				//	MessageBoxEx(window, L"Disabled", L"Main", MB_OK, 0);

				darkmode::RefreshTitleBarTheme(window);
				SendMessageW(window, WM_THEMECHANGED, 0, 0);
			}
		}

		/*
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
		*/

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

		// "return 0" means that WM_PAINT instead handles the background color setting up.
		// It makes passing the background color at window class initialization useless.
		case (input)WM_ERASEBKGND:
			return proceeded::False;

		case input::Command:
			switch (GetMenuInput(wArgument)) {
				case mainMenuInput::About: return windowMain::event::MessageAbout(window);
				case mainMenuInput::Quit: DestroyWindow(window); return proceeded::True;
				default: return windowMain::event::Default(window, (uint32)message, wArgument, lArgument);
			} break;

		case input::Create: return windowMain::event::Create(window);
		case input::Destroy: return windowMain::event::Destroy();
		case input::Paint: return windowMain::event::Paint(window);
		case input::SettingChange: return windowMain::event::SettingChange(window, wArgument, lArgument);

		#ifdef WINDOWS_VERSION_10
		namespace eu = event::uahmenubar;
		case (input)eu::UAHMenuEvent::DrawItem: return eu::DrawMenuItem(window, *(eu::UAHDRAWMENUITEM*)lArgument, themes::backgroundSecondary, themes::backgroundSelected, themes::backgroundHovered, (*(themes::colorPalette)).textPrimary);
		case (input)eu::UAHMenuEvent::Draw: return eu::DrawMenu(window, *(eu::UAHMENU*)lArgument, themes::backgroundSecondary);
		#endif

		case input::NonClientAreaPaint:
		case input::NonClientAreaFocus: {
			displayContextHandle drawContext { GetWindowDC(window) };
			// We need to get throuh some other paints first.
			windowMain::event::Default(window, (uint32)message, wArgument, lArgument);

			// UAHMENU
			#ifdef WINDOWS_VERSION_10
			event::uahmenubar::DrawBottomLine(window, drawContext, themes::border.Get());
			#endif
			return proceeded::True;
		}

		case input::ThemeChange: {
			#ifdef WINDOWS_VERSION_10 // UAHMENU
			if (event::uahmenubar::menuTheme) {
				CloseThemeData(event::uahmenubar::menuTheme);
				event::uahmenubar::menuTheme = nullptr;
			} 

			// HEEEREEEE IT IS ALSO CALLED ABOUT 7 times do something with it!!!!
			// I need to change the color either by creating a new window on top or doing some SelectObject magic i guess
			if (darkmode::isEnabled) themes::ChangeColorPalette(theme::darkMode);
			else themes::ChangeColorPalette(theme::lightMode);

			//themes::ReplaceBrushes();
			themes::Destroy(); // This makes brushes white as the data holded there is no longer.
			themes::InitializeBrushes();
			InvalidateRect(window, NULL, TRUE);
			DrawMenuBar(window);

			//SendMessageW(window, WM_PAINT, 0, 0);
			//displayContextHandle displayContext = GetDC(window);
			//SelectObject(displayContext, themes::backgroundPrimary.Get());
			//event::uahmenubar::UAHMENU newMenuRef {GetMenu(window), GetWindowDC(window), 0};
			//event::uahmenubar::DrawMenu(window, newMenuRef, themes::backgroundSecondary);
			#endif
		} break;

		default: return windowMain::event::Default(window, (uint32)message, wArgument, lArgument);

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
	sal_in handleInstnace process,	// The process we're given to run our program.
	sal_io handleInstnace ignored,	// Now has no meaing it's 0 always.
	sal_in wchar* cmdlineArgs,		// Contains command line arguments as a unicode string.
	sal_in int32 windowState		// flag that says whether the window should appear minimized, maximied, shown normally.
){
	resourceFile::Load(process);	// Getting the resourceFiles loaded.

	#ifdef WINDOWS_VERSION_10 // Darkmode initialization.
	darkmode::Initialize();
	if (darkmode::isEnabled) themes::ChangeColorPalette(theme::darkMode);
	#endif
	
	themes::InitializeBrushes();

	// Main window creation.
	Register(process, resource.className.Pointer(), (windowProcedure)WindowMainProcedure, resource.iconId, resource.iconSmallId, resource.menuId, themes::backgroundPrimary.Get());
	if (Initialize(process, resource.className.Pointer(), resource.title.Pointer(), windowState)) 
		return 0;

	mainProcess = process;

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

//MSG msg { 0 };
//PeekMessage(&msg, window, 0, 0, PM_NOREMOVE);
//while (msg.message == WM_SETTINGCHANGE) {
//	PeekMessage(&msg, window, 0, 0, PM_REMOVE);
//	PeekMessage(&msg, window, 0, 0, PM_NOREMOVE);
//}