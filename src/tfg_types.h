#pragma once

#pragma clang diagnostic ignored "-Waddress-of-temporary"
#pragma clang diagnostic ignored "-Wc99-designator"
#pragma clang diagnostic ignored "-Wreorder-init-list"

// 10MB;
#define MAX_FILE_SIZE 1024 * 10240

#include "../inc/sokol/sokol_app.h"
#include "../inc/sokol/sokol_log.h"
#include "../inc/sokol/sokol_gfx.h"
#include "../inc/sokol/sokol_gp.h"
#include "../inc/sokol/sokol_glue.h"
#include "../inc/sokol/util/sokol_imgui.h"

struct ImFont;

namespace mosaico_sim
{
	typedef unsigned long long int u64;

	struct v2f
	{
		float x = 0.0f;
		float y = 0.0f;
	};

	struct v2i
	{
		int x = 0;
		int y = 0;
	};

	struct rectf
	{
		float x = 0.0f;
		float y = 0.0f;
		float w = 0.0f;
		float h = 0.0f;
	};

	struct recti
	{
		int x = 0;
		int y = 0;
		int w = 0;
		int h = 0;
	};

	struct color
	{
		float r = 0;
		float g = 0;
		float b = 0;
		float a = 0;
	};

	struct span
	{
		void *data;
		size_t len;
	};

	struct mosaico_sim_display
	{
		sg_pass_action pass_action;
	};

	struct mosaico_sim_workspace
	{
		simgui_image_t texture;
		sg_image image_color;
		sg_image image_depth;
		sg_pass pass;
		sg_sampler sampler;
	};

	struct mosaico_sim_interface_side_bar
	{
		bool show = true;
		int flags;
		rectf rect;
		float button_padding;
	};

	struct mosaico_sim_interface_menu_bar
	{
		bool show = true;
		float h;
	};

	struct mosaico_sim_interface
	{
		mosaico_sim_interface_side_bar side_bar;
		mosaico_sim_interface_menu_bar menu_bar;
	};

	typedef enum
	{
		LOADSTATE_UNKNOWN = 0,
		LOADSTATE_SUCCESS,
		LOADSTATE_FAILED,
		LOADSTATE_FILE_TOO_BIG,

		LOADSTATE_COUNT
	} mosaico_sim_image_loadstate;

	struct mosaico_sim_image
	{
		sg_image sgi;
		mosaico_sim_image_loadstate loadstate;
		int size;
		uint8_t data[MAX_FILE_SIZE];
		int width;
		int height;
		int channels;
	};

	struct mosaico_sim_state
	{
		rectf main_window;
		float frame_time;
		float dpi_scale;
		uint32_t font_size;
		ImFont *font_main;
		ImFont *font_icon;

		mosaico_sim_display display;
		mosaico_sim_workspace workspace;
		mosaico_sim_interface gui;
		mosaico_sim_image image;
	};

}