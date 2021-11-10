// Code in this file represents the includes shared among all other code files in this project.

#pragma once

#include "settings/targetver.h"

#define WIN32_LEAN_AND_MEAN             // Wyklucz rzadko używane rzeczy z nagłówków systemu Windows
#include <windows.h>					// Pliki nagłówkowe systemu Windows

// Pliki nagłówkowe środowiska uruchomieniowego języka C

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// Resource - Files that are coresponding for menus, buttons and other.

#include "resource/resource.h"