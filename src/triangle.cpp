#include "types.h"
#include <math.h>

void add_triangle(program_state_t *program)
{
	float hw = program->viewport_width  * 0.5f + program->viewport_x;
	float hh = program->viewport_height * 0.5f - program->viewport_y;
	triangle_t tri = { 0 };
	tri.id = ++program->id_counter;
	tri.a[0] = hw + 0.0f;   tri.a[1] = hh + 100.0f;
	tri.b[0] = hw + 100.0f; tri.b[1] = hh + 100.0f;
	tri.c[0] = hw + 50.0f;  tri.c[1] = hh + 0.0f;
	tri.color[0] = 1.0f;
	tri.color[2] = 1.0f;
	tri.color[3] = 1.0f;
	program->tri_list.insert({ tri.id, tri });
	program->point_list.insert({ ++program->id_counter, tri.a });
	program->point_list.insert({ ++program->id_counter, tri.b });
	program->point_list.insert({ ++program->id_counter, tri.c });
}

// --------------------------------------------------------------------------------------------------------------------

long long get_triangle(program_state_t *program)
{
	for (id_t i = 0; i < program->tri_list.size(); ++i) { if (program->selected == program->tri_list[i].id) { return i; } }
	return -1;
}

// --------------------------------------------------------------------------------------------------------------------

const static float sign(const float *p1, const float *p2, const float *p3)
{
    return (p1[0] - p3[0]) * (p2[1] - p3[1]) - (p2[0] - p3[0]) * (p1[1] - p3[1]);
}

// --------------------------------------------------------------------------------------------------------------------

const bool point_is_inside_triangle(const float *pt, const triangle_t triangle)
{
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = sign(pt, triangle.a, triangle.b);
    d2 = sign(pt, triangle.b, triangle.c);
    d3 = sign(pt, triangle.c, triangle.a);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

// --------------------------------------------------------------------------------------------------------------------

void draw_triangles(const program_state_t *program)
{
	if (program->tri_list.empty()) { return; }
	for (auto tri : program->tri_list)
	{
		sgp_set_color(tri.second.color[0], tri.second.color[1], tri.second.color[2], tri.second.color[3]);
		sgp_push_transform();
		sgp_draw_filled_triangle(
			tri.second.a[0], tri.second.a[1],
			tri.second.b[0], tri.second.b[1],
			tri.second.c[0], tri.second.c[1]);
		sgp_pop_transform();
		
		sgp_set_color(37 / 255.0f, 150 / 255.0f, 190 / 255.0f, 1.0f);
		float ps = 5.0f;
		float hps = ps * 0.5f;
		if (program->selected == tri.second.id)
		{
			sgp_push_transform();
			sgp_draw_line(tri.second.a[0], tri.second.a[1], tri.second.b[0], tri.second.b[1]);
			sgp_draw_line(tri.second.a[0], tri.second.a[1], tri.second.c[0], tri.second.c[1]);
			sgp_draw_line(tri.second.b[0], tri.second.b[1], tri.second.c[0], tri.second.c[1]);
			sgp_draw_filled_rect(
				tri.second.a[0] - hps, tri.second.a[1] + hps,
				ps, -ps);
			sgp_draw_filled_rect(
				tri.second.b[0] - hps, tri.second.b[1] + hps,
				ps, -ps);
			sgp_draw_filled_rect(
				tri.second.c[0] - hps, tri.second.c[1] + hps,
				ps, -ps);
			sgp_draw_point(tri.second.c[0], tri.second.c[1]);
			sgp_pop_transform();
		}
	}
}

// --------------------------------------------------------------------------------------------------------------------