#pragma once

namespace themes {

	using namespace winapi::window;

	const theme::theme* colorPalette { &theme::lightMode };
	theme::solidBrush backgroundPrimary, backgroundSecondary, backgroundHovered, backgroundSelected, border;

	block ChangeColorPalette(const theme::theme& newTheme) {
		colorPalette = &newTheme;
	}

	//block ReplaceBrushes() {
	//	backgroundSecondary.Replace((*colorPalette).backgroundSecondary);
	//	backgroundSelected.Replace((*colorPalette).backgroundSelected);
	//	backgroundHovered.Replace((*colorPalette).backgroundHovered);
	//	backgroundPrimary.Replace((*colorPalette).backgroundPrimary);
	//}

	block InitializeBrushes() {
		backgroundSecondary.Create((*colorPalette).backgroundSecondary);
		backgroundSelected.Create((*colorPalette).backgroundSelected);
		backgroundHovered.Create((*colorPalette).backgroundHovered);
		backgroundPrimary.Create((*colorPalette).backgroundPrimary);
		border.Create((*colorPalette).borderColor);
	}

	block Destroy() {
		backgroundSecondary.Destroy();
		backgroundSelected.Destroy();
		backgroundHovered.Destroy();
		backgroundPrimary.Destroy();
		border.Destroy();
	}


}

