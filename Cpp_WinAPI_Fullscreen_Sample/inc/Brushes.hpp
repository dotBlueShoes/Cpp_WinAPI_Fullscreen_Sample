#pragma once
#include "Framework.hpp"

namespace brushes {
	
	const winapi::window::theme::palette<6>* colors { &winapi::window::theme::light };
	
	winapi::window::theme::solidBrush 
		primar, 
		second, 
		hovered, 
		selected, 
		border;
		
	// What have beed Initialized needs to be Destroyed!
	block Initialize() {
		selected.Create(colors->selected);
		hovered.Create(colors->hovered);
		primar.Create(colors->primar);
		second.Create(colors->second);
		border.Create(colors->border);
	}
	
	// Previously used palette if not needed is future use needs to be Destroyed.
	//  Newly loaded one have to re-initialized and destroyed when 
	//  we're done using these defined colors.
	block ChangePalette(const winapi::window::theme::palette<6>& newColors) { 
		colors = &newColors; 
	}

	block Destroy() {
		selected.Destroy();
		hovered.Destroy();
		primar.Destroy();
		second.Destroy();
		border.Destroy();
	}
	
}