#include "tfg_mosaicosim.hpp"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#pragma clang diagnostic ignored "-Wall"
#define STB_IMAGE_IMPLEMENTATION
#include "../inc/stblib/stb_image.h"
#pragma clang diagnostic pop


#define FULLCOLOR 4
#define GRAYSCALE 2

namespace mosaico_sim
{
    extern ms_state ms;

    void image_init(void);
    void image_frame(void);
    void image_input(const sapp_event *e);
    void image_cleanup(void);

    static void emsc_load_callback(const sapp_html5_fetch_response *response);

    void image_init(void)
    {
    }

    void image_frame(void)
    {
        if (ms.workspace.background_image.loadstate == LOADSTATE_SUCCESS)
        {
            if (sgp_is_valid() && !ms.workspace.background_image.hide)
            {
                sg_image_desc background_image = sg_query_image_desc(ms.workspace.background_image.texture);
                ms.workspace.background_image.image_x_viewport_ratio =
                {
                    .x = (float)background_image.width / (float)ms.workspace.viewport_rect.w,
                    .y = (float)background_image.height / (float)ms.workspace.viewport_rect.h,
                };
                sgp_set_image(0, ms.workspace.background_image.texture);
                sgp_push_transform();
                sgp_reset_color();
                sgp_draw_filled_rect(
                    0.0f,
                    0.0f,
                    (float)background_image.width / (float)background_image.height * ms.workspace.projection_base,
                    ms.workspace.projection_base);
                sgp_pop_transform();
                sgp_reset_image(0);
            }
        }
    }

    void image_input(const sapp_event *e)
    {
        if (e->type == SAPP_EVENTTYPE_FILES_DROPPED)
        {
            sapp_html5_fetch_dropped_file(&(sapp_html5_fetch_request)
            {
                .dropped_file_index = 0,
                .callback = emsc_load_callback,
                .buffer = SAPP_RANGE(ms.workspace.background_image.data),
            });
        }
    }

    void image_cleanup(void)
    {
    }

    static void emsc_load_callback(const sapp_html5_fetch_response *response)
    {
        if (response->succeeded)
        {
            int channels = 0;
            int width = 0;
            int height = 0;
            ms.workspace.background_image.loadstate = LOADSTATE_SUCCESS;
            ms.workspace.background_image.size = (int) response->data.size;
            // stbi_set_flip_vertically_on_load(1);
            stbi_uc *pixels = stbi_load_from_memory(
                ms.workspace.background_image.data,
                ms.workspace.background_image.size,
                &width,
                &height,
                &channels,
                FULLCOLOR);

            if (pixels)
            {
                ms.workspace.background_image.texture = sg_make_image(&(sg_image_desc)
                {
                    .width = width,
                    .height = height,
                    .data.subimage[0][0].ptr = pixels,
                    .data.subimage[0][0].size = (size_t)(width * height * 4),
                });
                stbi_image_free(pixels);
            }
            else
            {
                ms.workspace.background_image.loadstate = LOADSTATE_FAILED;
            }
        }
        else if (response->error_code == SAPP_HTML5_FETCH_ERROR_BUFFER_TOO_SMALL)
        {
            ms.workspace.background_image.loadstate = LOADSTATE_FILE_TOO_BIG;
        }
        else
        {
            ms.workspace.background_image.loadstate = LOADSTATE_FAILED;
        }
    }

}