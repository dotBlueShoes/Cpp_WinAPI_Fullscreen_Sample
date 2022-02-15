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
			//case (input)WM_CTLCOLOREDIT:
			case (input)WM_CTLCOLORDLG: {
				displayContextHandle displayContext = (displayContextHandle)wArgument;
				SetTextColor(displayContext, (*themes::colorPalette).textPrimary);
				SetBkColor(displayContext, (*themes::colorPalette).backgroundPrimary);
				return (proceeded)((INT_PTR)(themes::backgroundPrimary.Get()));
			}

			case input::SettingChange:
				if (darkmode::isSupported && darkmode::IsColorSchemeChangedMessage(lArgument)) {
					if (++messageCounter == 10) {
						SendMessageW(window, (uint32)input::ThemeChange, 0, 0);
						messageCounter = 0;
					}
				} break;

			case input::ThemeChange:
				if (darkmode::isSupported) {
					darkmode::isEnabled = darkmode::proxy::ShouldAppsUseDarkMode() && !darkmode::IsHighContrast();
					darkmode::RefreshTitleBarTheme(window);

					// This should happend only once but theres nothing wrong with it running again with each call on the very same window.
					windowHandle button = GetDlgItem(window, buttonInput::Ok);
					darkmode::AllowDarkModeForWindow(button);
					SendMessageW(button, (uint32)input::ThemeChange, 0, 0);

					if (darkmode::isEnabled) themes::ChangeColorPalette(theme::darkMode);
					else themes::ChangeColorPalette(theme::lightMode);
					themes::Destroy(); // This makes brushes white as the data holded there is no longer.
					themes::InitializeBrushes();
					
					UpdateWindow(window);

					// And it all makes sense now.
					// The parent does not recive it's themechanged message first the child gets it first...
					// even tho the visuals were saing the other.... which is fucked up.
					// however it was tested with messageboxes only

					// With this information now it's time to optimize it.
					// Make it so the needed code runs once.
					// - darkmode::isEnabled should only be assigned by "darkmode::proxy::ShouldAppsUseDarkMode() && !darkmode::IsHighContrast();" value.
					// - This should happend once for the single firstmost setting change. not once for every setting change each window recives.
					// - Same applies for Palette changes 
					// - Finally it should be possible to place repetetive things that change among windows turning blackmode and vice versa inside function/s.

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