#pragma once
#include "resource.h"
#include "../framework.hpp"
using namespace winapi;

struct {
	const uint32 windowClassNameId = IDC_CPPWINAPIFULLSCREENSAMPLE;
	const uint32 windowTitleId = IDS_APP_TITLE;
	array<wchar, 100> windowClassName, windowTitle;
	keysMap keys { 0 };
} resource;

namespace resourceFile {
	block Load(handleInstnace instance) {
		// Possesing string data from aresource file.
		wideString::Load(instance, resource.windowClassNameId, resource.windowClassName.Pointer(), (int32)resource.windowClassName.Length());
		wideString::Load(instance, resource.windowTitleId, resource.windowTitle.Pointer(), (int32)resource.windowTitle.Length());
		// Getting the keyMap
		resource.keys = accelerator::Load(instance, MAKEINTRESOURCE(resource.windowClassNameId));
	}
}

