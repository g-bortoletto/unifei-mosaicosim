#ifndef TYPES_H
#define TYPES_H

// -------------------------------------------------------------------------------------------------

#include "../include/sokol/sokol_gfx.h"

#include "../include/sokol/sokol_gp.h"

#include <vector>

#include <map>

#include <stdint.h>

typedef uint64_t u64;

typedef uint32_t u32;

// -------------------------------------------------------------------------------------------------

typedef int64_t i64;

typedef int32_t i32;

// -------------------------------------------------------------------------------------------------

typedef sgp_vec2 v2f;

typedef sgp_color v4f;

typedef sgp_rect rect;

typedef sgp_irect irect;

// -------------------------------------------------------------------------------------------------

typedef struct shape
{
	
	u64 id = 0;

	std::vector<v2f> p = {};

	v4f color = {};

	i32 locked = -1;

} shape;

// -------------------------------------------------------------------------------------------------

typedef struct mouse_state
{

	bool is_moving = false;
	
	bool is_left_button_down = false;
	
	bool is_right_button_down = false;
	
	bool is_middle_button_down = false;

	i32 scroll = 0;

	v2f position = { .x = 0.0f, .y = 0.0f };

	v2f delta = { .x = 0.0f, .y = 0.0f };;

} mouse_state;

// -------------------------------------------------------------------------------------------------

typedef struct window_state 
{
	
	i32 width;

	i32 height;

	float ratio = width / (float)height;

} window_state;

// -------------------------------------------------------------------------------------------------

typedef struct viewport_state
{
	
	int x, y, w, h;
	
	float ratio;

} viewport_state;

// -------------------------------------------------------------------------------------------------

typedef struct program_state
{

	mouse_state mouse = {};

	window_state window = {};

	viewport_state viewport = {};

	u64 id_counter = 0;

	u64 hot = 0;

	std::vector<u64> selected = {};

	std::vector<u64> last_selected = {};

	float scale = 1.0f;

	v2f translation = {};

	sg_image main_image = {};

	sg_pipeline pipeline = {};

	std::map<u64, shape> shape_list = {};

	u64 shape_list_size = 0;

} program_state;

// -------------------------------------------------------------------------------------------------

#endif