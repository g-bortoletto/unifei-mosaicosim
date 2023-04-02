#include <string.h>
#define IMGUI_IMPLEMENTATION
#define IMGUI_DEMO
#include "../include/imgui/imgui_single_file.h"

#define SOKOL_IMPL
#define SOKOL_GLCORE33
#include "../include/sokol/sokol_app.h"
#include "../include/sokol/sokol_gfx.h"
#include "../include/sokol/sokol_gp.h"
#include "../include/sokol/sokol_log.h"
#include "../include/sokol/sokol_glue.h"
#include "../include/fonts/segoeui.h"
#include "../include/sokol/sokol_imgui.h"

#include "../include/nfd/nfd_win.cpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb/stb_image.h"

#define global static

global int main_menu_bar_height;
global int side_bar_width = 200;
global int window_width;
global int window_height;
global float window_ratio;
global int viewport_x;
global int viewport_y;
global int viewport_width;
global int viewport_height;
global float viewport_ratio;
global sg_image main_image;
global sg_pipeline pipeline;

static sg_image load_image(const char *filename)
{
    int width, height, channels;
    sg_image img = { SG_INVALID_ID };
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 4);
    sg_image_desc image_desc = 
    {
        .width = width,
        .height = height,
        .min_filter = SG_FILTER_LINEAR,
        .mag_filter = SG_FILTER_LINEAR,
        .wrap_u = SG_WRAP_CLAMP_TO_EDGE,
        .wrap_v = SG_WRAP_CLAMP_TO_EDGE,
    };
    image_desc.data.subimage[0][0].ptr = data;
    image_desc.data.subimage[0][0].size = (size_t)(width * height * 4);
    img = sg_make_image(&image_desc);
    stbi_image_free(data);    
    return img;
}

static void draw_main_menu_bar(void)
{
	if (ImGui::BeginMainMenuBar())
    {
    	if (ImGui::BeginMenu("Arquivo"))
    	{
    		if (ImGui::MenuItem("Abrir imagem...", "CTRL+O")) 
    		{
    			nfdchar_t *file_path = 0;
    			nfdresult_t result = NFD_OpenDialog("png;jpg;jpeg", 0, &file_path);
    			if (result == NFD_OKAY)
    			{
                    main_image = load_image(file_path);
    			}
    		}
    		if (ImGui::MenuItem("Sair", "ALT+F4")) { sapp_request_quit(); }
    		ImGui::EndMenu();
    	}
    	if (ImGui::BeginMenu("Editar"))
        {
            if (ImGui::MenuItem("Desfazer", "CTRL+Z")) {}
            if (ImGui::MenuItem("Refazer", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Recortar", "CTRL+X")) {}
            if (ImGui::MenuItem("Copiar", "CTRL+C")) {}
            if (ImGui::MenuItem("Colar", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        main_menu_bar_height = ImGui::GetWindowSize().y;
    	ImGui::EndMainMenuBar();
    }
}

static void draw_side_bar(void)
{
	bool sidebar_open = true;
    ImGui::SetNextWindowPos(ImVec2(0, main_menu_bar_height));
    ImGui::SetNextWindowSize(ImVec2(side_bar_width, sapp_height() - main_menu_bar_height));
    ImGui::Begin("sidebar", &sidebar_open,
    	ImGuiWindowFlags_NoMove |
    	ImGuiWindowFlags_NoCollapse |
    	ImGuiWindowFlags_NoResize 
|    	ImGuiWindowFlags_NoTitleBar);
    ImGui::End();
}

static void draw_main_image(void)
{
    if (main_image.id != SG_INVALID_ID)
    {
        sg_image_desc main_image_desc = sg_query_image_desc(main_image);
        const float image_ratio = main_image_desc.width / (float)main_image_desc.height;
        sgp_set_image(0, main_image);
        float main_image_width = viewport_ratio <= image_ratio ? viewport_width : (viewport_height * (float)image_ratio);
        float main_image_height = viewport_ratio > image_ratio ? viewport_height : (viewport_width / (float)image_ratio);
        sgp_draw_textured_rect(
            viewport_x, 
            viewport_y, 
            main_image_width, 
            main_image_height);
        if (ImGui::Begin("ratio debug"))
        {
            ImGui::Text("image width:     %f", main_image_width);
            ImGui::Text("image height:    %f", main_image_height);
            ImGui::Text("image ratio:     %f", image_ratio);
            ImGui::Text("viewport width:  %d", viewport_width);
            ImGui::Text("viewport height: %d", viewport_height);
            ImGui::Text("viewport ratio:  %f", viewport_ratio);
            ImGui::End();
        }
    }
}

static void init(void) 
{
    // setup sokol-gfx and sokol-time
    sg_desc desc = { };
    desc.context = sapp_sgcontext();
    desc.logger.func = slog_func;
    sg_setup(&desc);

    simgui_desc_t simgui_desc = { };
    simgui_desc.no_default_font = true;
    simgui_setup(&simgui_desc);

    auto& io = ImGui::GetIO();
    ImFontConfig fontCfg;
    fontCfg.FontDataOwnedByAtlas = false;
    fontCfg.OversampleH = 2;
    fontCfg.OversampleV = 2;
    fontCfg.RasterizerMultiply = 1.5f;
    io.Fonts->AddFontFromMemoryCompressedTTF(
    	segoeui_compressed_data, 
    	segoeui_compressed_size, 
    	16.0f, 
    	&fontCfg);

    unsigned char* font_pixels;
    int font_width, font_height;
    io.Fonts->GetTexDataAsRGBA32(&font_pixels, &font_width, &font_height);
    sg_image_desc img_desc = { };
    img_desc.width = font_width;
    img_desc.height = font_height;
    img_desc.pixel_format = SG_PIXELFORMAT_RGBA8;
    img_desc.wrap_u = SG_WRAP_CLAMP_TO_BORDER;
    img_desc.wrap_v = SG_WRAP_CLAMP_TO_BORDER;
    img_desc.min_filter = SG_FILTER_LINEAR;
    img_desc.mag_filter = SG_FILTER_LINEAR;
    img_desc.data.subimage[0][0].ptr = font_pixels;
    img_desc.data.subimage[0][0].size = font_width * font_height * 4;
    io.Fonts->TexID = (ImTextureID)(uintptr_t) sg_make_image(&img_desc).id;

    sgp_desc sgpdesc = { 0 };
    sgp_setup(&sgpdesc);
    if(!sgp_is_valid()) 
    {
        fprintf(stderr, "Failed to create Sokol GP context: %s\n", sgp_get_error_message(sgp_get_last_error()));
        exit(-1);
    }
}

static void frame(void) 
{
    // update window size
    window_width = sapp_width();
    window_height = sapp_height();
    window_ratio = window_width / (float)window_height;

    // update viewport size
    viewport_x = side_bar_width;
    viewport_y = main_menu_bar_height;
    viewport_width = window_width - side_bar_width;
    viewport_height = window_height - main_menu_bar_height;
    viewport_ratio = viewport_width / (float)viewport_height;

    sgp_begin(window_width, window_height);
    simgui_new_frame({ window_width, window_height, sapp_frame_duration(), sapp_dpi_scale() });

    sgp_set_color(0.05f, 0.05f, 0.05f, 1.0f);
    sgp_clear();
    sgp_reset_color();

    draw_main_menu_bar();
    draw_side_bar();
    draw_main_image();

	sg_pass_action pass_action = { };
    sg_begin_default_pass(&pass_action, window_width, window_height);
    sgp_flush();
    sgp_end();
    simgui_render();
    sg_end_pass();
    sg_commit();
}

static void cleanup(void) 
{
    sg_destroy_image(main_image);
    simgui_shutdown();
    sgp_shutdown();
    sg_shutdown();
}

static void input(const sapp_event* event) 
{
    simgui_handle_event(event);
}

sapp_desc sokol_main(int argc, char* argv[]) 
{
    (void)argc;
    (void)argv;
    sapp_desc desc = { };
    desc.init_cb = init;
    desc.frame_cb = frame;
    desc.cleanup_cb = cleanup;
    desc.event_cb = input;
    desc.width = 1280;
    desc.height = 720;
    desc.high_dpi = true;
    desc.ios_keyboard_resizes_canvas = false;
    desc.gl_force_gles2 = true;
    desc.window_title = "tfg";
    desc.icon.sokol_default = true;
    desc.enable_clipboard = true;
    desc.logger.func = slog_func;
    return desc;
}