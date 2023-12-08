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
        ms.workspace.projection_base = 100.0f;
        ms.workspace.scale = 1.0f;
        image_init();
    }

    void workspace_frame(void)
    {
        if (ms.workspace.hide) { return; }
        if (!sgp_is_valid()) { return; }

        sgp_begin(ms.workspace.viewport_rect.w, ms.workspace.viewport_rect.h);
        sgp_set_color(
            ms.workspace.background_color.r,
            ms.workspace.background_color.g,
            ms.workspace.background_color.b,
            ms.workspace.background_color.a);
        sgp_clear();
        sgp_reset_color();

        sgp_project(
            ms.workspace.viewport_rect.x,
            ms.workspace.viewport_ratio * ms.workspace.projection_base,
            ms.workspace.projection_base,
            ms.workspace.viewport_rect.y);

        image_frame();

        // sgp_push_transform();
        // sgp_set_color(0.0f, 1.0f, 0.0f, 1.0f);
        // sgp_draw_filled_rect(10.0f, 10.0f, 10.0f, 10.0f);
        // sgp_pop_transform();

		sg_begin_pass(ms.offscreen.pass, ms.offscreen.pass_action);

		sgp_flush();
		sgp_end();
		sg_end_pass();
    }

    void workspace_input(const sapp_event *e)
    {
        image_input(e);
        if (e->type == SAPP_EVENTTYPE_MOUSE_SCROLL
            && e->modifiers == SAPP_MODIFIER_CTRL)
        {
            if (e->scroll_y > 0)
            {
                ms.workspace.scale *= 1.1f;
            }

            if (e->scroll_y < 0)
            {
                ms.workspace.scale /= 1.1f;
            }
        }
    }

    void workspace_cleanup(void)
    {
        image_cleanup();
    }

}