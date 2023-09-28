#include <sokol/sokol_app.h>

#include "tfgProgram.h"

static Program program;

static void Init()
{
	program.Init();
}

static void Frame()
{
	program.Frame();
}

static void Cleanup()
{
	program.Cleanup();
}

static void Input(const sapp_event *e)
{
	program.Input(e);
}

sapp_desc sokol_main(int argc, char **argv)
{
	sapp_desc desc = sapp_desc
	{
		.init_cb = Init,
		.frame_cb = Frame,
		.cleanup_cb = Cleanup,
		.event_cb = Input,
		.width = 1280,
		.height = 720,
		.sample_count = 8,
		.high_dpi = true,
		.window_title = "MosaicoSim",
		.enable_clipboard = true,
		.icon.sokol_default = true,
	};
	return desc;
}