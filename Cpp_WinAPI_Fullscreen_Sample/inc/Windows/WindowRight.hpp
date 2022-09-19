#pragma once

namespace windows {
	
	uint64 messageLength ( 0 );
	
	winapi::wchar debugMessage[255];
	winapi::rect oldPosition;
	
	proceeded stdcall WindowRightProcedure(
		winapi::windowHandle window,
		input message,
		winapi::messageW wArgument,
		winapi::messageL lArgument
	) {
		const auto SC_DRAG = SC_SIZE+9;
		switch (message) { 
				
			case input::OnMouseLeftClickDown: {
				GetWindowRect(window, &oldPosition);
				/// To make the window dragable. 
				PostMessage(window, WM_SYSCOMMAND, SC_DRAG, (LPARAM)NULL);
				return proceeded::False;
			}
			
			case input::OnMove: {
				
				/// To lock the Drag in x-axis.
				winapi::rect& newPosition = *((winapi::rect*)lArgument);
				newPosition.bottom = oldPosition.bottom;
				newPosition.top = oldPosition.top;
				
				if constexpr (DEBUG) {	
					using namespace winapi::debug::console;
					
					const string message ( 
						"pos: l: " 	+ ToString(newPosition.left) 	+ 
						", u: " 	+ ToString(newPosition.top) 	+ 
						", r: " 	+ ToString(newPosition.right) 	+ 
						", d: " 	+ ToString(newPosition.bottom)
					);
					
					LogInfo<LogLevel::Max>(message);
				}
				
				return proceeded::True;
			}
			
			default:
				return (proceeded)DefWindowProcW(window, (uint32)message, wArgument, lArgument);
		}
	}
	
}