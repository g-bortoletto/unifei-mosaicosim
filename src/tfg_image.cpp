#include "tfg_types.hpp"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#pragma clang diagnostic ignored "-Wall"
#include "../inc/sokol/sokol_app.h"
#include "../inc/sokol/sokol_fetch.h"
#include "../inc/sokol/sokol_glue.h"
#include "../inc/sokol/sokol_log.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../inc/stblib/stb_image.h"
#pragma clang diagnostic pop


#define FULLCOLOR 4
#define GRAYSCALE 2

namespace mosaico_sim
{
    extern mosaico_sim_state ms;

    void image_init(void);
    void image_frame(void);
    void image_input(const sapp_event *e);
    void image_cleanup(void);

    static void emsc_load_callback(const sapp_html5_fetch_response *response);

    void image_init(void)
    {
        #if !defined(__EMSCRIPTEN__)
        sfetch_setup(&(sfetch_desc_t)
        {
            .num_channels = 1,
            .num_lanes    = 1,
            .logger.func  = slog_func,
        });
        #endif
    }

    void image_frame(void)
    {
        mosaico_sim_interface_workspace *workspace = &ms.gui.workspace;

        if (workspace->image.loadstate == LOADSTATE_SUCCESS)
        {
            if (sgp_is_valid() && !workspace->image.hide)
            {
                sg_image_desc main_image_desc = sg_query_image_desc(workspace->image.sgi);
                sgp_set_image(0, workspace->image.sgi);
                sgp_push_transform();
                sgp_reset_color();
                sgp_draw_textured_rect(
                    0,
                    {
                        .x = 0.0f,
                        .y = 0.0f,
                        .w = (float)workspace->image.rect.w,
                        .h = (float)workspace->image.rect.h,
                    },
                    {
                        .x = 0.0f,
                        .y = 0.0f,
                        .w = (float)workspace->image.rect.w,
                        .h = (float)workspace->image.rect.h,
                    });
                sgp_pop_transform();
                sgp_reset_image(0);
            }
        }
    }

    void image_input(const sapp_event *e)
    {
        if (e->type == SAPP_EVENTTYPE_FILES_DROPPED)
        {
            #if defined(__EMSCRIPTEN__)
                sapp_html5_fetch_dropped_file(&(sapp_html5_fetch_request)
                {
                    .dropped_file_index = 0,
                    .callback = emsc_load_callback,
                    .buffer = SAPP_RANGE(ms.gui.workspace.image.data),
                });
            #endif
        }
    }

    void image_cleanup(void)
    {
        #if !defined(__EMSCRIPTEN__)
        sfetch_shutdown();
        #endif
    }

    static void emsc_load_callback(const sapp_html5_fetch_response *response)
    {
        mosaico_sim_interface_workspace *workspace = &ms.gui.workspace;

        if (response->succeeded)
        {
            workspace->image.loadstate = LOADSTATE_SUCCESS;
            workspace->image.size = (int) response->data.size;
            int width = 0;
            int height = 0;
            stbi_set_flip_vertically_on_load(1);
            stbi_uc *pixels = stbi_load_from_memory(
                workspace->image.data,
                workspace->image.size,
                &width,
                &height,
                &workspace->image.channels,
                FULLCOLOR);
            workspace->image.rect.w = (float)width;
            workspace->image.rect.h = (float)height;

            if (pixels)
            {
                workspace->image.sgi = sg_make_image(&(sg_image_desc)
                {
                    .width = (int)workspace->image.rect.w,
                    .height = (int)workspace->image.rect.h,
                    .data.subimage[0][0].ptr = pixels,
                    .data.subimage[0][0].size = (size_t)(workspace->image.rect.w * workspace->image.rect.h * 4),
                });
                stbi_image_free(pixels);
            }
            else
            {
                workspace->image.loadstate = LOADSTATE_FAILED;
            }
        }
        else if (response->error_code == SAPP_HTML5_FETCH_ERROR_BUFFER_TOO_SMALL)
        {
            workspace->image.loadstate = LOADSTATE_FILE_TOO_BIG;
        }
        else
        {
            workspace->image.loadstate = LOADSTATE_FAILED;
        }
    }

}