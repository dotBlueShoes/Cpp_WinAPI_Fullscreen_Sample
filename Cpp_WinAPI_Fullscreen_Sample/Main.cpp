// The entry of the application and it's main procedure.

#include "framework.hpp"
#include "window/window.hpp"
#include "window/windowAbout.hpp"

using namespace winapi;

const size maxStringLength = 100;

winapi::handleInstnace mainProcessInstance;		// Wystąpienie tej aplikacji.
array<winapi::wchar, 100> 
	windowClassName,							// Nazwa klasy głównego okna.
	windowTitle;								// Tekst paska tytułu.

int64 CALLBACK	WindowMainProcedure(windowHandle, uint32, messageW, messageL);

int32 APIENTRY wWinMain(
	_In_ handleInstnace processInstance,
	_In_opt_ handleInstnace hPrevInstance,
	_In_ wchar* lpCmdLine,
	_In_ int32 nCmdShow
){
	// Possesing string data from resource file.
	LoadStringW(processInstance, IDC_CPPWINAPIFULLSCREENSAMPLE, windowClassName.Pointer(), (int32)windowClassName.Length());
	LoadStringW(processInstance, IDS_APP_TITLE, windowTitle.Pointer(), (int32)windowTitle.Length());

	window::Register(processInstance, windowClassName.Pointer(), WindowMainProcedure);
	if (window::Initialize( processInstance, windowClassName.Pointer(), windowTitle.Pointer(), nCmdShow)) 
		return 0;

	mainProcessInstance = processInstance;

	{	// Program's LOOP
		keysMap keys = accelerator::Load(processInstance, MAKEINTRESOURCE(IDC_CPPWINAPIFULLSCREENSAMPLE));
		retrivedMessage message;

		while (message::Get(message)) {
			if (!accelerator::Translate(message.hwnd, keys, message)) {
				message::Translate(message);
				message::Dispatch(message);
			}
		}

		return (int32)message.wParam;
	}
}

int64 CALLBACK WindowMainProcedure(
	windowHandle window,
	uint32 message, 
	messageW w,
	messageL l
){
	switch (message) {

		case windowInput::command: {

			switch (GetMenuInput(w)) { 

				case menuInput::about:
					DialogBox(mainProcessInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), window, (DLGPROC)About);
					break;

				case menuInput::quit:
					DestroyWindow(window);
					break;

				default:
					return DefaultWindowProcedure(window, message, w, l);

			} break;

		}
		
		case windowInput::paint: {
			windowDrawContext drawContext;
			displayContextHandle displayContext { BeginPaint(window, &drawContext) };
			EndPaint(window, &drawContext);
			break;
		}
		
		case windowInput::destroy:
			PostQuitMessage(0);
			break;

		default:
			return DefaultWindowProcedure(window, message, w, l);
	}

	return 0;
}