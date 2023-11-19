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

namespace MosaicoSim
{
	struct
	{
		struct
		{
			simgui_image_desc_t imdesc;
			simgui_image_t imimg;
			ImTextureID imid;
			sg_pass pass;
		}
		offscreen;

		ImVec2 workspaceSize;
		ImVec2 workspacePosition;
	}
	state;

	void Init();
	void Frame();
	void Input(const sapp_event *e);
	void Cleanup();

	void Init()
	{
		sg_setup(&(sg_desc)
		{
			.context = sapp_sgcontext(),
			.logger.func = slog_func,
		});

		sgp_setup(&(sgp_desc){});

		simgui_setup(&(simgui_desc_t)
		{
			// .no_default_font = false,
		});
		// auto &io = ImGui::GetIO();
		// ImFontConfig fontCfg;
		// fontCfg.FontDataOwnedByAtlas = true;
		// fontCfg.OversampleH = 1;
		// fontCfg.OversampleV = 1;
		// fontCfg.RasterizerMultiply = 1.0f;
		// io.Fonts->AddFontFromMemoryCompressedTTF(
		// 	segoeui_compressed_data,
		// 	segoeui_compressed_size,
		// 	16.0f,
		// 	&fontCfg);
		// unsigned char *font_pixels;
		// int font_width, font_height;
		// io.Fonts->GetTexDataAsRGBA32(&font_pixels, &font_width, &font_height);
		// sg_image_desc img_desc =
		// {
		// 	.width = font_width,
		// 	.height = font_height,
		// 	.pixel_format = SG_PIXELFORMAT_RGBA8,
		// 	.data.subimage[0][0].ptr = font_pixels,
		// 	.data.subimage[0][0].size = (size_t)(font_width * font_height * 4),
		// };
		// io.Fonts->TexID = (ImTextureID)(uintptr_t)sg_make_image(&img_desc).id;

		state.offscreen.imdesc =
		{
			.image = sg_make_image(&(sg_image_desc)
			{
				.render_target = true,
				.width = 128,
				.height = 128,
			}),
			.sampler = sg_make_sampler(&(sg_sampler_desc)
			{
				.min_filter = SG_FILTER_LINEAR,
				.mag_filter = SG_FILTER_LINEAR,
				.wrap_u = SG_WRAP_CLAMP_TO_EDGE,
				.wrap_v = SG_WRAP_CLAMP_TO_EDGE,
			}),
		};
		state.offscreen.imimg = simgui_make_image(&state.offscreen.imdesc);

		state.offscreen.imid = simgui_imtextureid(state.offscreen.imimg);

		// state.offscreen.pass = sg_make_pass(&(sg_pass_desc)
		// {
		// 	.color_attachments[0].image = state.offscreen.imdesc.image,
		// });
	}

	void Frame()
	{
		float w = sapp_widthf();
		float h = sapp_heightf();

		sgp_begin(w, h);

		simgui_new_frame(&(simgui_frame_desc_t)
		{
			.width = (int)w,
			.height = (int)h,
			.delta_time = sapp_frame_duration(),
			.dpi_scale = sapp_dpi_scale(),
		});

		ImGui::SetNextWindowPos({0, 0}, ImGuiCond_Once);
		ImGui::SetNextWindowSize({200, 200}, ImGuiCond_Once);
		if (ImGui::Begin("Workspace"))
		{
			state.workspacePosition = ImGui::GetWindowPos();
			state.workspaceSize = ImGui::GetWindowSize();
			ImGui::Image(
				state.offscreen.imid,
				state.workspaceSize);
			ImGui::End();
		}

		// offscreen pass
		sgp_begin(128, 128);
		sgp_project(0, 128, 128, 0);
		sgp_draw_filled_triangle(
			0.0f, 0.0f, 
			0.0f, 128.0f,
			128.0f, 128.0f);
		sg_begin_pass(state.offscreen.pass, &(sg_pass_action)
		{
			.colors[0].load_action = SG_LOADACTION_LOAD,
			.colors[0].clear_value.r = 0.0f,
			.colors[0].clear_value.g = 1.0f,
			.colors[0].clear_value.b = 0.0f,
			.colors[0].clear_value.a = 1.0f,
		});
		sgp_flush();
		sgp_end();
		sg_end_pass();

		// display pass
		// sgp_push_transform();
		// sgp_set_image(0, state.offscreen.imdesc.image);
		// sgp_set_sampler(0, state.offscreen.imdesc.sampler);
		// sgp_set_color(1.0f, 1.0f, 1.0f, 1.0f);
		// sgp_draw_filled_rect(
		// 	state.workspacePosition.x, 
		// 	state.workspacePosition.y,
		// 	state.workspaceSize.x,
		// 	state.workspaceSize.y);
		// sgp_rect dest_rect = 
		// {
		// 	state.workspacePosition.x,
		// 	state.workspacePosition.y,
		// 	state.workspaceSize.x,
		// 	state.workspaceSize.y,
		// };
		// sgp_rect dest_rect = {0, 0, 128, 128};
		// sgp_rect src_rect = {0, 0, 128, 128};
		// sgp_draw_textured_rect(
		// 	0,
		// 	dest_rect,
		// 	src_rect);
		// sgp_reset_image(0);
		// sgp_reset_sampler(0);
		// sgp_pop_transform();

		sg_pass_action passAction = {};
		sg_begin_default_pass(
			&passAction,
			w,
			h);
		sgp_reset_pipeline();
		sgp_flush();
		sgp_end();
		simgui_render();
		sg_end_pass();
		sg_commit();
	}

	void Input(const sapp_event *e)
	{
		simgui_handle_event(e);
	}

	void Cleanup()
	{
		simgui_shutdown();
		sgp_shutdown();
		sg_shutdown();
	}
}