#include "types.h"

#include "gui.h"

#include "image.h"

#include "input.h"

#include "shape.h"

// --------------------------------------------------------------------------------------------------------------------

#define IMGUI_IMPLEMENTATION

#include "../include/imgui/imgui_single_file.h"

// --------------------------------------------------------------------------------------------------------------------

#define SOKOL_IMPL

#define SOKOL_GLCORE33

#include "../include/sokol/sokol_app.h"

#include "../include/sokol/sokol_gfx.h"

#include "../include/sokol/sokol_gp.h"

#include "../include/sokol/sokol_log.h"

#include "../include/sokol/sokol_glue.h"

#include "../include/fonts/segoeui.h"

#include "../include/sokol/sokol_imgui.h"

// --------------------------------------------------------------------------------------------------------------------

#include "../include/nfd/nfd_win.cpp"

// --------------------------------------------------------------------------------------------------------------------

#define STB_IMAGE_IMPLEMENTATION

#include "../include/stb/stb_image.h"

// --------------------------------------------------------------------------------------------------------------------

static program_state program;

// --------------------------------------------------------------------------------------------------------------------

static void init();

static void frame();

static void cleanup();

static void input(const sapp_event *e);

static void viewport_translate();

static void viewport_scale();

// --------------------------------------------------------------------------------------------------------------------

static void frame()
{
	// update window size
	program.window.width = sapp_width();
	program.window.height = sapp_height();
	program.window.ratio = program.window.width / (float)program.window.height;

	// update viewport size
	program.viewport =
	{
		.x = 200,
		.y = 0,
		.w = program.window.width - 200,
		.h = program.window.height
	};
	program.viewport.ratio = program.viewport.w / (float)program.viewport.h;

	program.mouse.is_moving = program.mouse.delta.x != 0 || program.mouse.delta.y != 0;

	sgp_begin(program.window.width, program.window.height);
	sgp_viewport(
		program.viewport.x,
		program.viewport.y,
		program.viewport.w,
		program.viewport.h);
	simgui_new_frame({ program.window.width, program.window.height, sapp_frame_duration(), sapp_dpi_scale() });

	sgp_set_color(0.05f, 0.05f, 0.05f, 1.0f);
	sgp_clear();
	sgp_reset_color();

	viewport_translate();
	viewport_scale();
	
	// gui
	side_bar_draw(&program);
	
	// main image
	main_image_draw(&program);

	// shapes
	triangles_update(&program);
	triangles_draw(&program);

#ifdef DEBUG
	debug_window_draw(&program);
#endif

	program.mouse.delta = {};
	program.mouse.scroll = 0;
	sg_pass_action pass_action = {};
	sg_begin_default_pass(&pass_action, program.window.width, program.window.height);
	sgp_reset_pipeline();
	sgp_flush();
	sgp_end();
	simgui_render();
	sg_end_pass();
	sg_commit();
}

static void input(const sapp_event *e)
{
	simgui_handle_event(e);

	update_mouse_position(e, &program);
	update_left_mouse_button(e, &program);
	update_right_mouse_button(e, &program);
	update_middle_mouse_button(e, &program);
}

static void init()
{
	// setup sokol-gfx
	sg_desc desc = { };
	desc.context = sapp_sgcontext();
	desc.logger.func = slog_func;
	sg_setup(&desc);

	// setup simgui
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

	// setup sokol gp
	sgp_desc sgpdesc = { 0 };
	sgp_setup(&sgpdesc);
	if(!sgp_is_valid()) 
	{
		fprintf(stderr, "Failed to create Sokol GP context: %s\n", sgp_get_error_message(sgp_get_last_error()));
		exit(-1);
	}
}

static void cleanup()
{
	sg_destroy_image(program.main_image);
	simgui_shutdown();
	sgp_shutdown();
	sg_shutdown();
}

// --------------------------------------------------------------------------------------------------------------------

sapp_desc sokol_main(int argc, char* argv[]) 
{
	(void)argc;
	(void)argv;
	sapp_desc desc = { };
	desc.init_cb = init;
	desc.frame_cb = frame;
	desc.cleanup_cb = cleanup;
	desc.event_cb = input;
	desc.sample_count = 8;
	desc.width = 1280;
	desc.height = 720;
	desc.high_dpi = true;
	desc.window_title = "tfg";
	desc.icon.sokol_default = true;
	desc.enable_clipboard = true;
	return desc;
}

static void viewport_translate()
{
	if (program.mouse.is_right_button_down)
	{
		program.translation.x += program.mouse.delta.x;
		program.translation.y += program.mouse.delta.y;
	}
	sgp_translate(program.translation.x, program.translation.y);
}

static void viewport_scale()
{
	program.scale += 0.025 * program.mouse.scroll;
}