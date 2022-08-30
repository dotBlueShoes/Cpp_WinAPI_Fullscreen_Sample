#pragma once

namespace windows {
	
	wchar debugMessage[255];
	uint64 messageLength ( 0 );
	rect oldPosition;
	
	proceeded stdcall WindowRightProcedure(
		windowHandle window,
		input message,
		messageW wArgument,
		messageL lArgument
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
				rect& newPosition = *((rect*)lArgument);
				newPosition.bottom = oldPosition.bottom;
				newPosition.top = oldPosition.top;
				
				#ifdef DEBUG
				{	using namespace mst::winapi::debug::console;
					
					const string message ( 
						"pos: l: " 	+ ToString(newPosition.left) 	+ 
						", u: " 	+ ToString(newPosition.top) 	+ 
						", r: " 	+ ToString(newPosition.right) 	+ 
						", d: " 	+ ToString(newPosition.bottom)
					);
					
					LogInfo(LogLevel::Max, message);
				}
				#endif
				
				return proceeded::True;
			}
			
			default:
				return (proceeded)DefWindowProcW(window, (uint32)message, wArgument, lArgument);
		}
	}
	
}