#pragma once

#include "../inc/sokol/sokol_app.h"
#include "../inc/sokol/sokol_log.h"
#include "../inc/sokol/sokol_gfx.h"
#include "../inc/sokol/sokol_gp.h"
#include "../inc/sokol/sokol_glue.h"

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

	struct rectwhf
	{
		float w = 0.0f;
		float h = 0.0f;
	};

	struct rectwhi
	{
		int w = 0;
		int h = 0;
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

	struct mosaico_sim_state
	{
		rectwhf main_window;
		float frame_time;
		float dpi_scale;
		uint32_t font_size;
		ImFont *font_main;
		ImFont *font_icon;

		struct
		{
			sg_pass_action pass_action;
		}
		display;

		struct
		{
			struct
			{
				bool show = true;
				int flags;
				rectf rect;
				float button_padding;
			} 
			control_bar;

			struct
			{
				bool show = true;
				float h;
			}
			menu_bar;
		}
		interface;
	};

}