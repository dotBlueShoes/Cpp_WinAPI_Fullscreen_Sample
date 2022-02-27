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


// Controls and fun ...
// https://docs.microsoft.com/en-us/windows/win32/controls/property-sheets
// https://docs.microsoft.com/en-us/windows/win32/controls/tooltip-control-reference
// https://cpp0x.pl/kursy/Kurs-WinAPI-C++/167
// https://docs.microsoft.com/en-us/cpp/overview/visual-cpp-samples?view=msvc-170

// 2. Brushes Prob - uneeded memory operations.
// 3. Fullscreen implementation.

#include "framework.hpp"
#include "windows/windowEditor.hpp"
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

		//MessageBoxEx(window, L"Main", L"PaintCall", MB_YESNO, 0);
		//windows::CreateEditor(mainProcess, window);
		registry::AddRegistryKey(window);

		return proceeded::True;
	}

	inline proceeded Destroy() {
		themes::Destroy();
		
		PostQuitMessage(0); // This is a call to the thread queue itself that we're finished.
		return proceeded::True;
	}

	inline proceeded Paint(const windowHandle& window) {
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
	
	inline proceeded SettingChange(const windowHandle& window, const messageW& wArgument, const messageL& lArgument) {
		if (darkmode::isSupported)
			if (wArgument == 0)
				return darkmode::CheckMainWindowWhetherImmersiveColorSet(window, (wchar*)lArgument, messageCounter);
		return proceeded::False;
	}

	inline proceeded ThemeChange(windowHandle window) {

		darkmode::RefreshTitleBarTheme(window);

		if (window::event::uahmenubar::menuTheme) {
			CloseThemeData(window::event::uahmenubar::menuTheme);
			window::event::uahmenubar::menuTheme = nullptr;
		}

		if (darkmode::isEnabled) themes::ChangeColorPalette(theme::darkMode);
		else themes::ChangeColorPalette(theme::lightMode);

		themes::Destroy(); // This makes brushes white as the data holded there is no longer.
		themes::InitializeBrushes();
		InvalidateRect(window, NULL, TRUE);
		DrawMenuBar(window);

		// return proceeded::False;
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
		namespace menu = event::uahmenubar;

		case input::Command:
			switch (GetMenuInput(wArgument)) {
				case mainMenuInput::About:					return windowMain::event::MessageAbout(window);
				case mainMenuInput::MaxMin: {
					GetWindowPlacement(window, &windowMode::windowedPlacement);
					(windowMode::windowedPlacement.showCmd == SW_MAXIMIZE) ? 
						ShowWindow(window, SW_SHOWDEFAULT) : ShowWindow(window, SW_MAXIMIZE);
					return proceeded::True; 
				} // F11
				case mainMenuInput::Quit:					DestroyWindow(window);					return proceeded::True;
				default:									return (proceeded)windowMain::event::Default(window, (uint32)message, wArgument, lArgument);
			} break;

		case (input)menu::UAHMenuEvent::DrawItem: 
			return menu::DrawMenuItem(window, *((menu::UAHDRAWMENUITEM*)lArgument), themes::backgroundSecondary, themes::backgroundSelected, themes::backgroundHovered, (*(themes::colorPalette)).textPrimary);
		case (input)menu::UAHMenuEvent::Draw: 
			return menu::DrawMenu(window, *((menu::UAHMENU*)lArgument), themes::backgroundSecondary);
		case input::SettingChange: 
			return windowMain::event::SettingChange(window, wArgument, lArgument);
		case input::EraseBackgroundOnCalledInvalidPortion: 
			return proceeded::False;
		case input::ThemeChange: 
			return windowMain::event::ThemeChange(window);
		case input::Create: 
			return windowMain::event::Create(window);
		case input::Paint: 
			return windowMain::event::Paint(window);
		case input::Destroy: 
			return windowMain::event::Destroy();

		case input::NonClientAreaPaint:
		case input::NonClientAreaFocus: {
			displayContextHandle drawContext { GetWindowDC(window) };
			windowMain::event::Default(window, (uint32)message, wArgument, lArgument); // We need to get throuh some other paints first.
			menu::DrawBottomLine(window, drawContext, themes::border.Get());
			return proceeded::True;
		}

		default: return windowMain::event::Default(window, (uint32)message, wArgument, lArgument);
		/* Keeping it just to get u know thats the option.
		//case (proceeded)uahmenubar::UAHMenuEvent::MeasureItem:
		//	event::Default(window, message, wArgument, lArgument); // Allow the default window procedure to handle the message. So we have something and we do actions on top of that.
		//	{ using namespace uahmenubar;
		//		uint32 height = (uint32)((*(UAHMEASUREMENUITEM*)lArgument).mis.itemHeight * 4.0 / 3.0),
		//			width = (uint32)((*(UAHMEASUREMENUITEM*)lArgument).mis.itemWidth * 4.0 / 3.0);
		//		MeasureMenuItem((*(UAHMEASUREMENUITEM*)lArgument), height, width);
		//	} return proceeded::True;
		*/
	}
	return proceeded::False;
}

int32 stdcall wWinMain(
	sal_in handleInstnace process,	// The process we're given to run our program.
	sal_io handleInstnace ignored,	// Now has no meaing it's 0 always.
	sal_in wchar* cmdlineArgs,		// Contains command line arguments as a unicode string.
	sal_in int32 windowState		// flag that says whether the window should appear minimized, maximied, shown normally.
){
	application::Initialize();		// Initializing Modules, DarkMode.
	resourceFile::Load(process);	// Getting the resourceFiles loaded.

	#ifdef WINDOWS_VERSION_10 
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

		application::Destroy();
		return (int32)message.wParam;
	}
}



// So i can return to these.
//MSG msg { 0 };
//PeekMessage(&msg, window, 0, 0, PM_NOREMOVE);
//while (msg.message == WM_SETTINGCHANGE) {
//	PeekMessage(&msg, window, 0, 0, PM_REMOVE);
//	PeekMessage(&msg, window, 0, 0, PM_NOREMOVE);
//}
//MessageBoxEx(window, L"Main", L"PaintCall", MB_OK, 0);