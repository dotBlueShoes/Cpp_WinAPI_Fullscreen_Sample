#pragma once
#include "Framework.hpp"
using namespace winapi::window;

namespace windows {

	uint64 messageCounter { 0 };

	block RefreshWindowButton(winapi::windowHandle window, resourceHandler::buttonInput button) {
		winapi::windowHandle buttonWindow { GetDlgItem(window, (uint32)button) };
		darkmode::AllowDarkModeForWindow(buttonWindow);
		SendMessageW(buttonWindow, (uint32)input::ThemeChange, 0, 0);
	}

	proceeded stdcall About(
		winapi::windowHandle window,
		input message,
		winapi::messageW wArgument,
		winapi::messageL lArgument
	) {

		switch (message) {
			case input::InitializeDialogWindow:
				if (darkmode::isSupported) {
					SetWindowTheme(GetDlgItem(window, (uint32)resourceHandler::buttonInput::Ok), L"Explorer", nullptr);	// Set up the darkmode/lightmode availability.
					RefreshWindowButton(window, resourceHandler::buttonInput::Ok); 										// Check whether lightmode/darkmode.
					darkmode::RefreshTitleBarTheme(window);
				} return proceeded::True;

			case input::ControlStaticBeforeDraw:
			case input::DialogWindowBeforeDraw: {
				winapi::displayContextHandle displayContext { (winapi::displayContextHandle)wArgument };
				SetTextColor(displayContext, (*brushes::colors).text);
				SetBkColor(displayContext, (*brushes::colors).primar);
				return (proceeded)((INT_PTR)(brushes::primar.Get()));
			}

			case input::SettingChange:
				if (darkmode::isSupported)
					if (wArgument == 0)
						return darkmode::CheckWhetherImmersiveColorSet(window, (winapi::wchar*)lArgument, messageCounter);
				return proceeded::False;

			case input::ThemeChange:
				if (darkmode::isSupported) {

					RefreshWindowButton(window, resourceHandler::buttonInput::Ok);
					darkmode::RefreshTitleBarTheme(window);

					if (darkmode::isEnabled) brushes::ChangePalette(theme::dark);
					else brushes::ChangePalette(theme::light);
					brushes::Destroy(); // This makes brushes white as the data holded there is no longer.
					brushes::Initialize();

					UpdateWindow(window); // to applay new background color.
					return proceeded::True;

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

				} return proceeded::False;
				
			case input::Command:
				if (
					LOWORD(wArgument) == (uint32)resourceHandler::buttonInput::Ok || 
					LOWORD(wArgument) == (uint32)resourceHandler::buttonInput::Cancel
				) {
					EndDialog(window, LOWORD(wArgument));
					return proceeded::True;
				}
				
			default:
				break;
		}

		return proceeded::False;
	}

};