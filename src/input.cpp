#include "../include/sokol/sokol_app.h"
#include "shape.h"
#include "types.h"

#include <algorithm>

void update_mouse_position(const sapp_event *e, program_state *program)
{
	program->hot = 0;

	program->mouse.position = 
	{
		.x = ((e->mouse_x - program->translation.x - program->viewport.x) / program->scale),
		.y = (e->mouse_y - program->translation.y) / program->scale
	};

	program->mouse.delta = 
	{
		.x = e->mouse_dx / program->scale,
		.y = e->mouse_dy / program->scale
	};

	for (auto triangle : program->shape_list)
	{
		if (triangle_has_point_inside(program->mouse.position, triangle.second))
		{
			program->hot = triangle.first;
		}
	}
}

void update_left_mouse_button(const sapp_event *e, program_state *program)
{
	if (program->mouse.position.x < 0) { return; }
	if (e->type == SAPP_EVENTTYPE_MOUSE_DOWN && 
		e->mouse_button == SAPP_MOUSEBUTTON_LEFT)
	{
		program->mouse.is_left_button_down = true;
		program->last_selected = program->selected;

		if (program->hot)
		{
			if (std::find(
				program->selected.begin(),
				program->selected.end(),
				program->hot) == program->selected.end())
			{
				if (!(e->modifiers & SAPP_MODIFIER_CTRL)) { program->selected.clear(); }
				program->selected.push_back(program->hot);
			}
		}

		if (!program->hot)
		{
			program->selected.clear();
		}
	}

	if (e->type == SAPP_EVENTTYPE_MOUSE_UP && 
		e->mouse_button == SAPP_MOUSEBUTTON_LEFT)
	{	
		program->mouse.is_left_button_down = false;
	}
}

void update_right_mouse_button(const sapp_event *e, program_state *program)
{
	if (e->type == SAPP_EVENTTYPE_MOUSE_DOWN && 
		e->mouse_button == SAPP_MOUSEBUTTON_RIGHT)
	{
		program->mouse.is_right_button_down = true;
	}

	if (e->type == SAPP_EVENTTYPE_MOUSE_UP && 
		e->mouse_button == SAPP_MOUSEBUTTON_RIGHT)
	{	
		program->mouse.is_right_button_down = false;
	}
}

void update_middle_mouse_button(const sapp_event *e, program_state *program)
{
	if (e->type == SAPP_EVENTTYPE_MOUSE_DOWN && 
		e->mouse_button == SAPP_MOUSEBUTTON_MIDDLE)
	{
		program->mouse.is_middle_button_down = true;
	}

	if (e->type == SAPP_EVENTTYPE_MOUSE_UP && 
		e->mouse_button == SAPP_MOUSEBUTTON_MIDDLE)
	{	
		program->mouse.is_middle_button_down = false;
		program->translation = {};
		program->scale = 1.0f;
	}

	if (e->type == SAPP_EVENTTYPE_MOUSE_SCROLL)
	{
		program->mouse.scroll = e->scroll_y;
	}
}

