#pragma once
#pragma comment(lib, "comctl32.lib")

#include "MST.hpp"							// MST - My Library that i use instead of standard libs and other known libraries.
#include "MST/WinAPI.hpp"

using namespace mst;
using namespace mst::winapi::window; // !

#include "ResourceHandler.hpp"				// Resource File.
#include "Brushes.hpp"						// Window themes.

//#define WIN32_LEAN_AND_MEAN					// Wyklucz rzadko używane rzeczy z nagłówków systemu Windows
// Pliki nagłówkowe środowiska uruchomieniowego języka C

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "commctrl.h"
#include "richedit.h"
#include <atlstr.h>

/// Global variables.
winapi::handleInstance mainProcess; /// This application instance.