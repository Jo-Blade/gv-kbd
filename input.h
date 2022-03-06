#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>

#include "2d-engine.h"

/* reverse axis factor
 * 1 = normal
 * -1 = reverse */
enum reverse {
	NO_REVERSE = 1,
	REVERSE = -1
};

/* an analog joystick with two axis */
struct joy {
	/* number of the selected axis */
	char x_axis;
	char y_axis;

	enum reverse reverse_x;
	enum reverse reverse_y;
};

enum btn_type {
	BUTTON,
	AXIS,
	HAT
};

/* you can combine two directions with binary
 * operators */
enum hat_direction {
	CENTER = 0x00,
	UP = 0x01,
	DOWN = 0x02,
	LEFT = 0x04,
	RIGHT = 0x08
};

enum key_value {
	KEY_PRESSED = 1,
	KEY_RELEASED = 0
};

/* a digital input */
struct btn {
	enum btn_type type;
	/* number of the selected button/axis etc.. */
	char wich;
	/* if the input is an axis, define the frontier
	 * between true/false */
	char true_value;
	/*for an axis, reverse the input */
	enum reverse reverse;
	/* if the input is a hat, define the true
	 * value */
	enum hat_direction direction;
};

/* a gamepad input event */
struct joy_event {
	char gamepad_number;
	enum btn_type type;
	/* wich button/hat/axis is it */
	char wich;
	/* contain the value:
	 * [-127;127] = joystick
	 * UP/DOWN/.. = hat
	 * KEY_DOWN/UP = btn */
	char value;
};

void listAvailablesJoy ();

int getEvent(struct joy_event *joy_event);

int digitalRead (struct btn *button,
		struct joy_event *event, enum key_value *value);

int joystickRead (struct joy *joystick,
		struct joy_event *event, struct vec2 *coordinates);

#endif
