#ifndef TYPES_H
#define TYPES_H

#include "../include/sokol/sokol_gfx.h"

#include "../include/sokol/sokol_gp.h"

#include <vector>

#include <map>

// --------------------------------------------------------------------------------------------------------------------

typedef unsigned long long id_t;

typedef enum shape_e
{
	triangle = 0,
	size
} shape_e;

// --------------------------------------------------------------------------------------------------------------------

typedef struct shape_t
{
	
	id_t id;
	
	shape_e type;
	
	std::vector<sgp_vec2> p;
	
	sgp_color color;

} shape_t;

// --------------------------------------------------------------------------------------------------------------------

typedef struct program_state_t
{

	bool is_mouse_moving;

	bool is_mouse_left_down;

	bool is_mouse_right_down;

	bool is_mouse_in_viewport;

	bool shape_lock;

	bool draw_selection;
	
	sgp_vec2 mouse_position;

	sgp_vec2 mouse_delta;

	sgp_rect selection;

	id_t id_counter;

	id_t selected;

	id_t last_selected;

	int point_lock = -1;
	
	int main_menu_bar_height;

	int side_bar_width = 200;

	int window_width;

	int window_height;

	int viewport_x;

	int viewport_y;

	int viewport_width;

	int viewport_height;

	float zoom;

	float window_ratio;

	float viewport_ratio;

	sgp_vec2 offset;

	sg_image main_image;

	sg_pipeline pipeline;

	std::map<id_t, shape_t> shape_list;

} program_state_t;

// --------------------------------------------------------------------------------------------------------------------

#endif
