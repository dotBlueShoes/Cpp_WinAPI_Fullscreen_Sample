#pragma once

#include "Resources/Settings/targetver.h"
#include "Resources/resource.h"				// Resource itself

#include "MST.hpp"
#include "MST/WinAPI.hpp"

namespace resourceHandler {
	
	const winapi::resourceType
		classNameId = IDC_CPPWINAPIFULLSCREENSAMPLE,
		iconId = IDI_CPPWINAPIFULLSCREENSAMPLE,
		menuId = IDC_CPPWINAPIFULLSCREENSAMPLE,
		windowAboutId = IDD_ABOUTBOX,
		iconSmallId = IDI_SMALL,
		titleId = IDS_APP_TITLE;

	array<winapi::wchar, 100> className, title;
	winapi::keysMapHandle keys { 0 };
	
	block Initialize(winapi::handleInstance instance) {
		winapi::wideString::Load(instance, classNameId, className.Pointer(), (int32)className.Length()); // Possesing string data from resource file.
		winapi::wideString::Load(instance, titleId, title.Pointer(), (int32)title.Length());
		keys = winapi::accelerator::Load(instance, MAKEINTRESOURCEW(classNameId)); // Getting the keyMap
	}
}

enum class buttonInput : uint16 {
	Cancel = IDCANCEL,
	Ok = IDOK
};

enum mainMenuInput : uint16 {
	About = IDM_ABOUT,
	Quit = IDM_EXIT,
	MaxMin = ID_WINDOW_MAXMIN
	//fullscreen = ID_FULLSCREEN,
	//fullscreenWindow = ID_FULLSCREEN_WINDOWED,
	//window = ID_WINDOWED,
	//WindowedBorderless = ID_WINDOWED_BORDERLESS
};