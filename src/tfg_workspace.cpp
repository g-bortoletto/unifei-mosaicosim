#include "tfg_mosaicosim.hpp"
#include "tfg_image.cpp"

namespace mosaico_sim
{
    extern ms_state ms;

    void workspace_init(void);
    void workspace_frame(void);
    void workspace_input(const sapp_event *e);
    void workspace_cleanup(void);

    void workspace_init(void)
    {
        image_init();
    }

    void workspace_frame(void)
    {
        if (ms.workspace.hide) { return; }
        if (!sgp_is_valid()) { return; }

        sgp_begin(ms.workspace.window_rect.w, ms.workspace.window_rect.h);

        // clear screen
        sgp_set_color(
            ms.workspace.background_color.r,
            ms.workspace.background_color.g,
            ms.workspace.background_color.b,
            ms.workspace.background_color.a);
        sgp_clear();
        sgp_reset_color();

		// draw
        image_frame();

		sg_begin_pass(ms.offscreen_pass, &(sg_pass_action)
		{
            .colors[0].load_action = SG_LOADACTION_CLEAR,
        });
		sgp_flush();
		sgp_end();
		sg_end_pass();
    }

    void workspace_input(const sapp_event *e)
    {
        image_input(e);
    }

    void workspace_cleanup(void)
    {
        image_cleanup();
    }

}