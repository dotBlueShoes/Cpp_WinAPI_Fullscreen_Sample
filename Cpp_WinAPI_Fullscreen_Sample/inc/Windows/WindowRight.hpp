#pragma once
#include "Procedures/ProcedureRight.hpp"

namespace windows::windowRight {
	
	const winapi::windowHandle Create(
		const winapi::handleInstance& process,
		const winapi::windowHandle& parentWindow,
		const winapi::brushHandle& backgroundBrush,
		const int32& windowState
	) {	
		const array<winapi::wchar, 12> className ( L"WindowRight" );
		const vector2<uint64> 
			windowRightOffset ( 700, 0 ),
			windowRightSize	( 700, 800 );
			
		if constexpr (DEBUG) debug::LogInfo("(CALL) Window-Right:Create");
				
		{
			const winapi::windowHandle window (
				CreateChildWindow (
					process, 
					parentWindow, 
					(winapi::windowProcedure)WindowRightProcedure, 
					(HCURSOR)LoadImage(NULL, MAKEINTRESOURCE(OCR_NORMAL), IMAGE_CURSOR, 0, 0, LR_SHARED),
					className.Pointer(),
					backgroundBrush,
					windowState,
					windowRightOffset,
					windowRightSize
				)
			);
			
			if constexpr (DEBUG)
				// ERROR HANDLING
				if (window == NULL) debug::LogError("Window not created!");
					
			return window;
		}
	}
	
}