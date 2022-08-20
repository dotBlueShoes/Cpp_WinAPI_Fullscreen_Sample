#pragma once

namespace windows {
	
	proceeded stdcall WindowRightProcedure(
		windowHandle window,
		input message,
		messageW wArgument,
		messageL lArgument
	) {
		switch (message) { 
			default:
				return (proceeded)DefWindowProcW(window, (uint32)message, wArgument, lArgument);
		}
	}
	
}