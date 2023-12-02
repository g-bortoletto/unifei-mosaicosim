#include "tfg_mosaicosim.cpp"

static void init(void)
{
	mosaico_sim::init();
}

static void frame(void)
{
	mosaico_sim::frame();
}

static void input(const sapp_event *e)
{
	mosaico_sim::input(e);
}

static void cleanup(void)
{
	mosaico_sim::cleanup();
}

sapp_desc sokol_main(int argc, char* argv[])
{
    (void)argc; (void)argv;
    return (sapp_desc){
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
		.event_cb = input,
		.high_dpi = true,
		.fullscreen = true,
        .window_title = "MosaicoSIM",
		.enable_clipboard = true,
        .enable_dragndrop = true,
        .logger.func = slog_func,
    };
}