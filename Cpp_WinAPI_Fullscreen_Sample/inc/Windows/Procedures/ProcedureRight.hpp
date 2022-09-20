#pragma once

namespace windows::windowRight {
	
	winapi::rect previousWindowPosition;
	
	proceeded stdcall WindowRightProcedure(
		winapi::windowHandle window,
		input message,
		winapi::messageW wArgument,
		winapi::messageL lArgument
	) {
		const auto SC_DRAG = SC_SIZE + 9;
		switch (message) { 
				
			case input::OnMouseLeftClickDown: {
				GetWindowRect(window, &previousWindowPosition);
				/// To make the window dragable. 
				PostMessage(window, WM_SYSCOMMAND, SC_DRAG, (LPARAM)NULL);
				return proceeded::False;
			}
			
			case input::OnMove: {
				/// To lock the Drag in x-axis.
				winapi::rect& newPosition = *((winapi::rect*)lArgument);
				newPosition.bottom = previousWindowPosition.bottom;
				newPosition.top = previousWindowPosition.top;
				
				if constexpr (DEBUG) {	
					
					// !
					// array<winapi::wchar, 100> debugMessage;
					const string message ( 
						"pos: l: " 	+ ToString(newPosition.left) 	+ 
						", u: " 	+ ToString(newPosition.top) 	+ 
						", r: " 	+ ToString(newPosition.right) 	+ 
						", d: " 	+ ToString(newPosition.bottom)
					);
					
					winapi::debug::console::LogInfo(message);
				}
				
				return proceeded::True;
			}
			
			default:
				return (proceeded)DefWindowProcW(window, (uint32)message, wArgument, lArgument);
		}
	}
	
}