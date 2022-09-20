#pragma once
#include "Procedures/ProcedureLeft.hpp"

namespace windows::windowLeft {
	
	const winapi::windowHandle Create(
		const winapi::handleInstance& process,
		const winapi::windowHandle& parentWindow,
		const winapi::brushHandle& backgroundBrush,
		const int32& windowState
	) {	
		const array<winapi::wchar, 11> className ( L"WindowLeft" );
		const vector2<uint64> 
			windowLeftSize ( 700, 800 ),
			windowLeftOffset ( 0, 0 );
			
		if constexpr (DEBUG) debug::LogInfo("(CALL) Window-Left:Create");
				
		{
			const winapi::windowHandle window (
				CreateChildWindow (
					process, 
					parentWindow, 
					(winapi::windowProcedure)WindowLeftProcedure, 
					(HCURSOR)LoadImage(NULL, MAKEINTRESOURCE(OCR_NORMAL), IMAGE_CURSOR, 0, 0, LR_SHARED),
					className.Pointer(),
					backgroundBrush,
					windowState,
					windowLeftOffset,
					windowLeftSize
				)
			);
			
			if constexpr (DEBUG)
				// ERROR HANDLING
				if (window == NULL) debug::LogError("Window not created!");
				
			return window;
		}
	}
	
}
