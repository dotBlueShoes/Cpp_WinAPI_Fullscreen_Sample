#pragma once
#include "Framework.hpp"

using namespace winapi::window;

namespace windows {
	
	block CreatePreviewWindow(const winapi::handleInstance& process, const winapi::windowHandle& window) {
		
		//const array<winapi::wchar, 10> sample { L"Type here" };
		winapi::rect parentWindowSize;
		//uint64 xOffset ( 2 );
		
		GetWindowRect(window, &parentWindowSize);
		
		// windowHandle richText = CreateWindowExW(
		// 	0, 
		// 	MSFTEDIT_CLASS, 
		// 	sample.Pointer(),
		// 	ES_MULTILINE | WS_VISIBLE | WS_CHILD | WS_TABSTOP, 
		// 	xOffset,
		// 	0,
		// 	parentWindowSize.right - parentWindowSize.left - 16 - (xOffset * 2),
		// 	parentWindowSize.bottom - parentWindowSize.top - 59,
		// 	window, 
		// 	nullptr, 
		// 	process, 
		// 	nullptr
		// );
			
		CreateWindowExW(
			0, 
			L"WindowClass2Name", 
			L"Window2Name", 
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, 
			0, 
			0, 
			400, 
			300, 
			window, 
			nullptr, 
			process, 
			nullptr
		);
	}
	
	// proceeded stdcall PreviewWindowProcedure(
	// 	windowHandle window,
	// 	input message,
	// 	messageW wArgument,
	// 	messageL lArgument,
	// 	UINT_PTR uid, DWORD_PTR RefData
	// ) {
	// 
	// 	return (proceeded)DefSubclassProc(window, (uint32)message, wArgument, lArgument);
	// }
	
}