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

        sgp_begin(ms.workspace.viewport_rect.w, ms.workspace.viewport_rect.h);

		// draw
        image_frame();

        float ratio = (float)ms.workspace.window_rect.w / (float)ms.workspace.window_rect.h;
        sgp_project(
            ms.workspace.viewport_rect.x,
            ratio * 1000.0f,
            1000.0f,
            ms.workspace.viewport_rect.y);
        sgp_set_color(0.0f, 1.0f, 0.0f, 1.0f);
        sgp_draw_filled_rect(10.0f, 10.0f, 100.0f, 100.0f);

		sg_begin_pass(ms.offscreen.pass, ms.offscreen.pass_action);

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