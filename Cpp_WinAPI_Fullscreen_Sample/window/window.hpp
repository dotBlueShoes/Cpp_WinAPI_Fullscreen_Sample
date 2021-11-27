#pragma once
#include "../framework.hpp"

namespace window {

	uint16 Register(
		const winapi::handleInstnace& windowInstance,
		const winapi::wchar* windowClassName,
		const winapi::windowProcedure& procedure
	) {
		winapi::windowClass windowProperties;
		windowProperties.cbSize = { sizeof winapi::windowClass };

		const winapi::iconHandle
			icon		{ LoadIcon(windowInstance, MAKEINTRESOURCE(IDI_CPPWINAPIFULLSCREENSAMPLE)) },
			iconSmall	{ LoadIcon(windowInstance, MAKEINTRESOURCE(IDI_SMALL)) };

		const winapi::wchar*		menuName		{ MAKEINTRESOURCEW(IDC_CPPWINAPIFULLSCREENSAMPLE) };
		const winapi::brushHandle	backgroundColor { (winapi::brushHandle)(COLOR_WINDOW + 1) };
		const winapi::cursorHandle	cursor			{ LoadCursor(nullptr, IDC_ARROW) };
		const uint32				windowStyle		{ CS_HREDRAW | CS_VREDRAW };

		windowProperties.style			= windowStyle;
		windowProperties.lpfnWndProc	= procedure;
		windowProperties.cbClsExtra		= 0;
		windowProperties.cbWndExtra		= 0;
		windowProperties.hInstance		= windowInstance;
		windowProperties.hIcon			= icon;
		windowProperties.hCursor		= cursor;
		windowProperties.hbrBackground	= backgroundColor;
		windowProperties.lpszMenuName	= menuName;
		windowProperties.lpszClassName	= windowClassName;
		windowProperties.hIconSm		= iconSmall;

		return RegisterClassExW(&windowProperties);
	}

	int64 Initialize(
		const winapi::handleInstnace& windowInstance,
		const winapi::wchar* windowClassName,
		const winapi::wchar* windowTitle,
		const int32& nCmdShow
	) {
		winapi::windowHandle window = CreateWindowEx(
			WS_EX_APPWINDOW,
			windowClassName,
			windowTitle,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0,
			CW_USEDEFAULT, 0,
			nullptr,
			nullptr,
			windowInstance,
			nullptr
		);

		if (!window) return 1;

		ShowWindow(window, nCmdShow);
		UpdateWindow(window);

		return 0;
	}

}