#include "types.h"

#include <math.h>

static void shape_add(program_state *program, const u32 num_points, const v2f *points)
{
	shape shape =
	{
		.id = ++program->id_counter,
		.color = { .r = 1.0f, .g = 0.0f, .b = 1.0f, .a = 1.0f }
	};
	
	for (int i = 0; i < num_points; ++i)
	{
		shape.p.push_back(points[i]);
	}

	program->shape_list.insert( { shape.id, shape });
	++program->shape_list_size;
}

// POINT --------------------------------------------------------------------------------------------------------------

const float point_distance(const v2f a, const v2f b)
{
	return sqrt((b.x - a.x) * (b.x - a.x) + 
		(b.y - a.y) * (b.y - a.y));
}

// --------------------------------------------------------------------------------------------------------------------

static void point_set(v2f *p, const v2f new_position)
{
	p->x = new_position.x;
	p->y = new_position.y;	
}

// --------------------------------------------------------------------------------------------------------------------

void point_move(v2f *p, const v2f offset)
{
	p->x += offset.x;
	p->y += offset.y;
}

// --------------------------------------------------------------------------------------------------------------------

static const v2f point_difference(const v2f a, const v2f b)
{
	return 
	{
		.x = b.x - a.x,
		.y = b.y - a.y
	};
}

// --------------------------------------------------------------------------------------------------------------------

static const v2f point_sum(const v2f a, const v2f b)
{
	return 
	{
		.x = a.x + b.x,
		.y = a.y + b.y
	};
}

// --------------------------------------------------------------------------------------------------------------------

void point_draw(const v2f p)
{
	if (sgp_is_valid())
	{
		sgp_push_transform();
		sgp_reset_color();
		sgp_draw_filled_rect(p.x - 2.5f, p.y - 2.5f, 5.0f, 5.0f);
		sgp_pop_transform();
	}
}

// TRIANGLE -----------------------------------------------------------------------------------------------------------

void triangle_add(program_state *program)
{
	v2f triangle[] =
	{
		{ .x = 0.0f,   .y = 100.0f },
		{ .x = 100.0f, .y = 100.0f },
		{ .x = 50.0f,  .y = 0.0f   }
	};
	for (int i = 0; i < 3; ++i)
	{
		point_move(
			&triangle[i], 
			{ .x = program->viewport.w * 0.5f, .y = program->viewport.h * 0.5f });
	}
	shape_add(program, 3, triangle);
}

const static float triangle_sign_util(const float *p1, const float *p2, const float *p3)
{
    return (p1[0] - p3[0]) * (p2[1] - p3[1]) - 
    	(p2[0] - p3[0]) * (p1[1] - p3[1]);
}

// --------------------------------------------------------------------------------------------------------------------

const bool triangle_has_point_inside(const sgp_vec2 pt, const shape triangle, const float point_tolerance)
{
    float d1, d2, d3;
    bool has_neg, has_pos;
    float p[]  = { pt.x, pt.y };
    float pa[] = { triangle.p[0].x, triangle.p[0].y };
    float pb[] = { triangle.p[1].x, triangle.p[1].y };
    float pc[] = { triangle.p[2].x, triangle.p[2].y };

    d1 = triangle_sign_util(p, pa, pb);
    d2 = triangle_sign_util(p, pb, pc);
    d3 = triangle_sign_util(p, pc, pa);

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

// --------------------------------------------------------------------------------------------------------------------

const void triangle_move(shape *tri, const v2f offset)
{
	if (!tri) { return; }
	if (tri->locked >= 0) { return; }

	point_move(&tri->p[0], offset);
	point_move(&tri->p[1], offset);
	point_move(&tri->p[2], offset);
}

// --------------------------------------------------------------------------------------------------------------------

const static void triangle_highlight_hot(const shape *tri)
{
	sgp_set_color(
		1.0f, 
		1.0f, 
		1.0f, 
		0.5f);
	sgp_set_blend_mode(SGP_BLENDMODE_BLEND);
	sgp_draw_filled_triangle(
	tri->p[0].x, tri->p[0].y,
	tri->p[1].x, tri->p[1].y,
	tri->p[2].x, tri->p[2].y);
	sgp_reset_blend_mode();
}

// --------------------------------------------------------------------------------------------------------------------

const static void triangle_highlight_selected(program_state *program)
{
	sgp_reset_color();
	sgp_set_blend_mode(SGP_BLENDMODE_BLEND);

	for (const u64 id : program->selected)
	{
		sgp_line l01 =
		{
			.a =
			{
				.x = program->shape_list[id].p[0].x,
				.y = program->shape_list[id].p[0].y
			},
			.b = 
			{
				.x = program->shape_list[id].p[1].x,
				.y = program->shape_list[id].p[1].y
			}
		};

		sgp_line l02 =
		{
			.a =
			{
				.x = program->shape_list[id].p[0].x,
				.y = program->shape_list[id].p[0].y
			},
			.b = 
			{
				.x = program->shape_list[id].p[2].x,
				.y = program->shape_list[id].p[2].y
			}
		};

		sgp_line l12 =
		{
			.a =
			{
				.x = program->shape_list[id].p[1].x,
				.y = program->shape_list[id].p[1].y
			},
			.b = 
			{
				.x = program->shape_list[id].p[2].x,
				.y = program->shape_list[id].p[2].y
			}
		};

		sgp_line selection_lines[] = { l01, l02, l12 };

		sgp_draw_lines(selection_lines, 3);
		sgp_reset_blend_mode();
	}
}

// --------------------------------------------------------------------------------------------------------------------

const void triangles_update(program_state *program)
{
	float point_distance_tolerance = 30.0f / program->scale;

	if (!program->selected.empty() &&
		program->mouse.is_left_button_down)
	{
		if (program->selected.size() == 1)
		{
			shape *selected = &program->shape_list[program->selected[0]];
			if(program->hot == program->selected[0])
			{
				for (int i = 0; i < 3; ++i)
				{
					if (selected->locked < 0 && 
						point_distance(
							program->mouse.position, 
							selected->p[i]) <= 
						point_distance_tolerance)
					{
						selected->locked = i;
					}
				}
			}

			if (selected->locked >= 0)
			{
				point_move(&selected->p[selected->locked], program->mouse.delta);
			}
		}

		for (auto id : program->selected)
		{
			triangle_move(&program->shape_list[id], program->mouse.delta);
		}
	}

	if (!program->selected.empty() &&
		!program->mouse.is_left_button_down)
	{
		for (auto s : program->selected)
		{
			program->shape_list[s].locked = -1;
		}
	}
}

// --------------------------------------------------------------------------------------------------------------------

void triangles_draw(program_state *program)
{
	if (!sgp_is_valid()) { return; }
	if (program->shape_list.empty()) { return; }

	float point_distance_tolerance = 30.0f / program->scale;

	for (auto tri : program->shape_list)
	{
		sgp_push_transform();
		
		sgp_set_color(
			tri.second.color.r, 
			tri.second.color.g, 
			tri.second.color.b, 
			tri.second.color.a);
		sgp_scale(program->scale, program->scale);
		sgp_draw_filled_triangle(
			tri.second.p[0].x, tri.second.p[0].y,
			tri.second.p[1].x, tri.second.p[1].y,
			tri.second.p[2].x, tri.second.p[2].y);

		if (program->hot == tri.first)
		{
			triangle_highlight_hot(&tri.second);

			for (auto p : tri.second.p)
			{
				if (point_distance(program->mouse.position, p) <= point_distance_tolerance)
				{
					point_draw(p);
				}
			}
		}

		if (!program->selected.empty())
		{
			triangle_highlight_selected(program);
		}
		
		sgp_pop_transform();

	}
}

// --------------------------------------------------------------------------------------------------------------------