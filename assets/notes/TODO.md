// The entry of the application and it's main procedure.

// HERE! DLL Library
// https://docs.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-freelibrary
// It might be possible to dynamically asociate the window procedure.
// https://stackoverflow.com/questions/31648180/c-changing-hwnd-window-procedure-in-runtime

// TODO
//  1. i know how to say to a user that a specifc feature is not available due to os he is using.
//   now i need to not ship the code that wont work under his version to do this i need macros
//   https://docs.microsoft.com/pl-pl/windows/win32/winprog/using-the-windows-headers?redirectedfrom=MSDN
//   so the darkmode api needs to be wraped in those and an alternative for other versions should exist.
//   Maybe i should create my own macros that i could turn on off and see what parts of code highlights then.
//   What about windows server versions?
//  2. Is IsWindows10OrGreater() needed? maybe if i pull out a macro that would say "NO_WINDOWS_VERSION_SPECIFIED"
//   that would mean that we're shipping the code with all versions code but we do those IsWindowsXOrGreater checks
//   why would someone like that... - i guess no intallation issues, no extra doing there.
//  3. flag64 variable, think about it, what it would cost, what it would optimise.
//   Would it be possible to create a enum class on top of that to have const masks for checks needed?
//   Iterestingly i belief that it's possible to override what function from dll returns and instead of BOOL return an uint32 readable value
//   tho it requires replacing in first place and after the calling of function is it worthit and acctually doable?
//  6. Create a border for menu items using https://stackoverflow.com/questions/16159127/win32-how-to-draw-a-rectangle-around-a-text-string
//  7. Enum inheritance https://www.codeproject.com/articles/16150/inheriting-a-c-enum-type

// flag64 darkmodeFlag
// enum class darkmodeFlag : flag64 {
//	isSupported : 0b0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000
// };
// See what we got.
//array<wchar, 3> buffor;
//Uint64ToWcharsTerminated<3>(buffor, darkmode::isEnabled, '\n');
//MessageBoxEx(window, L"woah", buffor.Pointer(), MB_OK, 0);
// handle leaks
// https://codereview.stackexchange.com/questions/20181/correct-way-of-using-hbrushes


// Controls and fun ...
// https://docs.microsoft.com/en-us/windows/win32/controls/property-sheets
// https://docs.microsoft.com/en-us/windows/win32/controls/tooltip-control-reference
// https://cpp0x.pl/kursy/Kurs-WinAPI-C++/167
// https://docs.microsoft.com/en-us/cpp/overview/visual-cpp-samples?view=msvc-170

// 2. Brushes Prob - uneeded memory operations.
// 3. Fullscreen implementation.

// So i can return to these.
//MSG msg { 0 };
//PeekMessage(&msg, window, 0, 0, PM_NOREMOVE);
//while (msg.message == WM_SETTINGCHANGE) {
//	PeekMessage(&msg, window, 0, 0, PM_REMOVE);
//	PeekMessage(&msg, window, 0, 0, PM_NOREMOVE);
//}
//MessageBoxEx(window, L"Main", L"PaintCall", MB_OK, 0);



# TODO
- registry
- child windows
- a focus test on those child windows would be great.
- richedit custom (defined by me) procedure.
- register and initialize redo.
- how to get data in and out of richedit.
- more of [in] [out] use.
- clear this todo and other comments.
- create a repo for registryToolApp

# LINKS

- https://stackoverflow.com/questions/33261966/what-is-app-aps-file-in-visual-c // .APS file
- https://docs.microsoft.com/en-us/windows/win32/midl/out-idl // [in], [out] and others
- https://docs.microsoft.com/en-us/windows/win32/controls/individual-control-info // All windows oficial controls