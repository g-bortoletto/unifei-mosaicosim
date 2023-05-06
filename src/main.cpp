// --------------------------------------------------------------------------------------------------------------------

#include "types.h"

#include "image.h"

#include "point.h"

#include "triangle.h"

#include "gui.h"

// --------------------------------------------------------------------------------------------------------------------

#define IMGUI_IMPLEMENTATION

#include "../include/imgui/imgui_single_file.h"

// --------------------------------------------------------------------------------------------------------------------

#define SOKOL_IMPL

#define SOKOL_GLCORE33

#include "../include/sokol/sokol_app.h"

#include "../include/sokol/sokol_gfx.h"

#include "../include/sokol/sokol_gp.h"

#include "../include/sokol/sokol_log.h"

#include "../include/sokol/sokol_glue.h"

#include "../include/fonts/segoeui.h"

#include "../include/sokol/sokol_imgui.h"

// --------------------------------------------------------------------------------------------------------------------

#include "../include/nfd/nfd_win.cpp"

// --------------------------------------------------------------------------------------------------------------------

#define STB_IMAGE_IMPLEMENTATION

#include "../include/stb/stb_image.h"

// --------------------------------------------------------------------------------------------------------------------

#include <string.h>

#include <vector>

using namespace std;

// --------------------------------------------------------------------------------------------------------------------

#define global static

// --------------------------------------------------------------------------------------------------------------------

global program_state_t program;

// --------------------------------------------------------------------------------------------------------------------

static void frame(void) 
{
	// update window size
	program.window_width = sapp_width();
	program.window_height = sapp_height();
	program.window_ratio = program.window_width / (float)program.window_height;

	// update viewport size
	program.viewport_x = program.side_bar_width;
	program.viewport_y = program.main_menu_bar_height;
	program.viewport_width = program.window_width - program.side_bar_width;
	program.viewport_height = program.window_height - program.main_menu_bar_height;
	program.viewport_ratio = program.viewport_width / (float)program.viewport_height;

	sgp_begin(program.window_width, program.window_height);
	simgui_new_frame({ program.window_width, program.window_height, sapp_frame_duration(), sapp_dpi_scale() });

	sgp_set_color(0.05f, 0.05f, 0.05f, 1.0f);
	sgp_clear();
	sgp_reset_color();

	if (program.is_mouse_right_down && program.is_mouse_moving)
	{
		program.offset = { program.offset.x + program.mouse_delta.x, program.offset.y + program.mouse_delta.y };
	}
	sgp_translate(program.offset.x, program.offset.y);
	program.mouse_delta = { 0 };

	// draw_main_menu_bar(&program);
	draw_side_bar(&program);
	draw_main_image(&program);
	draw_triangles(&program);
	if (program.draw_selection)
	{
		sgp_set_blend_mode(SGP_BLENDMODE_BLEND);
		sgp_set_color(100 / 255.0f, 149 / 255.0f, 237 / 255.0f, 0.6f);
		sgp_draw_filled_rect(
			program.selection.x * program.zoom, 
			program.selection.y * program.zoom, 
			program.selection.w * program.zoom, 
			program.selection.h * program.zoom);
		sgp_set_blend_mode(SGP_BLENDMODE_NONE);
	}
	// draw_selection(&program);
	
#ifdef DEBUG 
	draw_debug_window(&program); 
#endif

	// handle point movement
	if (program.is_mouse_left_down && program.shape_lock && program.point_lock >= 0)
	{
		point_set(&program.shape_list[program.selected], program.point_lock, program.mouse_position);
	}

	sg_pass_action pass_action = { };
	sg_begin_default_pass(&pass_action, program.window_width, program.window_height);
	sgp_reset_pipeline();
	sgp_flush();
	sgp_end();
	simgui_render();
	sg_end_pass();
	sg_commit();
}

// --------------------------------------------------------------------------------------------------------------------

static void left_mouse_down(const sapp_event *e)
{
	// press left mouse button
	if (e->type == SAPP_EVENTTYPE_MOUSE_DOWN && 
		e->mouse_button == SAPP_MOUSEBUTTON_LEFT)
	{
		program.is_mouse_left_down = true;
		if (program.is_mouse_in_viewport)
		{
			program.last_selected = program.selected;
			program.selected = 0;
			for (auto shape : program.shape_list)
			{
				if (point_is_inside_triangle(program.mouse_position, shape.second))
				{
					program.selected = shape.first;
				}
			}
			if (!program.selected && !program.draw_selection)
			{
				program.draw_selection = true;
				program.selection.x = (e->mouse_x - program.offset.x) / program.zoom;
				program.selection.y = (e->mouse_y - program.offset.y) / program.zoom;
			}
		}
	}

	program.selection.w = program.mouse_position.x - program.selection.x;
	program.selection.h = program.mouse_position.y - program.selection.y;
}

static void left_mouse_up(const sapp_event *e)
{
	// release left mouse button
	if (e->type == SAPP_EVENTTYPE_MOUSE_UP &&
		e->mouse_button == SAPP_MOUSEBUTTON_LEFT)
	{
		program.is_mouse_left_down = false;
		program.shape_lock = false;
		program.draw_selection = false;
		program.point_lock = -1;
	}
}

static void right_mouse_down(const sapp_event *e)
{
	if (e->type == SAPP_EVENTTYPE_MOUSE_DOWN &&
		e->mouse_button == SAPP_MOUSEBUTTON_RIGHT)
	{
		program.is_mouse_right_down = true;
	}
}

static void right_mouse_up(const sapp_event *e)
{
	if (e->type == SAPP_EVENTTYPE_MOUSE_UP &&
		e->mouse_button == SAPP_MOUSEBUTTON_RIGHT)
	{
		program.is_mouse_right_down = false;
	}
}

static void mouse_wheel(const sapp_event *e)
{
	if (e->type == SAPP_EVENTTYPE_MOUSE_SCROLL)
	{
		if (e->scroll_y > 0)
		{
			program.zoom += 0.1f;
		}
	
		if (e->scroll_y < 0)
		{
			program.zoom -= 0.1f;
		}
	}
}

static void input(const sapp_event* e) 
{
	simgui_handle_event(e);

	program.mouse_delta = { e->mouse_dx / program.zoom, e->mouse_dy / program.zoom };
	program.mouse_position = 
	{ 
		.x = (e->mouse_x - program.offset.x) / program.zoom, 
		.y = (e->mouse_y - program.offset.y) / program.zoom
	};
	program.is_mouse_in_viewport = 
		program.mouse_position.x > program.viewport_x && 
		program.mouse_position.y > program.viewport_y;
	program.is_mouse_moving = 
		program.mouse_delta.x != 0 ||
		program.mouse_delta.y != 0;

	if (!program.is_mouse_in_viewport) { program.is_mouse_left_down = false; }
	
	right_mouse_down(e);
	
	right_mouse_up(e);

	left_mouse_down(e);

	left_mouse_up(e);

	mouse_wheel(e);

	// handle shape movement
	if (program.is_mouse_in_viewport &&
		program.selected && 
		program.is_mouse_left_down)
	{
		if (program.last_selected == program.selected)
		{
			if (point_distance(
			program.shape_list[program.selected].p[0], 
			program.mouse_position) <= 10.0f)
			{
				program.shape_lock = true;
				if (program.point_lock < 0) 
				{ 
					program.point_lock = 0; 
				}
			}
			else if (point_distance(
			program.shape_list[program.selected].p[1], 
			program.mouse_position) <= 10.0f)
			{
				program.shape_lock = true;
				if (program.point_lock < 0) 
				{ 
					program.point_lock = 1; 
				}
			}
			else if (point_distance(
			program.shape_list[program.selected].p[2], 
			program.mouse_position) <= 10.0f)
			{
				program.shape_lock = true;
				if (program.point_lock < 0) 
				{ 
					program.point_lock = 2; 
				}
			}
			else
			{
				if (program.shape_lock) { return; }
				triangle_move(&program.shape_list[program.selected], program.mouse_delta);
			}
		}
		else
		{
			if (program.shape_lock) { return; }
			triangle_move(&program.shape_list[program.selected], program.mouse_delta);
		}
	}
}

// --------------------------------------------------------------------------------------------------------------------

static void init(void) 
{
	program.zoom = 1.0f;
	program.offset = { .x = 0.0f, .y = 0.0f };

	// setup sokol-gfx and sokol-time
	sg_desc desc = { };
	desc.context = sapp_sgcontext();
	desc.logger.func = slog_func;
	sg_setup(&desc);

	simgui_desc_t simgui_desc = { };
	simgui_desc.no_default_font = true;
	simgui_setup(&simgui_desc);

	auto& io = ImGui::GetIO();
	ImFontConfig fontCfg;
	fontCfg.FontDataOwnedByAtlas = false;
	fontCfg.OversampleH = 2;
	fontCfg.OversampleV = 2;
	fontCfg.RasterizerMultiply = 1.5f;
	io.Fonts->AddFontFromMemoryCompressedTTF(
		segoeui_compressed_data, 
		segoeui_compressed_size, 
		16.0f, 
		&fontCfg);

	unsigned char* font_pixels;
	int font_width, font_height;
	io.Fonts->GetTexDataAsRGBA32(&font_pixels, &font_width, &font_height);
	sg_image_desc img_desc = { };
	img_desc.width = font_width;
	img_desc.height = font_height;
	img_desc.pixel_format = SG_PIXELFORMAT_RGBA8;
	img_desc.wrap_u = SG_WRAP_CLAMP_TO_BORDER;
	img_desc.wrap_v = SG_WRAP_CLAMP_TO_BORDER;
	img_desc.min_filter = SG_FILTER_LINEAR;
	img_desc.mag_filter = SG_FILTER_LINEAR;
	img_desc.data.subimage[0][0].ptr = font_pixels;
	img_desc.data.subimage[0][0].size = font_width * font_height * 4;
	io.Fonts->TexID = (ImTextureID)(uintptr_t) sg_make_image(&img_desc).id;

	sgp_desc sgpdesc = { 0 };
	sgp_setup(&sgpdesc);
	if(!sgp_is_valid()) 
	{
		fprintf(stderr, "Failed to create Sokol GP context: %s\n", sgp_get_error_message(sgp_get_last_error()));
		exit(-1);
	}
}

// --------------------------------------------------------------------------------------------------------------------

static void cleanup(void) 
{
	sg_destroy_image(program.main_image);
	simgui_shutdown();
	sgp_shutdown();
	sg_shutdown();
}

// --------------------------------------------------------------------------------------------------------------------

sapp_desc sokol_main(int argc, char* argv[]) 
{
	(void)argc;
	(void)argv;
	sapp_desc desc = { };
	desc.init_cb = init;
	desc.frame_cb = frame;
	desc.cleanup_cb = cleanup;
	desc.event_cb = input;
	desc.sample_count = 4;
	desc.width = 1280;
	desc.height = 720;
	desc.high_dpi = true;
	desc.window_title = "tfg";
	desc.icon.sokol_default = true;
	desc.enable_clipboard = true;
	return desc;
}