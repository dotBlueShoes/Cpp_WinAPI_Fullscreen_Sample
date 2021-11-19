#pragma once
#include "../framework.hpp"

winapi::proceeded CALLBACK About(
	winapi::windowHandle hDlg, uint32 message, 
	winapi::messageW w, winapi::messageL l
) {

	switch (message) {
		case windowInput::initializeDialogWindow: return winapi::proceeded::pTrue;

		case windowInput::command:
			if (LOWORD(w) == IDOK || LOWORD(w) == IDCANCEL) {
				EndDialog(hDlg, LOWORD(w));
				return winapi::proceeded::pTrue;
			}
	}

	return winapi::proceeded::pFalse;
}