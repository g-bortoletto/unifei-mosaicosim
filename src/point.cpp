#include "point.h"

#include "types.h"

const float point_distance(const point_t p1, const point_t p2)
{
	float pr[] = { p2.x - p1.x, p2.y - p1.y };
	return sqrt(pr[0] * pr[0] + pr[1] * pr[1]);
}

const void point_move(shape_t *shape, const int index, const point_t amount)
{
	if (!shape) { return; }
	if (index < shape->p.size())
	{
		shape->p[index].x += amount.x;
		shape->p[index].y += amount.y;
	}
}