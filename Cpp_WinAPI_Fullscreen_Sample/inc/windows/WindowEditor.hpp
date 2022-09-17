#pragma once
#include "Framework.hpp"

// It appears that there is a better way then using RICHEDIT
//  https://stackoverflow.com/questions/22109514/richedit-syntax-highligh
//  however for now lets stick to it. And if i will be in need i might use
//  things like "ExtTextOut" the other day.

// ERROR
// ctrl clicking breaks.... the whole app

// https://docs.microsoft.com/en-us/windows/win32/controls/rich-edit-controls
// https://www.codeproject.com/articles/5594/a-chat-control-based-on-the-rich-edit-control
// https://www.codeproject.com/articles/1780/painless-streaming-of-rich-text-to-from-cricheditc
// https://docs.microsoft.com/en-us/windows/win32/api/richedit/nc-richedit-editstreamcallback
// https://docs.microsoft.com/en-us/windows/win32/api/richedit/ns-richedit-editstream
// https://stackoverflow.com/questions/8024157/is-it-possible-to-highlight-text-in-a-win32-edit-control
// https://stackoverflow.com/questions/14526097/best-algorithm-for-syntax-highlight-winapi
// https://docs.microsoft.com/en-us/windows/win32/controls/em-setcharformat

namespace windows {
	
	using namespace winapi::window;
	
	// proceeded stdcall EditorProcedure(
	// 	windowHandle window,
	// 	uint32 message,
	// 	messageW wArgument,
	// 	messageL lArgument,
	// 	UINT_PTR uid, DWORD_PTR RefData
	// ) {
	// 	// X switch (message) {
	// 	// X	 case EM_SETBKGNDCOLOR: {
	// 	// X	 	//MessageBoxEx(window, L"RICHEDIT", L"PaintCall", MB_OK, 0);
	// 	// X	 	displayContextHandle displayContext = (displayContextHandle)wArgument;
	// 	// X	 	SetTextColor(displayContext, (*themes::colorPalette).textPrimary);
	// 	// X	 	SetBkColor(displayContext, (*themes::colorPalette).backgroundPrimary);
	// 	// X	 	return ((INT_PTR)(themes::backgroundPrimary.Get()));
	// 	// X	 }
	// 	// X }
	// 	return (proceeded)DefSubclassProc(window, (uint32)message, wArgument, lArgument);
	// }

	block CreateEditor(
		const handleInstance& process, 
		const windowHandle& window, 
		const uint64& xOffset = 0, 
		const uint64& yOffset = 0
	) {
		const array<winapi::wchar, 10> windowDefaultText { L"Type here" };
		const uint64 topDefaultOffset ( 5 );
		winapi::rect parentWindowSize;
		
		GetWindowRect(window, &parentWindowSize);
		
		{
			windowHandle richText = CreateWindowExW(
				0, 
				MSFTEDIT_CLASS, 
				windowDefaultText.Pointer(),
				ES_MULTILINE | WS_VISIBLE | WS_CHILD | WS_TABSTOP, 
				xOffset,
				yOffset,
				parentWindowSize.right - parentWindowSize.left - xOffset /*- 16 - (xOffset * 2)*/,
				parentWindowSize.bottom - parentWindowSize.top - topDefaultOffset - yOffset /*- 59*/,
				window, 
				NULL, 
				process, 
				NULL
			);
			
			/// Background color change.
			SendMessage(richText, EM_SETBKGNDCOLOR, 0, brushes::colors->primar); 
			
			{
				CHARFORMAT2 colorMap1 {{ 0 }};
				colorMap1.cbSize = sizeof ( CHARFORMAT2 ); // Indicate which version of the structure is being used.
				colorMap1.dwMask = CFM_BACKCOLOR | CFM_COLOR; // Text background color and text color
				colorMap1.crBackColor = brushes::colors->primar;
				colorMap1.crTextColor = brushes::colors->text;
				
				// X CHARFORMAT2 colorMap2 {{ 0 }};
				// X colorMap2.cbSize = sizeof ( CHARFORMAT2 ); // Indicate which version of the structure is being used.
				// X colorMap2.dwMask = CFM_BACKCOLOR | CFM_COLOR; // Text background color and text color
				// X colorMap2.crBackColor = brushes::colorPalette->textPrimary;
				// X colorMap2.crTextColor = brushes::colorPalette->backgroundSecondary;
				
				/// Character format (text color, text background color).
				SendMessage(richText, EM_SETCHARFORMAT, 0, (LPARAM)&colorMap1);	
				// X SendMessage(richText, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&colorMap2);
			}
		}
	}
	
}