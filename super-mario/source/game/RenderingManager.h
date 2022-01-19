#ifndef _RENDERINGMANAGER_H_
#define _RENDERINGMANAGER_H_

#include "./GameVars.h"
#include "./Mario.h"

void GridDisplay() {
	//GridUtilities::DisplayGrid(al_get_backbuffer(display), gameMap->GetViewWindow(), *grid, TILEMAP_WIDTH);
	GridComputation::DisplayGrid(al_get_backbuffer(display), gameMap->GetViewWindow(), gameMap->GetGrid()->GetBuffer(), gameMap->GetTotalColumns());
	/*if (TOGGLE_FILLED_RECT) {
		al_draw_filled_rectangle(-viewWin->x + gridWin->x, -viewWin->y + gridWin->y, -viewWin->x + gridWin->x + gridWin->w, -viewWin->y + gridWin->y + gridWin->h, al_map_rgb(255, 0, 0));
	}
	else {
		al_draw_rectangle(-viewWin->x + gridWin->x, -viewWin->y + gridWin->y, -viewWin->x + gridWin->x + gridWin->w, -viewWin->y + gridWin->y + gridWin->h, al_map_rgb(255, 0, 0), 1.0);
	}*/

}

void Render() {
	
	gameMap->Display(al_get_backbuffer(display));
	Mario *mario = new Mario();
	//mario->move();

	if (SHOW_GRID_DEBUG) {
		GridDisplay();
	}

	al_flip_display();
}


#endif _RENDERINGMANAGER_H_