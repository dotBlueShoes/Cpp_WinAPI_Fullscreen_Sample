#pragma once

namespace windows {
	
	proceeded stdcall WindowLeftProcedure(
		windowHandle window,
		input message,
		messageW wArgument,
		messageL lArgument
	) {
		switch (message) { 
		
			case input::Create: {
				windows::CreateEditor(mainProcess, window);
				return proceeded::True;
			}
			
			default:
				return (proceeded)DefWindowProcW(window, (uint32)message, wArgument, lArgument);
		}
	}
	
}
