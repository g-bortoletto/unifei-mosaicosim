#define IMGUI_DEFINE_MATH_OPERATORS

#include "tfg_mosaicosim.hpp"
#include "tfg_gui.cpp"
#include "tfg_workspace.cpp"

#include "../inc/sokol/sokol_glue.h"

#include <stdio.h>
#define LOG_INFO(msg) fprintf(stdout, "[info] %s", msg)

namespace mosaico_sim
{
	ms_state ms =
	{
		.font_size = 16,
		.debug_mode = false,
		.workspace_frame_callback = workspace_frame,
	};

	void init(void);
	void frame(void);
	void input(const sapp_event *e);
	void cleanup(void);

	void init(void)
	{
		LOG_INFO("setting up sokol gfx");
		sg_setup(&(sg_desc)
		{
			.context = sapp_sgcontext(),
			.logger.func = slog_func,
		});

		sgp_setup(&(sgp_desc) {});

		gui_init();
		workspace_init();
	}

	void frame(void)
	{
		ms.mainwindow.w = sapp_width();
		ms.mainwindow.h = sapp_height();

		sgp_begin(ms.mainwindow.w, ms.mainwindow.h);

		gui_frame();
		workspace_frame();

		sg_begin_default_passf(&(sg_pass_action)
		{
			.colors[0].load_action = SG_LOADACTION_CLEAR,
			.colors[0].clear_value = { 0.0f, 0.0f, 0.0f, 1.0f },
		},
		ms.mainwindow.w,
		ms.mainwindow.h);

		gui_render();
		sgp_flush();
		sgp_end();
		sg_end_pass();
		sg_commit();
	}

	void input(const sapp_event *e)
	{
		if (e->type == SAPP_EVENTTYPE_KEY_UP
			&& e->key_code == SAPP_KEYCODE_C
			&& e->modifiers == (0 | SAPP_MODIFIER_CTRL | SAPP_MODIFIER_SHIFT))
		{
			ms.debug_mode = !ms.debug_mode;
		}
		gui_input(e);
		workspace_input(e);
	}

	void cleanup(void)
	{
		workspace_cleanup();
		gui_cleanup();
		sgp_shutdown();
		sg_shutdown();
	}
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#pragma clang diagnostic ignored "-Wall"
#pragma clang diagnostic ignored "-Waddress-of-temporary"
#pragma clang diagnostic ignored "-Wc99-designator"
#pragma clang diagnostic ignored "-Wreorder-init-list"
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
#pragma clang diagnostic pop
