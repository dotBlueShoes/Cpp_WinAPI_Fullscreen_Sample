#pragma once

// My understanding of what is happening...
//  basicly the uxtheme.lib file does not contain these definitions.
//  but the .dll itself does have them... and as it is answeard here:
//  https://stackoverflow.com/questions/3250467/what-is-inside-lib-file-of-static-library-statically-linked-dynamic-library-an
//  when we don't have a .lib and we want to comunicate with .dll
//  we need to manually call "LoadLibrary" and "FreeLibrary" alongside
//  with GetProcAddress to obtain the address of the functions and data items
//  stored in the .dll. 
//  (We also need to cast the returned address to an appropriate pointer-to-function in order)

// 1903, 1809 are specfied here
// https://docs.microsoft.com/en-us/windows/win32/accessibility/accessibility-whatsnew

#ifdef WINDOWS_VERSION_10

#include "../framework.hpp"
#include <uxtheme.h>

namespace darkmode {

	namespace proxy {

		enum WINDOWCOMPOSITIONATTRIB {
			WCA_UNDEFINED = 0,
			WCA_NCRENDERING_ENABLED = 1,
			WCA_NCRENDERING_POLICY = 2,
			WCA_TRANSITIONS_FORCEDISABLED = 3,
			WCA_ALLOW_NCPAINT = 4,
			WCA_CAPTION_BUTTON_BOUNDS = 5,
			WCA_NONCLIENT_RTL_LAYOUT = 6,
			WCA_FORCE_ICONIC_REPRESENTATION = 7,
			WCA_EXTENDED_FRAME_BOUNDS = 8,
			WCA_HAS_ICONIC_BITMAP = 9,
			WCA_THEME_ATTRIBUTES = 10,
			WCA_NCRENDERING_EXILED = 11,
			WCA_NCADORNMENTINFO = 12,
			WCA_EXCLUDED_FROM_LIVEPREVIEW = 13,
			WCA_VIDEO_OVERLAY_ACTIVE = 14,
			WCA_FORCE_ACTIVEWINDOW_APPEARANCE = 15,
			WCA_DISALLOW_PEEK = 16,
			WCA_CLOAK = 17,
			WCA_CLOAKED = 18,
			WCA_ACCENT_POLICY = 19,
			WCA_FREEZE_REPRESENTATION = 20,
			WCA_EVER_UNCLOAKED = 21,
			WCA_VISUAL_OWNER = 22,
			WCA_HOLOGRAPHIC = 23,
			WCA_EXCLUDED_FROM_DDA = 24,
			WCA_PASSIVEUPDATEMODE = 25,
			WCA_USEDARKMODECOLORS = 26,
			WCA_LAST = 27
		};

		struct WINDOWCOMPOSITIONATTRIBDATA {
			WINDOWCOMPOSITIONATTRIB Attrib;
			PVOID pvData;
			SIZE_T cbData;
		};

		enum IMMERSIVE_HC_CACHE_MODEE {
			IHCM_USE_CACHED_VALUE,
			IHCM_REFRESH
		};

		enum PreferredAppMode {
			Default,
			AllowDark,
			ForceDark,
			ForceLight,
			Max
		};

		// ----------
		// 1903 18362
		// ----------
		using rtlGetNtVersionNumbers =					void(__stdcall*)	(LPDWORD major, LPDWORD minor, LPDWORD build);	// 
		using setWindowCompositionAttribute =			int32(__stdcall*)	(HWND hWnd, WINDOWCOMPOSITIONATTRIBDATA*);		// 
		// ----------
		// 1809 17763
		// ----------
		using shouldAppsUseDarkMode =					bool(__stdcall*)	();												// ordinal 132
		using allowDarkModeForWindow =					bool(__stdcall*)	(HWND hWnd, bool allow);						// ordinal 133
		using allowDarkModeForApp =						bool(__stdcall*)	(bool allow);									// ordinal 135, in 1809
		using flushMenuThemes =							void(__stdcall*)	();												// ordinal 136
		using refreshImmersiveColorPolicyState =		void(__stdcall*)	();												// ordinal 104
		using isDarkModeAllowedForWindow =				bool(__stdcall*)	(HWND hWnd);									// ordinal 137
		using getIsImmersiveColorUsingHighContrast =	bool(__stdcall*)	(IMMERSIVE_HC_CACHE_MODEE mode);				// ordinal 106
		using openNcThemeData =							HTHEME(__stdcall*)	(HWND hWnd, LPCWSTR pszClassList);				// ordinal 49
		// ----------
		// 1903 18362
		// ----------
		using shouldSystemUseDarkMode =					bool(__stdcall*)	();												// ordinal 138
		using setPreferredAppMode =			PreferredAppMode(__stdcall*)	(PreferredAppMode appMode);						// ordinal 135, in 1903
		using isDarkModeAllowedForApp =					bool(__stdcall*)	();												// ordinal 139


		setWindowCompositionAttribute			SetWindowCompositionAttribute			{ nullptr };
		shouldAppsUseDarkMode					ShouldAppsUseDarkMode					{ nullptr };
		allowDarkModeForWindow					AllowDarkModeForWindow					{ nullptr };
		allowDarkModeForApp						AllowDarkModeForApp						{ nullptr };
		flushMenuThemes							FlushMenuThemes							{ nullptr };
		refreshImmersiveColorPolicyState		RefreshImmersiveColorPolicyState		{ nullptr };
		isDarkModeAllowedForWindow				IsDarkModeAllowedForWindow				{ nullptr };
		getIsImmersiveColorUsingHighContrast	GetIsImmersiveColorUsingHighContrast	{ nullptr };
		openNcThemeData							OpenNcThemeData							{ nullptr };
		// ----------
		// 1903 18362
		// ----------
		shouldSystemUseDarkMode					ShouldSystemUseDarkMode					{ nullptr };
		setPreferredAppMode						SetPreferredAppMode						{ nullptr };

	}

	bool isSupported { false };
	bool isEnabled { false };

	bool IsHighContrast() {
		HIGHCONTRASTW highContrast { sizeof highContrast };
		if (SystemParametersInfo(SPI_GETHIGHCONTRAST, sizeof(highContrast), &highContrast, FALSE))
			return highContrast.dwFlags & HCF_HIGHCONTRASTON;

		return false;
	}

	void Initialize() {
		if (moduleInstance uxtheme { LoadLibraryEx(L"uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32) }) {

			// Getting the functions from the .dll
			proxy::OpenNcThemeData =						(proxy::openNcThemeData)						GetProcAddress(uxtheme, MAKEINTRESOURCEA(49));
			proxy::RefreshImmersiveColorPolicyState =		(proxy::refreshImmersiveColorPolicyState)		GetProcAddress(uxtheme, MAKEINTRESOURCEA(104));
			proxy::GetIsImmersiveColorUsingHighContrast =	(proxy::getIsImmersiveColorUsingHighContrast)	GetProcAddress(uxtheme, MAKEINTRESOURCEA(106));
			proxy::ShouldAppsUseDarkMode =					(proxy::shouldAppsUseDarkMode)					GetProcAddress(uxtheme, MAKEINTRESOURCEA(132));
			proxy::AllowDarkModeForWindow =					(proxy::allowDarkModeForWindow)					GetProcAddress(uxtheme, MAKEINTRESOURCEA(133));
			proxy::SetPreferredAppMode =					(proxy::setPreferredAppMode)					GetProcAddress(uxtheme, MAKEINTRESOURCEA(135));
			proxy::IsDarkModeAllowedForWindow =				(proxy::isDarkModeAllowedForWindow)				GetProcAddress(uxtheme, MAKEINTRESOURCEA(137));

			{
				// Why the checking?
				// Well either each function is being checked or
				// we're sure everything is in the right place.
				// We're not, however theres prob. a way to establish certain
				// things at the beggining and make these if's go, however this
				// requires a good understanging of whats in specific windows versions and
				// quite a big overhead on top of the program.
				//  yeah most certainly a thing to do one day.

				moduleInstance user32 { nullptr };
				if (!GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, L"user32.dll", &user32))
					throw "Module address not found!";
				else {
					const char* functionName = "SetWindowCompositionAttribute";
					FARPROC functionAddress = GetProcAddress(user32, functionName);

					if (functionAddress == NULL) {
						throw "Function address not found!";
						return;
					}

					proxy::SetWindowCompositionAttribute = (proxy::setWindowCompositionAttribute)functionAddress;
				}
			}

			FreeLibrary(uxtheme);

			if (isSupported = proxy::OpenNcThemeData &&
				proxy::RefreshImmersiveColorPolicyState &&
				proxy::ShouldAppsUseDarkMode &&
				proxy::AllowDarkModeForWindow &&
				proxy::SetPreferredAppMode &&
				proxy::IsDarkModeAllowedForWindow) {

				//proxy::AllowDarkModeForApp(true);
				proxy::SetPreferredAppMode(proxy::AllowDark);
				proxy::RefreshImmersiveColorPolicyState();

				isEnabled = proxy::ShouldAppsUseDarkMode() && !IsHighContrast();

			}
		}
	}

}


#endif

//extern "C" {
//	NTSYSAPI	void stdcall RtlGetNtVersionNumbers			(sal_io LPDWORD major, sal_io LPDWORD minor, sal_io LPDWORD build);
//	WINUSERAPI	BOOL stdcall SetWindowCompositionAttribute	(sal_in HWND hWnd, sal_in WINDOWCOMPOSITIONATTRIBDATA*);
	// 1809 17763
//	THEMEAPI_(bool) ShouldAppsUseDarkMode(); // ordinal 132
//	THEMEAPI_(bool) AllowDarkModeForWindow(HWND hWnd, bool allow); // ordinal 133
//	THEMEAPI_(bool) AllowDarkModeForApp(bool allow); // ordinal 135, in 1809
//	THEMEAPI_(VOID) FlushMenuThemes(); // ordinal 136
//	THEMEAPI_(VOID) RefreshImmersiveColorPolicyState(); // ordinal 104
//	THEMEAPI_(bool) IsDarkModeAllowedForWindow(HWND hWnd); // ordinal 137
//	THEMEAPI_(bool) GetIsImmersiveColorUsingHighContrast(IMMERSIVE_HC_CACHE_MODE mode); // ordinal 106
//	THEMEAPI_(HTHEME) OpenNcThemeData(HWND hWnd, LPCWSTR pszClassList); // ordinal 49
//}