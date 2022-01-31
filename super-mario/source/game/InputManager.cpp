#include "InputManager.h"

void debugShortcuts() {

	int keyboard_offset = 16; //scroll offset should be always multiple of 16 ! (cant display half of a tile)

	if (al_key_down(&keyboard_state, ALLEGRO_KEY_RIGHT)) {
		if (gameMap->GetViewWindow().x + gameMap->GetViewWindow().w < SCROLLABLE_TILE_COL * TILE_WIDTH) {
			gameMap->ScrollWithBoundsCheck(keyboard_offset, 0);
		}
	}

	if (al_key_down(&keyboard_state, ALLEGRO_KEY_LEFT)) {
		gameMap->ScrollWithBoundsCheck(-keyboard_offset, 0);
		//std::cout << " ------------------------ Pressed Left arrow!" << std::endl;
	}

	if (al_key_down(&keyboard_state, ALLEGRO_KEY_HOME)) {
		gameMap->SetViewWindow(Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
	}

	if (al_key_down(&keyboard_state, ALLEGRO_KEY_END)) {
		gameMap->SetViewWindow(Rect(MAX_PIXEL_WIDTH - gameMap->GetViewWindow().w, MAX_PIXEL_HEIGHT - gameMap->GetViewWindow().h, SCREEN_WIDTH, SCREEN_HEIGHT));
	}

	if (al_mouse_button_down(&mouse_state, 1)) { //1 is left click
		int dx = 0, dy = 0;
		/* We did not understand dx,dy meaning in instructions :P */
		if (mouse_state.x > SCREEN_WIDTH / 2) {
			dx = 16;
		}
		else {
			dx = -16;
		}

		if (mouse_state.y > SCREEN_HEIGHT / 2) {
			dy = 16;
		}
		else {
			dy = -16;
		}

		gameMap->ScrollWithBoundsCheck(dx, dy);
		//gameMap->ScrollWithBoundsCheck(1, 0);
	}

	if (al_key_down(&keyboard_state, ALLEGRO_KEY_1) && al_key_down(&keyboard_state, ALLEGRO_KEY_LCTRL)) {
		SHOW_GRID_DEBUG = 1;
	}

	if (al_key_down(&keyboard_state, ALLEGRO_KEY_2) && al_key_down(&keyboard_state, ALLEGRO_KEY_LCTRL)) {
		SHOW_GRID_DEBUG = 0;
	}

	if (al_key_down(&keyboard_state, ALLEGRO_KEY_3) && al_key_down(&keyboard_state, ALLEGRO_KEY_LCTRL)) {
		TOGGLE_FILLED_RECT = 0;
	}

	if (al_key_down(&keyboard_state, ALLEGRO_KEY_4) && al_key_down(&keyboard_state, ALLEGRO_KEY_LCTRL)) {
		TOGGLE_FILLED_RECT = 1;
	}

}


void Input() {

	al_get_keyboard_state(&keyboard_state);
	al_get_mouse_state(&mouse_state);

	debugShortcuts();

	bool keypress = false;

	if (al_key_down(&keyboard_state, ALLEGRO_KEY_D)) {
		keypress = true;

		if (al_key_down(&keyboard_state, ALLEGRO_KEY_B)) {
			Mario::GetSingleton().runRight();
		}
		else
			Mario::GetSingleton().walkRight();

	}

	if (al_key_down(&keyboard_state, ALLEGRO_KEY_A)) {
		keypress = true;

		if (al_key_down(&keyboard_state, ALLEGRO_KEY_B))
			Mario::GetSingleton().runLeft();
		else
			Mario::GetSingleton().walkLeft();
	}

	if (al_key_down(&keyboard_state, ALLEGRO_KEY_W)) {
		keypress = true;
		Mario::GetSingleton().jump();
		//SoundPlayer::playSound("small_mario_jump"); //na paei inside jump? //OXI
	}

	if (!keypress) {
		Mario::GetSingleton().backToIdle();
	}

	Mario::GetSingleton().AlignViewWin(gameMap);
}