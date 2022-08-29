#include "Framework.hpp"
#include "Windows/WindowMain.hpp"	

/// Anti queue overflow mechanism. Whenever we know how many msgs we get and how we want to respond to them.
uint64 messageCounter ( 0 ); 

int32 stdcall wWinMain(
	[in]	handleInstance process,	/// The process we're given to run our program.
	[out]	handleInstance ignored,	/// Now has no meaing it's 0 always.
	[in]	wchar* cmdlineArgs,		/// Contains command line arguments as a unicode string.
	[in]	int32 windowState		/// flag that says whether the window should appear minimized, maximied, shown normally.
){
	
	// Debug
	mst::winapi::debug::console::RedirectIOToConsole();
	
	application::Initialize();		/// Initializing Modules, DarkMode.
	resourceFile::Load(process);	/// Getting the resourceFiles loaded.

	#ifdef WINDOWS_VERSION_10 
	if (darkmode::isEnabled) themes::ChangeColorPalette(theme::darkMode);
	#endif
	
	themes::InitializeBrushes();
	windows::CreateMainWindow(process, themes::backgroundPrimary.Get(), windowState);
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