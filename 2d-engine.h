#ifndef TWODENGINE_H
#define TWODENGINE_H

struct vec2 {
	char x;
	char y;
};

struct poly {
	char vertex_len;
	struct vec2 *vertex_array;
};

int pointInConvexPoly (struct vec2 point, struct poly *poly);

#endif
