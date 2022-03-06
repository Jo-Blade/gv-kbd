#include "input.h"

void listAvailablesJoy (){
	SDL_Joystick* pJoystick = NULL;

	printf("Availables Joysticks\n");
	printf("======================\n\n");

	for (char i = 0; i < SDL_NumJoysticks(); i++) {

		printf("Name : %s\n",SDL_JoystickNameForIndex(i));

		/* open the selected gamepad */
		pJoystick = SDL_JoystickOpen(i);
		printf("Axis : %d\n",SDL_JoystickNumAxes(pJoystick));
		printf("Hats : %d\n",SDL_JoystickNumHats(pJoystick));
		printf("Trackballs : %d\n",SDL_JoystickNumBalls(pJoystick));
		printf("Buttons : %d\n\n",SDL_JoystickNumButtons(pJoystick));
	}
}

/* get the next gamepad event
 * return 1 if program ended */
int getEvent(struct joy_event *joy_event){
	SDL_Event event;

	if(SDL_WaitEvent(&event)){
		switch (event.type) {
			case SDL_QUIT:
				return 1;
				break;
			case SDL_JOYAXISMOTION:
				joy_event->gamepad_number = event.jaxis.which;
				joy_event->type = AXIS;
				joy_event->wich = event.jaxis.axis;
				joy_event->value = (event.jaxis.value >> 9 << 1) + 1;
				break;
			case SDL_JOYBUTTONDOWN:
			case SDL_JOYBUTTONUP:
				joy_event->gamepad_number = event.jbutton.which;
				joy_event->type = BUTTON;
				joy_event->wich = event.jbutton.button;

				if (event.jbutton.state == SDL_PRESSED)
					joy_event->value = KEY_PRESSED;
				else
					joy_event->value = KEY_RELEASED;
				break;
			case SDL_JOYHATMOTION:
				joy_event->gamepad_number = event.jhat.which;
				joy_event->type = HAT;
				joy_event->wich = event.jhat.hat;

				switch (event.jhat.value) {
					case SDL_HAT_CENTERED:
						joy_event->value = CENTER;
						break;

					case SDL_HAT_LEFT:
						joy_event->value = LEFT;
						break;
					case SDL_HAT_RIGHT:
						joy_event->value = RIGHT;
						break;
					case SDL_HAT_UP:
						joy_event->value = UP;
						break;
					case SDL_HAT_DOWN:
						joy_event->value = DOWN;
						break;

					case SDL_HAT_LEFTDOWN:
						joy_event->value = LEFT | DOWN;
						break;
					case SDL_HAT_LEFTUP:
						joy_event->value = LEFT | UP;
						break;
					case SDL_HAT_RIGHTDOWN:
						joy_event->value = RIGHT | DOWN;
						break;
					case SDL_HAT_RIGHTUP:
						joy_event->value = RIGHT | UP;
						break;
				}
				break;
		}
	}
	else return 1;

	return 0;
}

/* get the boolean value from a button/ hat/ axis
 * this function return 1 if the button is the same
 * that the event then the digital value is stored in value,
 * 0 if nothing was done */
int digitalRead (struct btn *button, struct joy_event *event, enum key_value *value){
	if ( (button->type == event->type) && (button->wich == event->wich) ){
		/* the button is the same that the event, so read the value */
		switch (event->type) {
			case BUTTON:
				*value = event->value;
				break;
			case HAT:
				if ((event->value & button->direction) != 0)
					*value = KEY_PRESSED;
				else
					*value = KEY_RELEASED;
				break;
			case AXIS:
				if ((event->value * button->reverse) > button->true_value)
					*value = KEY_PRESSED;
				else
					*value = KEY_RELEASED;
				break;

		}
		return 1;
	}

	return 0;
}

/* get the coordinates of a joystick from an event:
 * return 1 if it is the good joystick (and read value)
 * return 0 if nothing was done */
int joystickRead (struct joy *joystick,
		struct joy_event *event, struct vec2 *coordinates){

	if (event->type == AXIS) {// && (joystick->wich == event->wich) ){
		if (event->wich == joystick->x_axis){
			coordinates->x = event->value * joystick->reverse_x;
			return 1;
		}
		else if (event->wich == joystick->y_axis){
			coordinates->y = event->value * joystick->reverse_y;
			return 1;
		}
		else return 0;
	}
	return 0;
	}
