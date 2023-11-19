#include "tfg_mosaicosim.cpp"

static void Init(void)
{
	MosaicoSim::Init();
}

static void Frame(void)
{
	MosaicoSim::Frame();
}

static void Input(const sapp_event *e)
{
	MosaicoSim::Input(e);
}

static void Cleanup(void)
{
	MosaicoSim::Cleanup();
}

sapp_desc sokol_main(int argc, char* argv[]) {
    (void)argc; (void)argv;
    return (sapp_desc){
        .init_cb = Init,
        .frame_cb = Frame,
        .cleanup_cb = Cleanup,
		.event_cb = Input,
        .window_title = "MosaicoSIM",
        .logger.func = slog_func,
    };
}