#pragma once
#include "settings/targetver.h"		// WinAPI version control - GUI look.
#include "resource.h"				// Resource itself

#include "mst.hpp"
#include "mst/winapi.hpp"
using namespace mst;
using namespace winapi;

struct {

	// To be more specific Every variable inside could be as well 
	//  called mainWindow[variable].
	const resourceType 
		classNameId = IDC_CPPWINAPIFULLSCREENSAMPLE,
		iconId = IDI_CPPWINAPIFULLSCREENSAMPLE,
		menuId = IDC_CPPWINAPIFULLSCREENSAMPLE,
		windowAboutId = IDD_ABOUTBOX,
		iconSmallId = IDI_SMALL,
		titleId = IDS_APP_TITLE;

	array<wchar, 100> className, title;
	keysMapHandle keys { 0 };

} resource;

namespace resourceFile {
	block Load(handleInstnace instance) {
		// Possesing string data from aresource file.
		wideString::Load(instance, resource.classNameId, resource.className.Pointer(), (int32)resource.className.Length());
		wideString::Load(instance, resource.titleId, resource.title.Pointer(), (int32)resource.title.Length());
		// Getting the keyMap
		resource.keys = accelerator::Load(instance, MAKEINTRESOURCE(resource.classNameId));
	}
}

enum class buttonInput : uint16 {
	Cancel = IDCANCEL,
	Ok = IDOK
};

enum mainMenuInput : uint16 {
	About = IDM_ABOUT,
	Quit = IDM_EXIT
};