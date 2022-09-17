#include "Framework.hpp"
#include "Windows/WindowMain.hpp"	

// Anti queue overflow mechanism. Whenever we know how many msgs we get and how we want to respond to them.
// uint64 messageCounter ( 0 ); 

int32 stdcall wWinMain(
	[in]	winapi::handleInstance process,	/// The process we're given to run our program.
	[out]	winapi::handleInstance ignored,	/// Now has no meaing it's 0 always.
	[in]	winapi::wchar* cmdlineArgs,		/// Contains command line arguments as a unicode string.
	[in]	int32 windowState				/// Flag that says whether the window should appear minimized, maximied, shown normally.
){
	
	using namespace winapi;
	const vector2<uint64> windowSize ( 1400, 800 );
	
	mainProcess = process; 			/// Setting up the Global
	
	application::Initialize();		/// Initializing Modules, DarkMode.
	
	{ 	namespace dbg = mst::winapi::debug::console; 
		dbg::LogInfo	("This debug message will display at the very begginging");
		dbg::LogWarning	("This debug message will display at Second");
		dbg::LogError	( "This debug message will display at Third");
	}

	if constexpr (SYSTEM_VERSION == SystemVersion::Windows10) 
		brushes::ChangePalette(window::theme::dark);
	
	brushes::Initialize();
	windows::mainWindow::Create(process, brushes::primar.Get(), windowState, windowSize);

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