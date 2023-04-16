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

typedef struct point_t
{
	
	float x;
	
	float y;

} point_t;

// --------------------------------------------------------------------------------------------------------------------

typedef struct color_t
{

	float r;
	
	float g;
	
	float b;
	
	float a;

} color_t;

// --------------------------------------------------------------------------------------------------------------------

typedef struct shape_t
{
	
	id_t id;
	
	shape_e type;
	
	std::vector<point_t> p;
	
	color_t color;

} shape_t;

// --------------------------------------------------------------------------------------------------------------------

typedef struct program_state_t
{

	bool is_mouse_left_down;

	bool is_mouse_in_viewport;

	point_t mouse_position;

	point_t mouse_delta;

	id_t id_counter;

	id_t selected;

	id_t last_selected;
	
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

	std::map<id_t, shape_t> shape_list;

} program_state_t;

// --------------------------------------------------------------------------------------------------------------------

#endif
