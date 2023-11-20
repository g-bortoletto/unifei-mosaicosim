#pragma clang diagnostic ignored "-Waddress-of-temporary"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "../inc/imgui/imgui.h"
#include "../inc/imgui/imgui.cpp"
#include "../inc/imgui/imgui_demo.cpp"
#include "../inc/imgui/imgui_draw.cpp"
#include "../inc/imgui/imgui_tables.cpp"
#include "../inc/imgui/imgui_widgets.cpp"

#define SOKOL_IMPL
#define SOKOL_GLES3
#include "../inc/sokol/sokol_app.h"
#include "../inc/sokol/sokol_log.h"
#include "../inc/sokol/sokol_gfx.h"
#include "../inc/sokol/sokol_gp.h"
#include "../inc/sokol/sokol_glue.h"
#include "../inc/sokol/util/sokol_imgui.h"

#include "../res/fonts/segoeui.h"

#include "tfg_types.h"

namespace mosaico_sim
{
	struct
	{
		rectwhf main_window;
		float frame_time;
		float dpi_scale;

		struct
		{
			sg_pass_action pass_action;
		}
		display;
	}
	state;

	void init(void);
	void frame(void);
	void input(const sapp_event *e);
	void cleanup(void);

	void init(void)
	{
		sg_setup(&(sg_desc)
		{
			.context = sapp_sgcontext(),
			.logger.func = slog_func,
		});

		simgui_setup(&(simgui_desc_t)
		{
			.no_default_font = true,
			.logger.func = slog_func,
		});

		ImGui::StyleColorsLight(&ImGui::GetStyle());

		auto& io = ImGui::GetIO();
		ImFontConfig font_cfg;
		font_cfg.FontDataOwnedByAtlas = false;
		font_cfg.OversampleH = 2;
		font_cfg.OversampleV = 2;
		font_cfg.RasterizerMultiply = 1.0f;
		io.Fonts->AddFontFromMemoryCompressedTTF(
			segoeui_compressed_data, 
			segoeui_compressed_size, 
			16.0f, 
			&font_cfg);

		unsigned char* font_pixels;
		int font_width, font_height;
		io.Fonts->GetTexDataAsRGBA32(&font_pixels, &font_width, &font_height);
		sg_image_desc img_desc = { };
		img_desc.width = font_width;
		img_desc.height = font_height;
		img_desc.pixel_format = SG_PIXELFORMAT_RGBA8;
		img_desc.data.subimage[0][0].ptr = font_pixels;
		img_desc.data.subimage[0][0].size = font_width * font_height * 4;
		sg_image font_img = sg_make_image(&img_desc);
		sg_sampler_desc smp_desc = { };
		smp_desc.min_filter = SG_FILTER_LINEAR;
		smp_desc.mag_filter = SG_FILTER_LINEAR;
		// for high dpi displays
		// smp_desc.min_filter = SG_FILTER_NEAREST;
		// smp_desc.mag_filter = SG_FILTER_NEAREST;
		smp_desc.wrap_u = SG_WRAP_CLAMP_TO_EDGE;
		smp_desc.wrap_v = SG_WRAP_CLAMP_TO_EDGE;
		sg_sampler font_smp = sg_make_sampler(&smp_desc);
		simgui_image_desc_t font_desc = { };
		font_desc.image = font_img;
		font_desc.sampler = font_smp;
		io.Fonts->TexID = simgui_imtextureid(simgui_make_image(&font_desc));

		state.display.pass_action.colors[0] = 
		{
			.load_action = SG_LOADACTION_CLEAR,
			.clear_value = { 0.33f, 0.33f, 0.33f, 1.0f },
		};
	}

	void frame(void)
	{
		state.main_window.w = sapp_width();
		state.main_window.h = sapp_height();
		state.frame_time = sapp_frame_duration();
		state.dpi_scale = sapp_dpi_scale();

		simgui_new_frame(&(simgui_frame_desc_t)
		{
			.width = (int)state.main_window.w,
			.height = (int)state.main_window.h,
			.delta_time = state.frame_time,
			.dpi_scale = state.dpi_scale,
		});

		ImGui::SetNextWindowPos({ 50.0f, 50.0f }, ImGuiCond_Once);
		ImGui::SetNextWindowSize({ 300.0f, 300.0f }, ImGuiCond_Once);
		if (ImGui::Begin("The Window"))
		{
			if (ImGui::Button("This is a test"))
			{

			}
			ImGui::End();
		}

		sg_begin_default_passf(
			&state.display.pass_action,
			state.main_window.w,
			state.main_window.h);

		// applyresources here

		// sg_draw here

		simgui_render();
		sg_end_pass();
		sg_commit();
	}

	void input(const sapp_event *e)
	{
		simgui_handle_event(e);
	}

	void cleanup(void)
	{
		simgui_shutdown();
		sg_shutdown();
	}
}