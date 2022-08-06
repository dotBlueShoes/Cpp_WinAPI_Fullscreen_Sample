#pragma once

#pragma comment(lib, "comctl32.lib")

#include "MST.hpp"							// MST - My Library that i use instead of standard libs and other known libraries.
#define WINDOWS_VERSION_10
#include "MST/WinAPI.hpp"
using namespace mst;

#include "resources/ResourceHandler.hpp"	// Resource File.
#include "resources/Themes.hpp"				// Window themes.

#define WIN32_LEAN_AND_MEAN					// Wyklucz rzadko używane rzeczy z nagłówków systemu Windows
#include <windows.h>						// Pliki nagłówkowe systemu Windows

// Pliki nagłówkowe środowiska uruchomieniowego języka C

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "commctrl.h"
#include "richedit.h"
#include <atlstr.h>