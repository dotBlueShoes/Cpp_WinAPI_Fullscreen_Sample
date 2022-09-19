#pragma once

#include "Resources/Settings/targetver.h"
#include "Resources/resource.h"				// Resource itself

#include "MST.hpp"
#include "MST/WinAPI.hpp"

struct {
	const winapi::resourceType
		classNameId = IDC_CPPWINAPIFULLSCREENSAMPLE,
		iconId = IDI_CPPWINAPIFULLSCREENSAMPLE,
		menuId = IDC_CPPWINAPIFULLSCREENSAMPLE,
		windowAboutId = IDD_ABOUTBOX,
		iconSmallId = IDI_SMALL,
		titleId = IDS_APP_TITLE;

	array<winapi::wchar, 100> className, title;
	winapi::keysMapHandle keys { 0 };
} resource;

namespace resourceHandler {
	block Initialize(winapi::handleInstance instance) {
		winapi::wideString::Load(instance, resource.classNameId, resource.className.Pointer(), (int32)resource.className.Length()); // Possesing string data from resource file.
		winapi::wideString::Load(instance, resource.titleId, resource.title.Pointer(), (int32)resource.title.Length());
		resource.keys = winapi::accelerator::Load(instance, MAKEINTRESOURCEW(resource.classNameId)); // Getting the keyMap
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