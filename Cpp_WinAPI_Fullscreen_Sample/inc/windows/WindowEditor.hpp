#pragma once
#include "Framework.hpp"

using namespace winapi::window;

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
	
	//EDITSTREAMCALLBACK RtfStreamCallback;

	//DWORD MEditStreamInCallback(
	//	[in] DWORD_PTR dwCookie,
	//	[in] LPBYTE pbBuff,
	//	[in] LONG cb,
	//	[in] LONG *pcb
	//) {
	//	CString *psBuffer = (CString *)dwCookie;
	//
	//		if (cb < psBuffer->GetLength()) cb = psBuffer->GetLength();
	//
	//		for (int i = 0; i < cb; i++)
	//			*(pbBuff + i) = psBuffer->GetAt(i);
	//		*pcb = cb;
	//		*psBuffer = psBuffer->Mid(cb);
	//		
	//	return 0;
	//}
	
	const array<winapi::wchar, 10> sample { L"Type here" };
	uint64 xOffset ( 2 );

	block CreateEditor(const handleInstnace& process, const windowHandle& window) {

		RECT parentWindowSize;
		GetWindowRect(window, &parentWindowSize);
		
		//const Rect otherhuh (parentWindowSize);
		
		
		
		// [in] const INT  *lpDx

		// 1 windowHandle richText = CreateWindowExW(
		// 1 	0, 
		// 1 	MSFTEDIT_CLASS, 
		// 1 	sample.Pointer(),
		// 1 	ES_MULTILINE | WS_VISIBLE | WS_CHILD | WS_TABSTOP, 
		// 1 	xOffset /*-1*/,  // X
		// 1 	0,	// Y
		// 1 	parentWindowSize.right - parentWindowSize.left - 16 - (xOffset * 2), // Width
		// 1 	parentWindowSize.bottom - parentWindowSize.top - 59, // Height
		// 1 	window, NULL, process, NULL);

		// Background color change
		// 1 SendMessage(richText, EM_SETBKGNDCOLOR, 0, themes::colorPalette->backgroundPrimary); 
		
		CHARFORMAT2 colorMap1 {{ 0 }};
		colorMap1.cbSize = sizeof ( CHARFORMAT2 ); // Indicate which version of the structure is being used.
		colorMap1.dwMask = CFM_BACKCOLOR | CFM_COLOR; // Text background color and text color
		colorMap1.crBackColor = themes::colorPalette->backgroundPrimary;
		colorMap1.crTextColor = themes::colorPalette->textPrimary;
		
		// X CHARFORMAT2 colorMap2 {{ 0 }};
		// X colorMap2.cbSize = sizeof ( CHARFORMAT2 ); // Indicate which version of the structure is being used.
		// X colorMap2.dwMask = CFM_BACKCOLOR | CFM_COLOR; // Text background color and text color
		// X colorMap2.crBackColor = themes::colorPalette->textPrimary;
		// X colorMap2.crTextColor = themes::colorPalette->backgroundSecondary;
		
		// 1 SendMessage(richText, EM_SETCHARFORMAT, 0, (LPARAM)&colorMap1);	// The default character format is set.
		// X SendMessage(richText, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&colorMap2);
		
		// X {
		// X 	EDITSTREAM stream;
		// X 	stream.dwCookie = (DWORD_PTR)sample.Pointer(); // pointer your rtf data
		// X 	stream.dwError = 0;
		// X 	stream.pfnCallback = (EDITSTREAMCALLBACK)RtfStreamCallback; // callback which will push down the next chunk of rtf data when needed
		// X 
		// X 	LRESULT bytesAccepted = 0;
		// X 	bytesAccepted = SendMessage(richText, EM_STREAMIN, SF_RTF, (LPARAM)&stream);
		// X }
		
		// X {
		// X 	CString sWriteText; //Where the text will be streamed from
		// X 	sWriteText = L"Rich text is shown here in sample project";
		// X 
		// X 	// This is hard-coded for example purposes. It is likely this would
		// X 	// be read from file or another source.
		// X 	EDITSTREAM es;
		// X 	es.dwCookie = (DWORD)&sWriteText; // Pass a pointer to the CString to the callback function
		// X 	es.pfnCallback = MEditStreamInCallback; // Specify the pointer to the callback function
		// X 
		// X 	SendMessage(richText, EM_STREAMIN, SF_RTF, (LPARAM)&es);
		// X 	//richText.StreamIn(SF_RTF, es); // Perform the streaming
		// X }

		// X SetWindowSubclass(window, richText , 0, 0); // uneeded right now.
	}

	proceeded stdcall Editor(
		windowHandle window,
		uint32 message,
		messageW wArgument,
		messageL lArgument,
		UINT_PTR uid, DWORD_PTR RefData
	) {
		// X switch (message) {
		// X	 case EM_SETBKGNDCOLOR: {
		// X	 	//MessageBoxEx(window, L"RICHEDIT", L"PaintCall", MB_OK, 0);
		// X	 	displayContextHandle displayContext = (displayContextHandle)wArgument;
		// X	 	SetTextColor(displayContext, (*themes::colorPalette).textPrimary);
		// X	 	SetBkColor(displayContext, (*themes::colorPalette).backgroundPrimary);
		// X	 	return ((INT_PTR)(themes::backgroundPrimary.Get()));
		// X	 }
		// X }
		return (proceeded)DefSubclassProc(window, (uint32)message, wArgument, lArgument);
	}

}