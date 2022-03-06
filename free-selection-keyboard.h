#ifndef GVKBD_MODE_1
#define GVKBD_MODE_1

#include <linux/input-event-codes.h>

#include "config.h"
#include "input.h"
#include "2d-engine.h"

/* trigger area for a specific letter */
struct fs_keyboard_trigger_area {
	int keycode;
	struct poly area;

	/* the position of the key (in number of keys)
	 * on the graphical virtual keyboard */
	struct vec2 graphic_pos;
};

struct free_selection_keyboard_state {
	enum key_value left_bumper;
	enum key_value right_bumper;

	/* index in [[0;19]] of the selected trigger
	 * area struct */
	int selected_key_L_index;
	struct vec2 joyL;

	int selected_key_R_index;
	struct vec2 joyR;

	struct fs_keyboard_trigger_area grid_L[19];
	struct fs_keyboard_trigger_area grid_R[19];
};

void freeSelectionKeyboardInit
(struct free_selection_keyboard_state *keyboard_state);

int fsUpdateRenderer( struct free_selection_keyboard_state *keyboard_state,
		SDL_Renderer *pRenderer, SDL_Texture *pTexture);

int freeSelectionKeyboard (struct mapping *gamepad_map,
		char gamepad_number, SDL_Renderer *pRenderer,
		SDL_Haptic *pHaptic);

void fillFSTriggerArea (struct fs_keyboard_trigger_area *area,
		int keycode, char vertex_len, struct vec2 *vertex_array,
		int graphic_pos_x, int graphic_pos_y);

void initKeysAreas (struct fs_keyboard_trigger_area grid_L[19],
		struct fs_keyboard_trigger_area grid_R[19]);

void fsFreeKeysAreas (struct fs_keyboard_trigger_area grid_L[19],
		struct fs_keyboard_trigger_area grid_R[19]);

int fsCreateShortRumbleEffect ( SDL_Haptic *pHaptic );

/* used for tests */
char key2Char (int keycode);

#endif
