#include "Framework.hpp"
#include "Windows/PreviewWindow/PreviewWindow.hpp"
#include "Windows/DialogWindows/WindowAbout.hpp"
#include "Windows/WindowEditor.hpp"
#include "Windows/WindowRight.hpp"
#include "Windows/WindowLeft.hpp"

namespace windows::mainWindow {
	
	windowHandle rightWindow, leftWindow;

	namespace event {

		inline proceeded Create(const windowHandle& mainWindow) {
	
			/// Refresh titlebar theme color.
			if constexpr (SYSTEM_VERSION == SystemVersion::Windows10) 
				if (darkmode::isSupported) {
					darkmode::AllowDarkModeForWindow(mainWindow);
					darkmode::RefreshTitleBarTheme(mainWindow);
				}
	
			//MessageBoxEx(window, L"Main", L"Call", MB_YESNO, 0);
			
			/// Creation of inner windows that have their own drawing system. 
			//windows::CreateEditor(mainProcess, window);
			
			//registry::AddRegistryKey(window);
			//registry::RemoveRegistryKey(window);
			
			
			//windows::CreatePreviewWindow(mainProcess, mainWindow);
			
			// Making window in window.... It's hard
			// https://stackoverflow.com/questions/42734429/win32-c-child-window-is-not-visible
			// 2 windowClass windowProperties;
			// 2 windowProperties.cbSize = ( sizeof(windowClass) );
			// 2 
			// 2 windowProperties.style			= NULL; //
			// 2 windowProperties.lpfnWndProc	= (windowProcedure)WindowOtherProcedure;
			// 2 windowProperties.cbClsExtra		= 0;
			// 2 windowProperties.cbWndExtra		= 0;
			// 2 windowProperties.hInstance		= mainProcess;
			// 2 windowProperties.hIcon			= nullptr;
			// 2 windowProperties.hCursor		= nullptr;
			// 2 windowProperties.hbrBackground	= themes::backgroundSecondary.Get();
			// 2 windowProperties.lpszMenuName	= nullptr;
			// 2 windowProperties.lpszClassName	= L"WindowClass2Name";
			// 2 windowProperties.hIconSm		= nullptr;
			// 2 
			// 2 RegisterClassExW(&windowProperties);
			// 2 
			// 2 if (!CreateWindowExW(0, L"WindowClass2Name", L"Window2Name", WS_CHILD | WS_SIZEBOX | WS_CLIPSIBLINGS | WS_OVERLAPPEDWINDOW , 0, 0, 400, 300, window, nullptr, mainProcess, NULL))
			// 2 	MessageBoxExW(window, L"Here", L"Oh yeah", MB_YESNO, 0);
			// 2 
			// 2 ShowWindow(window, SW_SHOW);
			// 2 UpdateWindow(window);
	
			return proceeded::True;
		}
	
		inline proceeded Destroy() {
			brushes::Destroy();
			PostQuitMessage(0); 		/// Call to the thread queue itself that we're finished.
			return proceeded::True;
		}
	
		inline proceeded Paint(const windowHandle& window) {
			// 1 const array<winapi::wchar, 10> sample { L"Type here" };
			// 1 
			// 1 windowDrawContext drawContext;
			// 1 rect clientArea { 0 };
			// 1 
			// 1 {
			// 1 	displayContextHandle displayContext ( BeginPaint(window, &drawContext) );
			// 1 	
			// 1 	/// To get the whole "background" area.
			// 1 	GetClientRect(window, &clientArea);
			// 1 	
			// 1 	/// Setting up the background and text color. This takes action in windows created here.
			// 1 	SetBkColor(displayContext, brushes::colors->primar);
			// 1 	SetTextColor(displayContext, brushes::colors->text);
			// 1 	
			// 1 	/// If we woudn't recreate some window/s with each draw we could use this
			// 1 	/// to simply make our background the default we wanted. SetBkColor wouldn't be needed then.
			// 1 	//FillRect(displayContext, &clientArea, brushes::backgroundPrimary.Get());
			// 1 	
			// 1 	/// Displing some text in a clientArea making the background color change.
			// 1 	ExtTextOutW (
			// 1 		displayContext,			/// on what we are drawing
			// 1 		0,						/// x coordinate
			// 1 		0,						/// y coordinate
			// 1 		ETO_OPAQUE,				/// styles
			// 1 		&clientArea,			/// rect specyfing the window coordinates
			// 1 		sample.Pointer(),		/// string
			// 1 		sample.Length(),		/// string length
			// 1 		nullptr					/// distance between letters
			// 1 	);
			// 1 	
			// 1 	EndPaint(window, &drawContext);
			// 1 }
	
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
	
			if (darkmode::isEnabled) brushes::ChangePalette(theme::dark);
			else brushes::ChangePalette(theme::light);
	
			/// This makes brushes white as the data holded there is no longer available.
			brushes::Destroy(); 
			brushes::Initialize();
			InvalidateRect(window, NULL, TRUE);
			DrawMenuBar(window);
	
			return proceeded::True;
		}
	
		inline proceeded Resize(windowHandle window, uint32 state, int clientX, int clientY) {
			/// More about - https://devblogs.microsoft.com/oldnewthing/20050706-26/?p=35023
			const uint64 windowsNumber ( 2 );
			const uint64 clientXHalf ( clientX / 2 );
			
			multipleWindowHandle windows ( BeginDeferWindowPos(windowsNumber) );
			
			//if (windows) windows = DeferWindowPos (
			//	windows, 
			//	leftWindow,
			//	nullptr, 
			//	0, 
			//	0, 
			//	clientXHalf, 
			//	clientY,
			//	SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOACTIVATE
			//);
				
			if (windows) windows = DeferWindowPos (
				windows, 
				rightWindow, 
				nullptr, 
				clientXHalf, 
				0, 
				clientX - clientXHalf, 
				clientY,
				SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOACTIVATE
			);
				
			if (windows) EndDeferWindowPos(windows);
			return proceeded::False;
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
		namespace menu = mst::winapi::window::event::uahmenubar;
		switch (message) {
	
			case input::Command: {
				switch (GetMenuInput(wArgument)) {
					case mainMenuInput::About: return event::MessageAbout(window);
					case mainMenuInput::Quit: DestroyWindow(window); return proceeded::True;
	
					case mainMenuInput::MaxMin: {
						
						GetWindowPlacement(window, &windowMode::windowedPlacement);
						if (windowMode::windowedPlacement.showCmd == SW_MAXIMIZE) ShowWindow(window, SW_SHOWDEFAULT);
						else ShowWindow(window, SW_MAXIMIZE);
							
						return proceeded::True;
					} // F11
	
					default: return (proceeded)event::Default(window, (uint32)message, wArgument, lArgument);
				} break;
			}
	
			/// Clang questions enum values specified outside the used type... 
			///  As i want menu theming to be optional and not any way dirived it stays this way.
			#pragma GCC diagnostic push
			#pragma GCC diagnostic ignored "-Wswitch"
			
			// 1 case (input)menu::Event::DrawItem: {
			// 1 	auto menuItem ( *((menu::UAHDRAWMENUITEM*)lArgument) );
			// 1 	return menu::DrawMenuItem(
			// 1 		window, menuItem, 
			// 1 		brushes::second, 
			// 1 		brushes::selected, 
			// 1 		brushes::hovered, 
			// 1 		(*(brushes::colors)).text
			// 1 	);
			// 1 }
			// 1 
			// 1 case (input)menu::Event::Draw: {
			// 1 	auto menuInstance ( *((menu::UAHMENU*)lArgument) );
			// 1 	return menu::DrawMenu(window, menuInstance, brushes::second);
			// 1 }
			
			#pragma GCC diagnostic pop
	
			case input::SettingChange:
				return event::SettingChange(window, wArgument, lArgument);
	
			case input::EraseBackgroundOnCalledInvalidPortion:
				return proceeded::False;
	
			case input::ThemeChange:
				return event::ThemeChange(window);
	
			case input::Create:
				return event::Create(window);
			
			case input::Paint:
				return event::Paint(window);
	
			case input::Destroy:
				return event::Destroy();
			
			case input::NonClientAreaPaint:
			case input::NonClientAreaFocus: {
				displayContextHandle drawContext { GetWindowDC(window) };
				event::Default(window, (uint32)message, wArgument, lArgument); // We need to get throuh some other paints first.
				menu::DrawBottomLine(window, drawContext, brushes::border.Get());
				return proceeded::True;
			}
			
			case input::Resize:
				event::Resize(window, wArgument, LOWORD(lArgument), HIWORD(lArgument));
				
			default:
				return event::Default(window, (uint32)message, wArgument, lArgument);
			
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

	const windowHandle Create(
		const winapi::handleInstance& process, 
		const winapi::brushHandle& backgroundBrush,
		const int32& windowState,
		const vector2<uint64>& windowMainSize
	) {
		
		Register(
			process, 
			resource.className.Pointer(), 
			(windowProcedure)WindowMainProcedure, 
			resource.iconId, 
			resource.iconSmallId, 
			resource.menuId, 
			backgroundBrush
		);
		
		{
			const vector2<uint64> 
				windowMainPoistion	( CW_USEDEFAULT, 0 ),
				windowRightOffset	( 700, 0 ),
				//windowLeftOffset	( 0, 0 ),
				windowRightSize		( 700, 800 );
				//windowLeftSize		( 700, 800 );
				
			const windowHandle mainWindow ( Initialize(
				process, 
				resource.className.Pointer(), 
				resource.title.Pointer(), 
				windowState,
				windowMainPoistion,
				windowMainSize
			) ); 
				
			//leftWindow = CreateChildWindow (
			//	mainProcess, 
			//	mainWindow, 
			//	(windowProcedure)WindowLeftProcedure, 
			//	(HCURSOR)LoadImage(NULL, MAKEINTRESOURCE(OCR_NORMAL), IMAGE_CURSOR, 0, 0, LR_SHARED),
			//	L"WindowClass3Name",
			//	brushes::primar.Get(),
			//	windowState,
			//	windowLeftOffset,
			//	windowLeftSize
			//);
			
			// 1 rightWindow = CreateChildWindow (
			// 1 	mainProcess, 
			// 1 	mainWindow, 
			// 1 	(windowProcedure)WindowRightProcedure, 
			// 1 	(HCURSOR)LoadImage(NULL, MAKEINTRESOURCE(OCR_NORMAL), IMAGE_CURSOR, 0, 0, LR_SHARED),
			// 1 	L"WindowClass2Name",
			// 1 	brushes::second.Get(),
			// 1 	windowState,
			// 1 	windowRightOffset,
			// 1 	windowRightSize
			// 1 );
			
			return mainWindow;
		}
		
	}

}