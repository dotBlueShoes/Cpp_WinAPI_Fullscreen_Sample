// header.h: plik dołączany dla standardowych systemowych plików dołączanych,
// lub pliki dołączane specyficzne dla projektu
//

#pragma once

#include "mst.hpp"						// MST standard library

#include "settings/targetver.h"			// WinAPI version control - GUI look.

#define WIN32_LEAN_AND_MEAN             // Wyklucz rzadko używane rzeczy z nagłówków systemu Windows
#include <windows.h>					// Pliki nagłówkowe systemu Windows


// Pliki nagłówkowe środowiska uruchomieniowego języka C

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>