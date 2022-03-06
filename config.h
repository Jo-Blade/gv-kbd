#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

#include "input.h"

struct mapping {
	struct btn btn_east;
	struct btn btn_west;
	struct btn btn_north;
	struct btn btn_left;

	struct btn left_bumper;
	struct btn right_bumper;

	struct joy joy_L;
	struct joy joy_R;

	bool rumble;
};

#endif
