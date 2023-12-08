#pragma once

#pragma clang diagnostic ignored "-Waddress-of-temporary"
#pragma clang diagnostic ignored "-Wc99-designator"
#pragma clang diagnostic ignored "-Wreorder-init-list"

// 10MB;
#define MAX_FILE_SIZE 1024 * 10240

#include "../inc/sokol/sokol_app.h"
#include "../inc/sokol/sokol_gfx.h"
#include "../inc/sokol/sokol_gp.h"
#include "../inc/sokol/util/sokol_imgui.h"

struct ImFont;

namespace mosaico_sim
{
	typedef unsigned long long int u64;
	typedef sgp_vec2               v2f;
	typedef sgp_rect               rectf;
	typedef sgp_color              color;

	struct span
	{
		u64  size;
		void *data;
	};

	enum ms_image_loadstate
	{
		LOADSTATE_UNKNOWN = 0,
		LOADSTATE_SUCCESS,
		LOADSTATE_FAILED,
		LOADSTATE_FILE_TOO_BIG,

		LOADSTATE_COUNT
	};

	struct ms_image
	{
		sg_image           texture;
		ms_image_loadstate loadstate;
		int                size;
		uint8_t            data[MAX_FILE_SIZE];
		bool               hide;
		v2f                image_x_viewport_ratio;
	};

	struct ms_workspace
	{
		rectf          window_rect;
		float          window_ratio;
		rectf          viewport_rect;
		float          viewport_ratio;
		ms_image       background_image;
		color          background_color;
		float          projection_base;
		float          scale;
		bool           hide;
	};

	struct ms_sidebar
	{
		uint32_t flags;
		rectf    rect;
		float    button_padding;
		bool     hide;
	};

	struct ms_menubar
	{
		float height;
		bool  hide;
	};

	struct ms_state
	{
		rectf          mainwindow;
		uint32_t       font_size;
		ImFont         *font_main;
		ImFont         *font_icon;

		struct
		{
			simgui_image_t texture;
			sg_pass        pass;
			sg_pass_action pass_action;
			int            resolution;
		} offscreen;

		struct
		{
			sg_pass_action pass_action;
		} onscreen;

		ms_menubar     menubar;
		ms_sidebar     sidebar;
		ms_workspace   workspace;
		void           (*workspace_frame_callback)(void);

		struct
		{
			float x_previous;
			float y_previous;
			float x;
			float y;
		} mouse;

		bool           debug_mode;
	};
}