﻿#pragma once

#include "resource/resource.h"			// Resource File.
#include "mst.hpp"						// MST standard library.
#include "mst/winapi.hpp"
using namespace mst;

#include "settings/targetver.h"			// WinAPI version control - GUI look.

#define WIN32_LEAN_AND_MEAN             // Wyklucz rzadko używane rzeczy z nagłówków systemu Windows
#include <windows.h>					// Pliki nagłówkowe systemu Windows


// Pliki nagłówkowe środowiska uruchomieniowego języka C

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// Resource based data.

enum windowInput : int32 {
	initializeDialogWindow = WM_INITDIALOG,
	command = WM_COMMAND,
	destroy = WM_DESTROY,
	paint = WM_PAINT
};

enum menuInput : int32 {
	about = IDM_ABOUT,
	quit = IDM_EXIT
};