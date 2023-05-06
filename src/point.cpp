#include "point.h"

#include "types.h"

const float point_distance(const sgp_vec2 p1, const sgp_vec2 p2)
{
	float pr[] = { p2.x - p1.x, p2.y - p1.y };
	return sqrt(pr[0] * pr[0] + pr[1] * pr[1]);
}

const sgp_vec2 point_difference(const sgp_vec2 p1, const sgp_vec2 p2)
{
	return { p2.x - p1.x, p2.y - p1.y };
}

const sgp_vec2 point_sum(const sgp_vec2 p1, const sgp_vec2 p2)
{
	return { p2.x + p1.x, p2.y + p1.y };
}

const void point_move(shape_t *shape, const int index, const sgp_vec2 amount)
{
	if (!shape) { return; }
	if (index < shape->p.size())
	{
		shape->p[index].x += amount.x;
		shape->p[index].y += amount.y;
	}
}

const void point_set(shape_t *shape, const int index, const sgp_vec2 new_position)
{
	if (!shape) { return; }
	if (index < shape->p.size())
	{
		shape->p[index].x = new_position.x;
		shape->p[index].y = new_position.y;
	}
}

const void point_draw(const sgp_vec2 point, const float scale)
{
	if (sgp_is_valid())
	{
		sgp_draw_filled_rect(
			point.x - 5.0f / scale, 
			point.y - 5.0f / scale, 
			10.0f / scale, 
			10.0f / scale);
	}
}
