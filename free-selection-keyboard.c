#include "free-selection-keyboard.h"

/* free selection keyboard:
 * =======================
 * choose keyboard key with joyL or joyR and press it
 * using L/R shoulder */

void freeSelectionKeyboardInit (struct free_selection_keyboard_state *keyboard_state){
	keyboard_state->left_bumper = KEY_RELEASED;
	keyboard_state->right_bumper = KEY_RELEASED;

	keyboard_state->selected_key_L_index = 7;
	keyboard_state->selected_key_R_index = 7;

	keyboard_state->joyL.x = 0;
	keyboard_state->joyL.y = 0;

	keyboard_state->joyR.x = 0;
	keyboard_state->joyR.y = 0;

	initKeysAreas(keyboard_state->grid_L, keyboard_state->grid_R);
}

int fsUpdateRenderer( struct free_selection_keyboard_state *keyboard_state,
		SDL_Renderer *pRenderer, SDL_Texture *pTexture){
		SDL_Rect rect_L;
		SDL_Rect rect_R;

		SDL_RenderCopy(pRenderer, pTexture, NULL, NULL);

		rect_L.x = 3 + 63*keyboard_state->grid_L[keyboard_state->selected_key_L_index].graphic_pos.x
			+ 20*keyboard_state->grid_L[keyboard_state->selected_key_L_index].graphic_pos.y;
		rect_L.y = 17 + 54*keyboard_state->grid_L[keyboard_state->selected_key_L_index].graphic_pos.y;
		rect_L.h = 50;
		rect_L.w = 60;

		rect_R.x = 3 + 63*keyboard_state->grid_R[keyboard_state->selected_key_R_index].graphic_pos.x
			+ 20*keyboard_state->grid_R[keyboard_state->selected_key_R_index].graphic_pos.y;
		rect_R.y = 17 + 54*keyboard_state->grid_R[keyboard_state->selected_key_R_index].graphic_pos.y;
		rect_R.h = 50;
		rect_R.w = 60;

		SDL_SetRenderDrawColor(pRenderer, 239, 239, 239, 255);
		SDL_RenderDrawRect(pRenderer, &rect_L);
		SDL_RenderDrawRect(pRenderer, &rect_R);

        SDL_RenderPresent(pRenderer);

		return 0;
}

/* main loop */
int freeSelectionKeyboard (struct mapping *gamepad_map,
		char gamepad_number, SDL_Renderer *pRenderer,
		SDL_Haptic *pHaptic){
	struct joy_event gamepad_event;
	enum key_value key_value;
	struct free_selection_keyboard_state keyboard_state;
	int rumble_effect_id;
	SDL_Surface *pImage;
	SDL_Texture * pTexture;

	/* background image */
	pImage = SDL_LoadBMP("azerty.bmp");
	pTexture = SDL_CreateTextureFromSurface(pRenderer, pImage);

	freeSelectionKeyboardInit(&keyboard_state);
	fsUpdateRenderer(&keyboard_state, pRenderer, pTexture);

	if (gamepad_map->rumble)
		rumble_effect_id = fsCreateShortRumbleEffect(pHaptic);
	else
		rumble_effect_id = -1;

	/* pour les tests, on utilise pas d'interface graphique,
	 * on affiche juste le texte résultant */
	printf("début du texte:\n\n");

	while( getEvent(&gamepad_event) == 0 ){
		if( gamepad_event.gamepad_number == gamepad_number ){
			if (digitalRead(&(gamepad_map->left_bumper), &gamepad_event,
						&key_value)){
				/* stop rumble if necessary */
				if (rumble_effect_id >= 0)
					SDL_HapticStopEffect(pHaptic, rumble_effect_id);

				if ((key_value == KEY_PRESSED) &&
						(keyboard_state.left_bumper == KEY_RELEASED)){
					printf("%c",key2Char(
								keyboard_state.grid_L[keyboard_state.selected_key_L_index].keycode));
					fflush(stdout);
				}
				/* if the button is an axis, the button is triggered
				 * only once */
				keyboard_state.left_bumper = key_value;
			}
			else if (digitalRead(&(gamepad_map->right_bumper), &gamepad_event,
						&key_value)){
				if (rumble_effect_id >= 0)
					SDL_HapticStopEffect(pHaptic, rumble_effect_id);

				if ((key_value == KEY_PRESSED) &&
						(keyboard_state.right_bumper == KEY_RELEASED)){
					printf("%c",key2Char(
								keyboard_state.grid_R[keyboard_state.selected_key_R_index].keycode));
					fflush(stdout);
				}
				keyboard_state.right_bumper = key_value;
			}
			else if (joystickRead(&(gamepad_map->joy_L), &gamepad_event,
						&(keyboard_state.joyL))){

				for (int i = 0; i < 19; i++) {

					if (pointInConvexPoly(keyboard_state.joyL,
								&(keyboard_state.grid_L[i].area))){

						int keycode = keyboard_state.grid_L[i].keycode;

						if (keyboard_state.grid_L[keyboard_state.selected_key_L_index].keycode != keycode){
							keyboard_state.selected_key_L_index = i;

							/* run rumble */
							if (rumble_effect_id >= 0){
								SDL_HapticStopEffect(pHaptic, rumble_effect_id);

								if (SDL_HapticRunEffect( pHaptic, rumble_effect_id, 1 ) < 0)
									fprintf(stderr,
											"ERROR: cannot rumble the joystick, (%s)\n",
											SDL_GetError());
							}

							fsUpdateRenderer(&keyboard_state, pRenderer, pTexture);
						}
					}
				}
			}
			else if (joystickRead(&(gamepad_map->joy_R), &gamepad_event,
						&(keyboard_state.joyR))){

				for (int i = 0; i < 19; i++) {

					if (pointInConvexPoly(keyboard_state.joyR,
								&(keyboard_state.grid_R[i].area))){

						int keycode = keyboard_state.grid_R[i].keycode;

						if (keyboard_state.grid_R[keyboard_state.selected_key_R_index].keycode != keycode){
							keyboard_state.selected_key_R_index = i;

							if (rumble_effect_id >= 0){
								SDL_HapticStopEffect(pHaptic, rumble_effect_id);

								if (SDL_HapticRunEffect( pHaptic, rumble_effect_id, 1 ) < 0)
									fprintf(stderr,
											"ERROR: cannot rumble the joystick, (%s)\n",
											SDL_GetError());
							}

							fsUpdateRenderer(&keyboard_state, pRenderer, pTexture);
						}
					}
				}
			}
		}
	}

	/* We destroy the effect, although closing the device also does this */
	if (rumble_effect_id >= 0)
		SDL_HapticDestroyEffect( pHaptic, rumble_effect_id );

	fsFreeKeysAreas(keyboard_state.grid_L, keyboard_state.grid_R);

	/* destroy the background image */
	SDL_DestroyTexture(pTexture);
	SDL_FreeSurface(pImage);

	return 0;
}

int fsCreateShortRumbleEffect ( SDL_Haptic *pHaptic) {
	int effect_id;
	SDL_HapticEffect effect;

	/* Create the effect */
	/* 0 is safe default */
	SDL_memset( &effect, 0, sizeof(SDL_HapticEffect) );
	effect.type = SDL_HAPTIC_SINE;
	/* Polar coordinates */
	effect.periodic.direction.type = SDL_HAPTIC_POLAR;
	/* Force comes from south */
	effect.periodic.direction.dir[0] = 18000;

	effect.periodic.period = 1000;
	effect.periodic.magnitude = 30000;
	/* 20ms long */
	effect.periodic.length = 50;
	/* Takes 0 second to get max strength */
	effect.periodic.attack_length = 0;
	/* Takes 20ms to fade away */
	effect.periodic.fade_length = 0;

	/* Upload the effect */
	effect_id = SDL_HapticNewEffect( pHaptic, &effect );

	if(effect_id < 0)
		fprintf(stderr,"ERROR: cannot rumble the joystick, (%s)\n",
				SDL_GetError());

	return effect_id;
}

char key2Char (int keycode){

	switch (keycode) {
		case KEY_A:
			return 'a';
		case KEY_B:
			return 'b';
		case KEY_C:
			return 'c';
		case KEY_D:
			return 'd';
		case KEY_E:
			return 'e';
		case KEY_F:
			return 'f';
		case KEY_G:
			return 'g';
		case KEY_H:
			return 'h';
		case KEY_I:
			return 'i';
		case KEY_J:
			return 'j';
		case KEY_K:
			return 'k';
		case KEY_L:
			return 'l';
		case KEY_M:
			return 'm';
		case KEY_N:
			return 'n';
		case KEY_O:
			return 'o';
		case KEY_P:
			return 'p';
		case KEY_Q:
			return 'q';
		case KEY_R:
			return 'r';
		case KEY_S:
			return 's';
		case KEY_T:
			return 't';
		case KEY_U:
			return 'u';
		case KEY_V:
			return 'v';
		case KEY_W:
			return 'w';
		case KEY_X:
			return 'x';
		case KEY_Y:
			return 'y';
		case KEY_Z:
			return 'z';
		case KEY_COMMA:
			return ',';
		case KEY_DOT:
			return '.';
		case KEY_SLASH:
			return '/';
		case KEY_SPACE:
			return ' ';
	}
	return 0;
}

void fsFreeKeysAreas (struct fs_keyboard_trigger_area grid_L[19],
		struct fs_keyboard_trigger_area grid_R[19]){

	for (int i = 0; i < 19; i++) {
		free(grid_L[i].area.vertex_array);
		free(grid_R[i].area.vertex_array);
	}
}

/* fill an element of the trigger grid list */
void fillFSTriggerArea (struct fs_keyboard_trigger_area *area,
		int keycode, char vertex_len, struct vec2 *vertex_array,
		int graphic_pos_x, int graphic_pos_y)
{
	area->keycode = keycode;
	area->area.vertex_len = vertex_len;

	area->graphic_pos.x = graphic_pos_x;
	area->graphic_pos.y = graphic_pos_y;

	area->area.vertex_array =
		(struct vec2 *)malloc(vertex_len * sizeof(struct vec2));

	for (int i = 0; i < vertex_len; i++)
		area->area.vertex_array[i] = vertex_array[i];
}

/* fill the trigger grid lists for all joysticks
 * to know where is each key */
void initKeysAreas (struct fs_keyboard_trigger_area grid_L[19],
		struct fs_keyboard_trigger_area grid_R[19]){

	/* take care to the order (otherwise the polygon will
	 * not be convex */
	struct vec2 a[] = { {-128,16}, {-97,10}, {-79,70}, {-128,127} };
	fillFSTriggerArea(&(grid_L[0]), KEY_A, 4, a, 0, 0);
	fillFSTriggerArea(&(grid_R[0]), KEY_Y, 4, a, 6, 0);

	struct vec2 z[] = { {-128,127}, {-79,70}, {-9,93}, {-11,127} };
	fillFSTriggerArea(&(grid_L[1]), KEY_Z, 4, z, 1, 0);
	fillFSTriggerArea(&(grid_R[1]), KEY_U, 4, z, 7, 0);

	struct vec2 q[] = { {-128,16}, {-97,10}, {-97,-10}, {-128,-16} };
	fillFSTriggerArea(&(grid_L[2]), KEY_Q, 4, q, 0, 1);
	fillFSTriggerArea(&(grid_R[2]), KEY_H, 4, q, 6, 1);

	struct vec2 w[] = { {-128,-16}, {-97,-10}, {-79,-70}, {-128,-128} };
	fillFSTriggerArea(&(grid_L[3]), KEY_W, 4, w, 0, 2);
	fillFSTriggerArea(&(grid_R[3]), KEY_N, 4, w, 6, 2);

	struct vec2 x[] = { {-128,-128}, {-79,-70}, {-9,-93}, {-11,-128} };
	fillFSTriggerArea(&(grid_L[4]), KEY_X, 4, x, 1, 2);
	fillFSTriggerArea(&(grid_R[4]), KEY_COMMA, 4, x, 7, 2);

	struct vec2 e[] = { {-11,127}, {-9,93}, {0,52}, {9,93}, {11,127} };
	fillFSTriggerArea(&(grid_L[5]), KEY_E, 5, e, 2, 0);
	fillFSTriggerArea(&(grid_R[5]), KEY_I, 5, e, 8, 0);

	struct vec2 c[] = { {-11,-128}, {-9,-93}, {0,-52},
		{9,-93}, {11,-128} };
	fillFSTriggerArea(&(grid_L[6]), KEY_C, 5, c, 2, 2);
	fillFSTriggerArea(&(grid_R[6]), KEY_DOT, 5, c, 8, 2);

	struct vec2 d[] = { {-7,0}, {0,52}, {7,0}, {0,-52} };
	fillFSTriggerArea(&(grid_L[7]), KEY_D, 4, d, 2, 1);
	fillFSTriggerArea(&(grid_R[7]), KEY_K, 4, d, 8, 1);

	struct vec2 r[] = { {127,127}, {79,70}, {9,93}, {11,127} };
	fillFSTriggerArea(&(grid_L[8]), KEY_R, 4, r, 3, 0);
	fillFSTriggerArea(&(grid_R[8]), KEY_O, 4, r, 9, 0);

	struct vec2 v[] = { {127,-128}, {79,-70}, {9,-93}, {11,-128} };
	fillFSTriggerArea(&(grid_L[9]), KEY_V, 4, v, 3, 2);
	fillFSTriggerArea(&(grid_R[9]), KEY_SLASH, 4, v, 9, 2);

	struct vec2 g[] = { {127,16}, {97,10}, {97,-10}, {127,-16} };
	fillFSTriggerArea(&(grid_L[10]), KEY_G, 4, g, 4, 1);
	fillFSTriggerArea(&(grid_R[10]), KEY_M, 4, g, 10, 1);

	struct vec2 t[] = { {127,16}, {97,10}, {79,70}, {127,127} };
	fillFSTriggerArea(&(grid_L[11]), KEY_T, 4, t, 4, 0);
	fillFSTriggerArea(&(grid_R[11]), KEY_P, 4, t, 10, 0);

	struct vec2 b[] = { {127,-16}, {97,-10}, {79,-70}, {127,-128} };
	fillFSTriggerArea(&(grid_L[12]), KEY_B, 4, b, 4, 2);
	fillFSTriggerArea(&(grid_R[12]), KEY_SPACE, 4, b, 10, 2);

	/* because we need convex polygons, the S and F areas will be
	 * divided in 3 parts */
	struct vec2 s1[] = { {-97,10}, {-7,0}, {0,52}, {-9,93}, {-79,70} };
	fillFSTriggerArea(&(grid_L[13]), KEY_S, 5, s1, 1, 1);
	fillFSTriggerArea(&(grid_R[13]), KEY_J, 5, s1, 7, 1);

	struct vec2 s2[] = { {-97,10}, {-7,0}, {-97,-10} };
	fillFSTriggerArea(&(grid_L[14]), KEY_S, 3, s2, 1, 1);
	fillFSTriggerArea(&(grid_R[14]), KEY_J, 3, s2, 7, 1);

	struct vec2 s3[] = { {-97,-10}, {-7,0}, {0,-52}, {-9,-93}, {-79,-70} };
	fillFSTriggerArea(&(grid_L[15]), KEY_S, 5, s3, 1, 1);
	fillFSTriggerArea(&(grid_R[15]), KEY_J, 5, s3, 7, 1);

	struct vec2 f1[] = { {97,10}, {7,0}, {0,52}, {9,93}, {79,70} };
	fillFSTriggerArea(&(grid_L[16]), KEY_F, 5, f1, 3, 1);
	fillFSTriggerArea(&(grid_R[16]), KEY_L, 5, f1, 9, 1);

	struct vec2 f2[] = { {97,10}, {7,0}, {97,-10} };
	fillFSTriggerArea(&(grid_L[17]), KEY_F, 3, f2, 3, 1);
	fillFSTriggerArea(&(grid_R[17]), KEY_L, 3, f2, 9, 1);

	struct vec2 f3[] = { {97,-10}, {7,0}, {0,-52}, {9,-93}, {79,-70} };
	fillFSTriggerArea(&(grid_L[18]), KEY_F, 5, f3, 3, 1);
	fillFSTriggerArea(&(grid_R[18]), KEY_L, 5, f3, 9, 1);
}
