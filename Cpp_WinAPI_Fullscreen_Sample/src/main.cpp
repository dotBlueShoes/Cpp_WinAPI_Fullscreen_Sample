#include "Framework.hpp"

#include "Windows/WindowMain.hpp"	

/// ERRORs
/// - ctrl clicking breaks.... the whole app

/// compile and run process.. 
/// 1. compile resource
/// 2. build
/// 3. add manifest (so the buttons appear right..)
/// 4. run

int32 stdcall wWinMain(
	[in]	winapi::handleInstance process,	/// The process we're given to run our program.
	[out]	winapi::handleInstance ignored,	/// Now has no meaing it's 0 always.
	[in]	winapi::wchar* cmdlineArgs,		/// Contains command line arguments as a unicode string.
	[in]	int32 windowState				/// Flag that says whether the window should appear minimized, maximied, shown normally.
){
	using namespace winapi;
	const vector2<uint64> windowSize ( 1400, 800 );
	
	mainProcess = process; 					/// Setting up the Global
	
	application::Initialize();				/// Initializing Modules, DarkMode.
	resourceHandler::Initialize(process);	/// Loading Strings into arrays.

	if constexpr (SYSTEM_VERSION == SystemVersion::Windows10) 
		brushes::ChangePalette(window::theme::dark);
	
	brushes::Initialize();
	windows::mainWindow::Create(process, brushes::primar.Get(), windowState, windowSize);

	{	// Program's main loop.
		retrivedMessage message;
		while (message::Get(message)) {
			if (!accelerator::Translate(message.hwnd, resourceHandler::keys, message)) {
				message::Translate(message);
				message::Dispatch(message);
			}
		} 
	
		application::Destroy();
		return (int32)message.wParam;
	}
	
	return -1;
}