#pragma once

namespace windows::windowLeft {
	
	proceeded stdcall WindowLeftProcedure(
		winapi::windowHandle window,
		input message,
		winapi::messageW wArgument,
		winapi::messageL lArgument
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
