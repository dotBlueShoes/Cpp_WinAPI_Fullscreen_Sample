#pragma once
#include "../framework.hpp"
using namespace winapi::window;

namespace windows {

	uint64 messageCounter { 0 };

	proceeded stdcall About(
		windowHandle window,
		input message,
		messageW wArgument, 
		messageL lArgument
	) {

		switch (message) {
			case input::InitializeDialogWindow:
				if (darkmode::isSupported) {
					SetWindowTheme(GetDlgItem(window, buttonInput::Ok), L"Explorer", nullptr);
					SendMessageW(window, WM_THEMECHANGED, 0, 0);
				}
				return proceeded::True;

			case (input)WM_CTLCOLORSTATIC: 
			case (input)WM_CTLCOLORDLG: {
				displayContextHandle displayContext = (displayContextHandle)wArgument;
				SetTextColor(displayContext, (*themes::colorPalette).textPrimary);
				SetBkColor(displayContext, (*themes::colorPalette).backgroundPrimary);
				return (proceeded)((INT_PTR)(themes::backgroundPrimary.Get()));
			}

			case input::SettingChange:
				if (darkmode::isSupported && darkmode::IsColorSchemeChangedMessage(lArgument)) {
					if (++messageCounter == 10) {
						SendMessageW(window, WM_THEMECHANGED, 0, 0);
						messageCounter = 0;
					}
				} break;

			case input::ThemeChange:
				if (darkmode::isSupported) {
					darkmode::proxy::AllowDarkModeForWindow(window, darkmode::isEnabled);
					darkmode::RefreshTitleBarTheme(window);

					windowHandle button = GetDlgItem(window, IDOK);
					darkmode::proxy::AllowDarkModeForWindow(button, darkmode::isEnabled);
					SendMessageW(button, WM_THEMECHANGED, 0, 0); // ? Something is wrong

					SendMessageW(window, WM_CTLCOLORDLG, (messageW)(GetDC(window)), 0); // ? Something is wrong
					UpdateWindow(window);

					//MessageBoxEx(window, L"Changed", L"About", MB_OK, 0);
				} break;
				
			case input::Command:
				if (LOWORD(wArgument) == buttonInput::Ok || LOWORD(wArgument) == buttonInput::Cancel) {
					EndDialog(window, LOWORD(wArgument));
					return proceeded::True;
				}
		}

		return proceeded::False;
	}

};