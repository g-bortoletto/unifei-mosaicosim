#ifndef TYPES_H
#define TYPES_H

#include "../include/sokol/sokol_gfx.h"

#include "../include/sokol/sokol_gp.h"

#include <vector>

#include <map>

// --------------------------------------------------------------------------------------------------------------------

typedef unsigned long long id_t;

// --------------------------------------------------------------------------------------------------------------------

typedef struct triangle_t
{

	id_t id;
	
	float a[2];
	
	float b[2];
	
	float c[2];
	
	float color[4];

} triangle_t;

// --------------------------------------------------------------------------------------------------------------------

typedef struct program_state_t
{

	bool is_mouse_left_down;

	id_t id_counter;

	int main_menu_bar_height;

	int side_bar_width = 200;

	int window_width;

	int window_height;

	float window_ratio;

	int viewport_x;

	int viewport_y;

	int viewport_width;

	int viewport_height;

	float viewport_ratio;

	sg_image main_image;

	sg_pipeline pipeline;

	id_t selected;

	std::map<id_t, triangle_t> tri_list;

} program_state_t;

// --------------------------------------------------------------------------------------------------------------------

#endif
