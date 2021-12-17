#pragma once

#include "mst.hpp"							// MST - My Library that i use instead of standard libs and other known libraries.
#define WINDOWS_VERSION _WIN32_WINNT_WIN10
#include "mst/winapi.hpp"
using namespace mst;

#include "resource/resourcehandler.hpp"		// Resource File.
#include "settings/targetver.h"				// WinAPI version control - GUI look.

#define WIN32_LEAN_AND_MEAN					// Wyklucz rzadko używane rzeczy z nagłówków systemu Windows
#include <windows.h>						// Pliki nagłówkowe systemu Windows
//#include <uxtheme.h>

// Pliki nagłówkowe środowiska uruchomieniowego języka C

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>