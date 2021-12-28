#pragma once
#include "../framework.hpp"
using namespace winapi;

namespace windows {

	window::proceeded stdcall About(
		windowHandle window, 
		uint32 message,
		messageW wArgument, 
		messageL lArgument
	) {

		switch (message) {
			case (uint32)window::input::InitializeDialogWindow: return window::proceeded::True;
			case (uint32)window::input::Command:
				if (LOWORD(wArgument) == buttonInput::Ok || LOWORD(wArgument) == buttonInput::Cancel) {
					EndDialog(window, LOWORD(wArgument));
					return window::proceeded::True;
				}
		}

		return window::proceeded::False;
	}

};