#include "point.h"

#include "types.h"

#include <math.h>

void add_triangle(program_state_t *program)
{
	float hw = program->viewport_width  * 0.5f + program->viewport_x;
	float hh = program->viewport_height * 0.5f - program->viewport_y;
	
	shape_t shape =
	{
		.id = ++program->id_counter,
		.type = shape_e::triangle,
		.color = { .r = 1.0f, .g = 0.0f, .b = 1.0f, .a = 1.0f }
	};
	
	shape.p.push_back(
	{
		.x = hw + 0.0f, 
		.y = hh + 100.0f 
	});

	shape.p.push_back(
	{
		.x = hw + 100.0f, 
		.y = hh + 100.0f 
	});

	shape.p.push_back(
	{
		.x = hw + 50.0f, 
		.y = hh + 0.0f 
	});

	program->shape_list.insert( { shape.id, shape });
}

// --------------------------------------------------------------------------------------------------------------------

const static float sign(const float *p1, const float *p2, const float *p3)
{
    return (p1[0] - p3[0]) * (p2[1] - p3[1]) - (p2[0] - p3[0]) * (p1[1] - p3[1]);
}

// --------------------------------------------------------------------------------------------------------------------

const bool triangle_is_inside_rect(const shape_t *triangle, const sgp_rect rect)
{
	for (auto point : triangle->p)
	{
		if (point.x >= rect.x &&
			point.x <= rect.w &&
			point.y >= rect.y &&
			point.y <= rect.h)
		{
			return true;
		}
	}
	return false;
}

// --------------------------------------------------------------------------------------------------------------------

const bool point_is_inside_triangle(const sgp_vec2 pt, const shape_t triangle, const float point_tolerance)
{
    float d1, d2, d3;
    bool has_neg, has_pos;
    float p[]  = { pt.x, pt.y };
    float pa[] = { triangle.p[0].x, triangle.p[0].y };
    float pb[] = { triangle.p[1].x, triangle.p[1].y };
    float pc[] = { triangle.p[2].x, triangle.p[2].y };

    d1 = sign(p, pa, pb);
    d2 = sign(p, pb, pc);
    d3 = sign(p, pc, pa);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    bool is_inside = !(has_neg && has_pos);

    if (is_inside)
    { 
    	bool cp0 = point_distance(pt, triangle.p[0]) <= point_tolerance;
    	bool cp1 = point_distance(pt, triangle.p[1]) <= point_tolerance;
    	bool cp2 = point_distance(pt, triangle.p[2]) <= point_tolerance;
    	bool cp = cp0 || cp1 || cp2;
    	return is_inside || cp;
	}
	return false;
}

const void triangle_move(shape_t *tri, const sgp_vec2 new_position)
{
	if (!tri) { return; }

	point_move(tri, 0, new_position);
	point_move(tri, 1, new_position);
	point_move(tri, 2, new_position);
}

// --------------------------------------------------------------------------------------------------------------------

void draw_triangles(const program_state_t *program)
{
	if (!sgp_is_valid()) { return; }
	if (program->shape_list.empty()) { return; }
	for (auto tri : program->shape_list)
	{
		if (tri.second.type == shape_e::triangle)
		{
			sgp_push_transform();
			// draw filled triangle
			sgp_set_color(tri.second.color.r, tri.second.color.g, tri.second.color.b, tri.second.color.a);
			sgp_scale(program->zoom, program->zoom);
			sgp_draw_filled_triangle(
				tri.second.p[0].x, tri.second.p[0].y,
				tri.second.p[1].x, tri.second.p[1].y,
				tri.second.p[2].x, tri.second.p[2].y);
			
			// draw selection line
			sgp_set_color(1.0f, 1.0f, 1.0f, 1.0f);
			float ps = 5.0f;
			float hps = ps * 0.5f;
			if (program->selected == tri.second.id)
			{
				sgp_draw_line(tri.second.p[0].x, tri.second.p[0].y, tri.second.p[1].x, tri.second.p[1].y);
				sgp_draw_line(tri.second.p[0].x, tri.second.p[0].y, tri.second.p[2].x, tri.second.p[2].y);
				sgp_draw_line(tri.second.p[1].x, tri.second.p[1].y, tri.second.p[2].x, tri.second.p[2].y);
			}

			// draw points if mouse is close to it
			if (program->selected == tri.first && 
				point_is_inside_triangle(
				{
					.x = program->mouse_position.x,
					.y = program->mouse_position.y
				},
				tri.second, 
				30.0f / program->zoom))
			{
				sgp_set_color(1.0f, 1.0f, 1.0f, 1.0f);
				for (auto p : tri.second.p)
				{
					if (point_distance(p, program->mouse_position) <= 20.0f / program->zoom)
					{
						sgp_draw_filled_rect(
							p.x - 5.0f / program->zoom, 
							p.y - 5.0f / program->zoom, 
							10.0f / program->zoom, 
							10.0f / program->zoom);
					}
				}
			}
			sgp_pop_transform();
		}
	}
}

// --------------------------------------------------------------------------------------------------------------------