#include "../inc/sokol/sokol_app.h"
#include "../inc/sokol/sokol_fetch.h"
#include "../inc/sokol/sokol_glue.h"
#include "../inc/sokol/sokol_log.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../inc/stblib/stb_image.h"

#include "tfg_types.h"

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
        if (ms.image.loadstate == LOADSTATE_SUCCESS)
        {
            if (sgp_is_valid())
            {
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
                    .buffer = SAPP_RANGE(ms.image.data),
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
        if (response->succeeded)
        {
            ms.image.loadstate = LOADSTATE_SUCCESS;
            ms.image.size = (int) response->data.size;

            stbi_uc *pixels = stbi_load_from_memory(
                ms.image.data,
                ms.image.size,
                &ms.image.width,
                &ms.image.height,
                &ms.image.channels,
                FULLCOLOR);
            if (pixels)
            {
                ms.image.sgi = sg_make_image(&(sg_image_desc)
                {
                    .width = ms.image.width,
                    .height = ms.image.height,
                    .data.subimage[0][0].ptr = pixels,
                    .data.subimage[0][0].size = (size_t)(ms.image.width * ms.image.height * 4),
                });
                stbi_image_free(pixels);
            }
            else
            {
                ms.image.loadstate = LOADSTATE_FAILED;
            }
        }
        else if (response->error_code == SAPP_HTML5_FETCH_ERROR_BUFFER_TOO_SMALL)
        {
            ms.image.loadstate = LOADSTATE_FILE_TOO_BIG;
        }
        else
        {
            ms.image.loadstate = LOADSTATE_FAILED;
        }
    }

}