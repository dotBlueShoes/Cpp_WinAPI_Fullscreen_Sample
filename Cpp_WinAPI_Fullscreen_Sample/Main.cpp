// The entry of the application and it's main procedure.

#include "framework.hpp"
#include "window/window.hpp"
#include "window/windowAbout.hpp"

using namespace winapi;

const size maxStringLength = 100;
handleInstnace mainProcessInstance;			// Wystąpienie tej aplikacji.

namespace event {
	inline auto MessagePaint(const windowHandle& window) {
		windowDrawContext drawContext;
		displayContextHandle displayContext { BeginPaint(window, &drawContext) };

		//SetWindowTheme(window, L"Explorer", NULL);

		EndPaint(window, &drawContext);
	}

	inline auto MessageAbout(const windowHandle& window) {
		DialogBox(mainProcessInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), window, (DLGPROC)window::About);
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
	switch (message) {

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

	window::Register(processInstance, resource.windowClassName.Pointer(), WindowMainProcedure);	// Parent window creation.
	if (window::Initialize(processInstance, resource.windowClassName.Pointer(), resource.windowTitle.Pointer(), windowState)) 
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