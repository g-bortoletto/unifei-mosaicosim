#pragma clang diagnostic ignored "-Waddress-of-temporary"
#pragma clang diagnostic ignored "-Wc99-designator"
#pragma clang diagnostic ignored "-Wreorder-init-list"

#define IMGUI_DEFINE_MATH_OPERATORS

#include "tfg_types.h"

#include "tfg_interface.cpp"

namespace mosaico_sim
{
	mosaico_sim_state ms =
	{
		.font_size = 16,
	};

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

		ms.main_window.w = 1280;
		ms.main_window.h = 720;
		ms.display.pass_action.colors[0] =
		{
			.load_action = SG_LOADACTION_CLEAR,
			.clear_value = { 0.33f, 0.33f, 0.33f, 1.0f },
		};

		interface_init();
	}

	void frame(void)
	{
		ms.main_window.w = sapp_width();
		ms.main_window.h = sapp_height();
		ms.frame_time = sapp_frame_duration();
		ms.dpi_scale = sapp_dpi_scale();

		sgp_begin(ms.main_window.w, ms.main_window.h);

		interface_frame();

		sg_begin_default_passf(
			&ms.display.pass_action,
			ms.main_window.w,
			ms.main_window.h);

		// applyresources here


		// sg_draw here

		interface_render();
		sgp_flush();
		sgp_end();
		sg_end_pass();
		sg_commit();
	}

	void input(const sapp_event *e)
	{
		interface_input(e);
	}

	void cleanup(void)
	{
		interface_cleanup();
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
