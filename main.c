#include "main.h"

int openJoy (SDL_Joystick** pJoystick, SDL_Haptic** pHaptic,
		struct mapping* gamepad_map, char gamepad_number){
	/* check if the device exist */
	if (gamepad_number >= SDL_NumJoysticks()){
		fprintf(stderr, "ERROR: cannot found joystick with index %d, no more joysticks\n",
				gamepad_number);
		return -1;
	}

	/* open the selected gamepad */
	*pJoystick = SDL_JoystickOpen(gamepad_number);
	if ( pJoystick == NULL )
	{
		fprintf(stderr,"ERROR: cannot open joystick with index %d, (%s)\n",
				gamepad_number, SDL_GetError());
		return -1;
	}

	if (gamepad_map->rumble){
		*pHaptic = SDL_HapticOpenFromJoystick(*pJoystick);
		/* Most likely joystick isn't haptic */
		if ( pHaptic == NULL ){
			fprintf(stderr,"ERROR: cannot allow rumble for joystick %d, (%s)\n",
					gamepad_number, SDL_GetError());
			gamepad_map->rumble = false;

			return 0;
		}

		/* See if it can do sine waves */
		if ((SDL_HapticQuery(*pHaptic) & SDL_HAPTIC_SINE)==0) {
			fprintf(stderr, "ERROR: joystick does not support haptic sine effect:disable rumble\n");
			gamepad_map->rumble = false;

			return 0;
		}

		printf("rumble is activated\n");
	}
	else printf("rumble is disabled\n");

	return 0;
}

int main(/*int argc, char* argv[]*/)
{
	char gamepad_number;
	struct mapping gamepad_map;
	SDL_Window* pWindow = NULL;
	SDL_Renderer* pRenderer = NULL;
	SDL_Joystick* pJoystick = NULL;
	SDL_Haptic* pHaptic = NULL;

	/* default mapping for testing */
	/* xbox one s */
	gamepad_map.left_bumper.type = AXIS;
	gamepad_map.right_bumper.type = AXIS;

	gamepad_map.left_bumper.wich = 2;
	gamepad_map.right_bumper.wich = 5;

	/* joycon combines */
	   /*gamepad_map.left_bumper.type = BUTTON;
	   gamepad_map.right_bumper.type = BUTTON;

	   gamepad_map.left_bumper.wich = 7;
	   gamepad_map.right_bumper.wich = 8; */


	/* unused for joycon */
	gamepad_map.left_bumper.true_value = -50;
	gamepad_map.right_bumper.true_value = -50;

	gamepad_map.left_bumper.reverse = NO_REVERSE;
	gamepad_map.right_bumper.reverse = NO_REVERSE;

	gamepad_map.joy_L.x_axis = 0;
	gamepad_map.joy_L.y_axis = 1;

	gamepad_map.joy_L.reverse_x = NO_REVERSE;
	gamepad_map.joy_L.reverse_y = REVERSE;

	/* xbox one s */
	gamepad_map.joy_R.x_axis = 3;
	gamepad_map.joy_R.y_axis = 4;
	

	gamepad_map.rumble = true;

	/* joycon combines 
	   gamepad_map.joy_R.x_axis = 2;
	   gamepad_map.joy_R.y_axis = 3; */


	gamepad_map.joy_R.reverse_x = NO_REVERSE;
	gamepad_map.joy_R.reverse_y = REVERSE;

	/* use the first gamepad by default */
	gamepad_number = 0;

	/* allow gamepad when app is in background */
	SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS,"1");

	/* init SDL for gamepad input and display */
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC) != 0 )
	{
		fprintf(stderr, "ERROR: cannot init SDL, (%s)\n",SDL_GetError());
		return -1;
	}

	/* create main window */
	pWindow = SDL_CreateWindow("gv-kbd v"VERSION,SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			WIN_WIDTH, WIN_HEIGHT,
			SDL_WINDOW_SHOWN);

	if(!pWindow)
	{
		fprintf(stderr, "ERROR: cannot open window, (%s)\n",
				SDL_GetError());
		return -1;
	}

    pRenderer = SDL_CreateRenderer(pWindow, -1,
			SDL_RENDERER_ACCELERATED);

	if (!pRenderer)
	{
		fprintf(stderr, "ERROR: cannot create renderer, (%s)\n",
				SDL_GetError());
		return -1;
	}

	/* use the event system for gamepad input */
	SDL_JoystickEventState(SDL_ENABLE);

	/* show to user all availables joysticks */
	listAvailablesJoy();

	/* open the current joystick */
	if (openJoy(&pJoystick, &pHaptic, &gamepad_map, gamepad_number) < 0)
		return -1;

	freeSelectionKeyboard(&gamepad_map, gamepad_number,
			pRenderer, pHaptic);

	/* exit properly the program */
	SDL_HapticClose(pHaptic);
	SDL_JoystickClose(pJoystick);
	SDL_DestroyRenderer(pRenderer);
	SDL_DestroyWindow(pWindow);
	SDL_Quit();
	return 0;
}
