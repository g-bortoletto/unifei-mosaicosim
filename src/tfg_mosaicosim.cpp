#pragma clang diagnostic ignored "-Waddress-of-temporary"
#pragma clang diagnostic ignored "-Wc99-designator"
#pragma clang diagnostic ignored "-Wreorder-init-list"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "../inc/imgui/imgui.h"

#include "../inc/sokol/sokol_app.h"
#include "../inc/sokol/sokol_log.h"
#include "../inc/sokol/sokol_gfx.h"
#include "../inc/sokol/sokol_gp.h"
#include "../inc/sokol/sokol_glue.h"

#include "tfg_types.h"

#include "tfg_interface.cpp"

namespace mosaico_sim
{
	struct _state
	{
		rectwhf main_window;
		float frame_time;
		float dpi_scale;

		struct
		{
			sg_pass_action pass_action;
		}
		display;
	}
	state;

	void init(void);
	void frame(void);
	void input(const sapp_event *e);
	void cleanup(void);

	void init(void)
	{
		sg_setup(&(sg_desc)
		{
			.context = sapp_sgcontext(),
			.logger.func = slog_func,
		});

		interface_init();

		state.display.pass_action.colors[0] = 
		{
			.load_action = SG_LOADACTION_CLEAR,
			.clear_value = { 0.33f, 0.33f, 0.33f, 1.0f },
		};
	}

	void frame(void)
	{
		state.main_window.w = sapp_width();
		state.main_window.h = sapp_height();
		state.frame_time = sapp_frame_duration();
		state.dpi_scale = sapp_dpi_scale();

		interface_frame();

		sg_begin_default_passf(
			&state.display.pass_action,
			state.main_window.w,
			state.main_window.h);

		// applyresources here

		// sg_draw here

		interface_render();
		sg_end_pass();
		sg_commit();
	}

	void input(const sapp_event *e)
	{
		simgui_handle_event(e);
	}

	void cleanup(void)
	{
		simgui_shutdown();
		sg_shutdown();
	}
}

#include "../inc/imgui/imgui.cpp"
#include "../inc/imgui/imgui_demo.cpp"
#include "../inc/imgui/imgui_draw.cpp"
#include "../inc/imgui/imgui_tables.cpp"
#include "../inc/imgui/imgui_widgets.cpp"

#define SOKOL_IMPL
#define SOKOL_GLES3
#include "../inc/sokol/sokol_app.h"
#include "../inc/sokol/sokol_log.h"
#include "../inc/sokol/sokol_gfx.h"
#include "../inc/sokol/sokol_gp.h"
#include "../inc/sokol/sokol_glue.h"
#include "../inc/sokol/util/sokol_imgui.h"
