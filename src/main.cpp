// --------------------------------------------------------------------------------------------------------------------

#include "types.h"

#include "image.h"

#include "triangle.h"

// --------------------------------------------------------------------------------------------------------------------

#define IMGUI_IMPLEMENTATION

#define IMGUI_DEMO

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

#include <string.h>

#include <vector>

using namespace std;

// --------------------------------------------------------------------------------------------------------------------

#define global static

// --------------------------------------------------------------------------------------------------------------------

global program_state_t program;

// --------------------------------------------------------------------------------------------------------------------

static void draw_main_menu_bar(void)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Arquivo"))
		{
			if (ImGui::MenuItem("Abrir imagem...", "CTRL+O")) 
			{
				open_image(&program.main_image);
			}
			if (ImGui::MenuItem("Sair", "ALT+F4")) { sapp_request_quit(); }
			ImGui::EndMenu();
		}
		// if (ImGui::BeginMenu("Editar"))
		// {
		// 	if (ImGui::MenuItem("Desfazer", "CTRL+Z")) {}
		// 	if (ImGui::MenuItem("Refazer", "CTRL+Y", false, false)) {}  // Disabled item
		// 	ImGui::Separator();
		// 	if (ImGui::MenuItem("Recortar", "CTRL+X")) {}
		// 	if (ImGui::MenuItem("Copiar", "CTRL+C")) {}
		// 	if (ImGui::MenuItem("Colar", "CTRL+V")) {}
		// 	ImGui::EndMenu();
		// }
		program.main_menu_bar_height = ImGui::GetWindowSize().y;
		ImGui::EndMainMenuBar();
	}
}

// --------------------------------------------------------------------------------------------------------------------

static void draw_side_bar(void)
{
	bool sidebar_open = true;
	ImGui::SetNextWindowPos(ImVec2(0, program.main_menu_bar_height));
	ImGui::SetNextWindowSize(ImVec2(program.side_bar_width, sapp_height() - program.main_menu_bar_height));
	if (ImGui::Begin("sidebar", &sidebar_open,
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize 
|       ImGuiWindowFlags_NoTitleBar))
	{
		if (ImGui::Button("Carregar imagem", ImVec2(185.0f, 0.0f)))     { open_image(&program.main_image); }
		if (ImGui::Button("Adicionar triângulo", ImVec2(185.0f, 0.0f))) { add_triangle(&program); }
		ImGui::InputInt("ID", (int *)(&program.selected));
		int tri_index = get_triangle(&program);
		if (tri_index >= 0)
		{
			ImGui::InputFloat2("A", program.tri_list[(id_t)tri_index].a);
			ImGui::InputFloat2("B", program.tri_list[(id_t)tri_index].b);
			ImGui::InputFloat2("C", program.tri_list[(id_t)tri_index].c);
			ImGui::ColorPicker3("Cor", program.tri_list[(id_t)tri_index].color, 
				ImGuiColorEditFlags_NoAlpha | 
				ImGuiColorEditFlags_NoSidePreview);
		}
		ImGui::End();
	}
}

// --------------------------------------------------------------------------------------------------------------------

static void frame(void) 
{
	// update window size
	program.window_width = sapp_width();
	program.window_height = sapp_height();
	program.window_ratio = program.window_width / (float)program.window_height;

	// update viewport size
	program.viewport_x = program.side_bar_width;
	program.viewport_y = program.main_menu_bar_height;
	program.viewport_width = program.window_width - program.side_bar_width;
	program.viewport_height = program.window_height - program.main_menu_bar_height;
	program.viewport_ratio = program.viewport_width / (float)program.viewport_height;

	sgp_begin(program.window_width, program.window_height);
	simgui_new_frame({ program.window_width, program.window_height, sapp_frame_duration(), sapp_dpi_scale() });

	sgp_set_color(0.05f, 0.05f, 0.05f, 1.0f);
	sgp_clear();
	sgp_reset_color();

	draw_main_menu_bar();
	draw_side_bar();
	draw_main_image(&program);
	draw_triangles(&program);

	sg_pass_action pass_action = { };
	sg_begin_default_pass(&pass_action, program.window_width, program.window_height);
	sgp_reset_pipeline();
	sgp_flush();
	sgp_end();
	simgui_render();
	sg_end_pass();
	sg_commit();
}

// --------------------------------------------------------------------------------------------------------------------

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

// --------------------------------------------------------------------------------------------------------------------

static void cleanup(void) 
{
	sg_destroy_image(program.main_image);
	simgui_shutdown();
	sgp_shutdown();
	sg_shutdown();
}

// --------------------------------------------------------------------------------------------------------------------

static void input(const sapp_event* e) 
{
	simgui_handle_event(e);

	if (e->mouse_x > program.viewport_x &&
		e->mouse_y > program.viewport_y &&
		e->type == SAPP_EVENTTYPE_MOUSE_DOWN && 
		e->mouse_button == SAPP_MOUSEBUTTON_LEFT)
	{
		for (auto tri : program.tri_list)
		{
			float pt[] = { e->mouse_x, e->mouse_y };
			if (point_is_inside_triangle(pt, tri))
			{
				program.selected = tri.id;
			}
			else
			{
				program.selected = 0;
			}
		}
	}
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