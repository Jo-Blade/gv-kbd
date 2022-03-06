#ifndef MAIN_H
#define MAIN_H

#include <SDL2/SDL.h>

#include "config.h"
#include "free-selection-keyboard.h"

#define VERSION "1.0"
#define WIN_WIDTH 742
#define WIN_HEIGHT 190

int openJoy (SDL_Joystick** pJoystick, SDL_Haptic** pHaptic,
		struct mapping* gamepad_map, char gamepad_number);

#endif
