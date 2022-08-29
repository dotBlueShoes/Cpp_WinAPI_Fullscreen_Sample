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
				
			case (input)WM_LBUTTONDOWN: {
				GetWindowRect(window, &oldPosition);
				/// To make the window dragable. 
				PostMessage(window, WM_SYSCOMMAND, SC_DRAG, (LPARAM)NULL);
				return proceeded::False;
			}
			
			case (input)WM_MOVING: {
				
				/// To lock the Drag in x-axis.
				rect& newPosition = *((rect*)lArgument);
				newPosition.bottom = oldPosition.bottom;
				newPosition.top = oldPosition.top;
				
				// Debug
				// messageLength = swprintf_s(
				// 	debugMessage, 
				// 	L"[MOVING] %d, %d, %d, %d", 
				// 	newPosition.left, newPosition.top, newPosition.right, newPosition.bottom
				// );
				
				return proceeded::True;
			}
			
			default:
				return (proceeded)DefWindowProcW(window, (uint32)message, wArgument, lArgument);
		}
	}
	
}