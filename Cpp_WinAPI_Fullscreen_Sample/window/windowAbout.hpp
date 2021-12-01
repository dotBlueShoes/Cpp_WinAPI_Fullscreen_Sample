#pragma once
#include "../framework.hpp"

using namespace winapi;

// This is cool however
//  - got an error that conversion from mst::winapi::messageW to mst::uint32 is a loss type conversion.
//  - i really want to template those 2.

// Retrives Higher half of the type. 
getter uint16 Hig(uint32 whole) { return (uint16)(whole >> 16); }
// Retrives Lower half of the type. 
getter uint16 Low(uint32 whole) { return (uint16)(whole); }

namespace window {

	proceeded CALLBACK About(
		windowHandle window, uint32 message,
		messageW wArgument, messageL lArgument
	) {

		switch (message) {
			case windowInput::InitializeDialogWindow: return proceeded::True;

			case windowInput::Command:

				if (LOWORD(wArgument) == buttonInput::Ok || LOWORD(wArgument) == buttonInput::Cancel) {
					EndDialog(window, LOWORD(wArgument));
					return proceeded::True;
				}
		}

		return proceeded::False;
	}

};