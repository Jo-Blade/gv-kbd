#include "2d-engine.h"

/* detect if a point is in a convex polygon:
 * 1 = true  //  0 = false
 * IMPORTANT: if the point is on an edge,
 * 				return true*/
int pointInConvexPoly (struct vec2 point, struct poly *poly){
	/* method: calculate determinant for each edge
	 * to know if the point is on the left or right side.
	 * if the point is on the same side of all edges then
	 * it is in the poly */

	long det;
	/* edge [AB] */
	struct vec2 vertex_A;
	struct vec2 vertex_B;
	char sign;

	/* last edge (last vertex to first vertex) */
	vertex_A = poly->vertex_array[poly->vertex_len -1];
	vertex_B = poly->vertex_array[0];

	det = (point.y - vertex_A.y)*(vertex_B.x - vertex_A.x)
		- (point.x - vertex_A.x)*(vertex_B.y - vertex_A.y);

	if( det == 0 )
		/* if the first det is null, we
		 * must look for the sign of the second det */
		sign = 0;
	else
	/* all det must have the same sign than the first det */
	sign = (det > 0) * 2 -1;

	for (int i = 1; i < poly->vertex_len; i++) {
		vertex_A = poly->vertex_array[i-1];
		vertex_B = poly->vertex_array[i];

		det = (point.y - vertex_A.y)*(vertex_B.x - vertex_A.x)
			- (point.x - vertex_A.x)*(vertex_B.y - vertex_A.y);

		if (sign == 0)
			/* if the first det was zero
			 * get the sign of all det */
			sign = (det > 0) * 2 -1;
		else
			/* if different sign */
			if ((det * sign) < 0)
				return 0;
	}
	return 1;
}
