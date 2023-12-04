#include "tfg_types.hpp"
#include "tfg_image.cpp"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#pragma clang diagnostic ignored "-Wall"
#include "../inc/imgui/imgui.h"

#include "../inc/sokol/util/sokol_imgui.h"

#include "../res/fonts/segoeui.h"
#include "../res/fonts/fontawesome85.h"
#include "../inc/icons/icons_font_awesome.h"

#include "../inc/sokol/sokol_gp.h"
#pragma clang diagnostic pop

using namespace ImGui;

namespace mosaico_sim
{
	extern mosaico_sim_state ms;

	void interface_init(void);
	void interface_frame(void);
	void interface_input(const sapp_event *e);
	void interface_cleanup(void);

	inline void interface_render(void);
	static void fonts_load(void);
	static void menu_frame(void);
	static void sidebar_frame(void);
	static void workspace_frame(void);
	static void workspace_window_frame(void);
	static void workspace_window_contents_frame(void);
	static void workspace_update_window(rectf rect);
	static void workspace_update_viewport(rectf rect);

	void interface_init(void)
	{
		simgui_setup(&(simgui_desc_t)
		{
			.no_default_font = true,
			.logger.func = slog_func,
		});

		StyleColorsLight(&GetStyle());

		fonts_load();

		ms.gui.side_bar.rect =
		{
			.x = 0.0f,
			.y = 0.0f,
			.w = 300.0f,
			.h = sapp_heightf(),
		};
		ms.gui.side_bar.button_padding = 5.0f;

		sgp_setup(&(sgp_desc) {});

		sg_image_desc fb_image_desc;
		memset(&fb_image_desc, 0, sizeof(sg_image_desc));
		fb_image_desc.render_target = true;
		fb_image_desc.width = ms.main_window.w;
		fb_image_desc.height = ms.main_window.h;
		ms.gui.workspace.framebuffer_image_color = sg_make_image(&fb_image_desc);

		sg_image_desc fb_depth_image_desc;
		memset(&fb_depth_image_desc, 0, sizeof(sg_image_desc));
		fb_depth_image_desc.render_target = true;
		fb_depth_image_desc.width = ms.main_window.w;
		fb_depth_image_desc.height = ms.main_window.h;
		fb_depth_image_desc.pixel_format = (sg_pixel_format)sapp_depth_format();
		ms.gui.workspace.framebuffer_image_depth = sg_make_image(&fb_depth_image_desc);

		sg_sampler_desc linear_sampler_desc =
		{
			.min_filter = SG_FILTER_LINEAR,
			.mag_filter = SG_FILTER_LINEAR,
			.wrap_u = SG_WRAP_CLAMP_TO_EDGE,
			.wrap_v = SG_WRAP_CLAMP_TO_EDGE,
		};
		ms.gui.workspace.framebuffer_sampler = sg_make_sampler(&linear_sampler_desc);

		sg_pass_desc pass_desc;
		memset(&pass_desc, 0, sizeof(sg_pass_desc));
		pass_desc.color_attachments[0].image = ms.gui.workspace.framebuffer_image_color;
		pass_desc.depth_stencil_attachment.image = ms.gui.workspace.framebuffer_image_depth;
		ms.gui.workspace.framebuffer_pass = sg_make_pass(&pass_desc);

		ms.gui.workspace.framebuffer_texture = simgui_make_image(&(simgui_image_desc_t)
		{
			.image = ms.gui.workspace.framebuffer_image_color,
			.sampler = ms.gui.workspace.framebuffer_sampler,
		});

		image_init();
	}

	void interface_frame()
	{
		simgui_new_frame(&(simgui_frame_desc_t)
		{
			.width = sapp_width(),
			.height = sapp_height(),
			.delta_time = sapp_frame_duration(),
			.dpi_scale = sapp_dpi_scale(),
		});

		menu_frame();
		sidebar_frame();
		workspace_frame();
		// if (Begin("idk"))
		// {
		// 	Text("%s ", ICON_FA_GLASS); SameLine();
		// 	End();
		// }
	}

	void interface_input(const sapp_event *e)
	{
		simgui_handle_event(e);
		image_input(e);
	}

	void interface_cleanup(void)
	{
		image_cleanup();
		sgp_shutdown();
		simgui_shutdown();
	}

	inline void interface_render(void)
	{
		simgui_render();
	}

	static void fonts_load(void)
	{
		auto& io = GetIO();
		ImFontConfig font_main_cfg;
		font_main_cfg.FontDataOwnedByAtlas = false;
		font_main_cfg.OversampleH = 2;
		font_main_cfg.OversampleV = 2;
		font_main_cfg.RasterizerMultiply = 1.0f;
		ms.font_main = io.Fonts->AddFontFromMemoryCompressedTTF(
			segoeui_compressed_data,
			segoeui_compressed_size,
			ms.font_size,
			&font_main_cfg);

		unsigned char* font_main_pixels;
		int font_main_width, font_main_height;
		io.Fonts->GetTexDataAsRGBA32(&font_main_pixels, &font_main_width, &font_main_height);
		sg_image_desc font_main_img_desc = { };
		font_main_img_desc.width = font_main_width;
		font_main_img_desc.height = font_main_height;
		font_main_img_desc.pixel_format = SG_PIXELFORMAT_RGBA8;
		font_main_img_desc.data.subimage[0][0].ptr = font_main_pixels;
		font_main_img_desc.data.subimage[0][0].size = font_main_width * font_main_height * 4;
		sg_image font_main_img = sg_make_image(&font_main_img_desc);
		sg_sampler_desc font_smp_desc = { };
		font_smp_desc.min_filter = SG_FILTER_LINEAR;
		font_smp_desc.mag_filter = SG_FILTER_LINEAR;
		// for high dpi displays
		// font_smp_desc.min_filter = SG_FILTER_NEAREST;
		// font_smp_desc.mag_filter = SG_FILTER_NEAREST;
		font_smp_desc.wrap_u = SG_WRAP_CLAMP_TO_EDGE;
		font_smp_desc.wrap_v = SG_WRAP_CLAMP_TO_EDGE;
		sg_sampler font_main_smp = sg_make_sampler(&font_smp_desc);
		simgui_image_desc_t font_main_desc = { };
		font_main_desc.image = font_main_img;
		font_main_desc.sampler = font_main_smp;
		io.Fonts->TexID = simgui_imtextureid(simgui_make_image(&font_main_desc));

		ImFontConfig font_icon_cfg;
		font_icon_cfg.MergeMode = true;
		font_icon_cfg.GlyphMinAdvanceX = 13.0f; // Use if you want to make the icon monospaced
		static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		io.Fonts->AddFontFromMemoryCompressedBase85TTF(
			fontawesome_compressed_data_base85,
			13.0f,
			&font_icon_cfg,
			icon_ranges);
		unsigned char* font_icon_pixels;
		int font_icon_width, font_icon_height;
		io.Fonts->GetTexDataAsRGBA32(&font_icon_pixels, &font_icon_width, &font_icon_height);
		sg_image_desc font_icon_img_desc = { };
		font_icon_img_desc.width = font_icon_width;
		font_icon_img_desc.height = font_icon_height;
		font_icon_img_desc.pixel_format = SG_PIXELFORMAT_RGBA8;
		font_icon_img_desc.data.subimage[0][0].ptr = font_icon_pixels;
		font_icon_img_desc.data.subimage[0][0].size = font_icon_width * font_icon_height * 4;
		sg_image font_icon_img = sg_make_image(&font_icon_img_desc);
		sg_sampler font_icon_smp = sg_make_sampler(&font_smp_desc);
		simgui_image_desc_t font_icon_desc = { };
		font_icon_desc.image = font_icon_img;
		font_icon_desc.sampler = font_icon_smp;
		io.Fonts->TexID = simgui_imtextureid(simgui_make_image(&font_icon_desc));
	}

	static void menu_frame(void)
	{
		if (BeginMainMenuBar())
		{
			ms.gui.menu_bar.h = GetWindowHeight();

			if (BeginMenu("Arquivo"))
			{
				if (MenuItem("Novo projeto", "CTRL+N"))
				{
				}

				if (MenuItem("Abrir projeto", "CTRL+O"))
				{
				}

				if (MenuItem("Salvar projeto", "CTRL+S"))
				{
				}

				Separator();

				if (MenuItem("Sair", "ALT+F4"))
				{
				}

				EndMenu();
			}

			if (BeginMenu("Editar"))
			{
				if (MenuItem("Selecionar tudo", "CTRL+A"))
				{
				}

				Separator();

				if (MenuItem("Desfazer", "CTRL+Z"))
				{
				}

				if (MenuItem("Refazer", "CTRL+Y"))
				{
				}

				Separator();

				if (MenuItem("Recortar", "CTRL+X"))
				{
				}

				if (MenuItem("Copiar", "CTRL+C"))
				{
				}

				if (MenuItem("Colar", "CTRL+V"))
				{
				}

				if (MenuItem("Duplicar", "CTRL+D"))
				{
				}

				Separator();

				{
				}

				EndMenu();
			}

			if (BeginMenu("Opções"))
			{
				if (BeginMenu("Tema da interface"))
				{
					ImGuiStyle& style = GetStyle();
					if (MenuItem("Claro", 0))
					{
						StyleColorsLight(&style);
					}
					if (MenuItem("Escuro", 0))
					{
						StyleColorsDark(&style);
					}
					EndMenu();
				}

				if (BeginMenu("Tamanho da fonte"))
				{
					int step = 1;
					if (InputScalar("##font_size", ImGuiDataType_U32, &ms.font_size, &step))
					{
						if (ms.font_size < 8) { ms.font_size = 8; }
						ImGuiIO &io = GetIO();
						io.FontGlobalScale = ms.font_size / 16.0f;
					}
					EndMenu();
				}
				EndMenu();
			}

			EndMainMenuBar();
		}
	}

	static void sidebar_frame(void)
	{
		ms.gui.side_bar.rect.x = ms.main_window.x;
		ms.gui.side_bar.rect.y = ms.gui.menu_bar.h;
		ms.gui.side_bar.rect.h = ms.main_window.h - ms.gui.side_bar.rect.y;

		ms.gui.side_bar.flags = ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoCollapse
			//| ImGuiWindowFlags_NoBackground
			| ImGuiWindowFlags_NoTitleBar;
		SetNextWindowPos(ImVec2(ms.gui.side_bar.rect.x, ms.gui.side_bar.rect.y));
		SetNextWindowSizeConstraints(
			ImVec2(250.0f, ms.gui.side_bar.rect.h),
			ImVec2(ms.main_window.w * 0.5f, ms.gui.side_bar.rect.h));
		if (Begin("Barra de Controle", &ms.gui.side_bar.show, ms.gui.side_bar.flags))
		{
			ms.gui.side_bar.rect.w = GetWindowWidth();
			float button_size = ms.gui.side_bar.rect.w - 3.0f * ms.gui.side_bar.button_padding;

			static int vertices = 3;
			static unsigned char step = 1;

			SeparatorText("CONTROLES");
			Spacing();

			Text("Zoom %s", ICON_FA_SEARCH);
			static float zoom = 1.0f;
			SetNextItemWidth(button_size);
			InputFloat("##Zoom", &zoom, 0.1f, 1.0f, "%.2f");
			Spacing();

			for (int i = 0; i < 10; ++i)
			{
				Spacing();
			}

			SeparatorText("IMAGEM DE REFERÊNCIA");
			Spacing();

			BeginDisabled(false);

			if (Button("Adicionar##ref", ImVec2(button_size, 0.0f)))
			{

			}
			EndDisabled();

			BeginDisabled(false);
			if (Button("Remover##ref", ImVec2(button_size, 0.0f)))
			{
			}
			EndDisabled();

			if (Checkbox("Ocultar", &ms.gui.workspace.image.hide))
			{}

			for (int i = 0; i < 10; ++i)
			{
				Spacing();
			}

			SeparatorText("PEÇA");
			Spacing();

			Text("Cor");
			static color current_color = {};
			SetNextItemWidth(button_size);
			if (ColorEdit3(
				"## COR PEÇA",
				&current_color.r))
			{
			}

			Spacing();
			Text("%s", "Número de lados");
			SetNextItemWidth(button_size);
			if (InputScalar("## ADD_PEÇA", ImGuiDataType_U8, &vertices, &step))
			{
				if (vertices < 3) vertices = 3;
				if (vertices > 9) vertices = 9;
			}

			if (Button("Adicionar##peça", ImVec2(button_size, 0.0f)))
			{
			}

			BeginDisabled(false);
			if (Button("Remover##peça", ImVec2(button_size, 0.0f)))
			{
			}
			EndDisabled();

			BeginDisabled(false);
			if (Button("Limpar##peça", ImVec2(button_size, 0.0f)))
			{
			}
			EndDisabled();

			for (int i = 0; i < 10; ++i) Spacing();
			SeparatorText("REJUNTE");
			Spacing();
			static bool overlap_show = false;
			Checkbox("Sobrepor imagem de referência", &overlap_show);
			Text("Cor");
			SetNextItemWidth(button_size);
			static color overlap_color = {};
			ColorEdit3("##CORREJUNTE", &overlap_color.r);
			End();
		}
	}

	static void workspace_frame(void)
	{
		workspace_window_frame();
		workspace_window_contents_frame();
	}

	static void workspace_window_frame()
	{
		ImGuiWindowFlags workspace_flags =
			ImGuiWindowFlags_None
			| ImGuiWindowFlags_NoCollapse
			// | ImGuiWindowFlags_NoBackground
			| ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoBringToFrontOnFocus
			| ImGuiWindowFlags_HorizontalScrollbar;
		ImGuiCond posCond = ImGuiCond_Always;
		ImGuiCond sizeCond = ImGuiCond_Always;
		SetNextWindowBgAlpha(1.0f);
		SetNextWindowPos(ImVec2(ms.gui.side_bar.rect.w, ms.gui.side_bar.rect.y), posCond);
		SetNextWindowSize(ImVec2(ms.main_window.w - ms.gui.side_bar.rect.w, ms.gui.side_bar.rect.h), sizeCond);
		if (Begin("workspace", NULL, workspace_flags))
		{
			ImVec2 window_pos = GetWindowPos();
			ImVec2 window_size = GetWindowSize();
			ms.gui.workspace.window =
			{
				.x = window_pos.x,
				.y = window_pos.y,
				.w = window_size.x,
				.h = window_size.y,
			};

			ImVec2 content_min = GetWindowContentRegionMin();
			ImVec2 content_max = GetWindowContentRegionMax();
			ms.gui.workspace.viewport =
			{
				.x = 0.0f,
				.y = 0.0f,
				.w = content_max.x,
				.h = content_max.y,
			};
			ImTextureID texture_id = simgui_imtextureid(ms.gui.workspace.framebuffer_texture);
			Image(texture_id, ImVec2(ms.gui.workspace.image.rect.w, ms.gui.workspace.image.rect.h));

			End();
		}
	}

	static bool image_rect_draw;
	static bool viewport_vertex_draw;

	static void workspace_window_contents_frame(void)
	{
		sgp_begin(ms.gui.workspace.window.w, ms.gui.workspace.window.h);
		sgp_viewport(
			ms.gui.workspace.viewport.x,
			ms.gui.workspace.viewport.y,
			ms.gui.workspace.viewport.w,
			ms.gui.workspace.viewport.h);

		ImColor bg = GetStyle().Colors[ImGuiCol_WindowBg];
		sgp_set_color(
			bg.Value.x,
			bg.Value.y,
			bg.Value.z,
			bg.Value.w);
		sgp_clear();
		sgp_reset_color();

		if (ms.debug_mode)
		{
		}

		// draw_pieces
		// sgp_set_color(0.0f, 1.0f, 1.0f, 1.0f);
		// sgp_draw_filled_rect(0, 0, content_area.x, content_area.y);
		image_frame();

		sg_pass_action pass_action;
		memset(&pass_action, 0, sizeof(sg_pass_action));
		pass_action.colors[0].load_action = SG_LOADACTION_CLEAR;
		sg_begin_pass(ms.gui.workspace.framebuffer_pass, &pass_action);
		sgp_flush();
		sgp_end();
		sg_end_pass();

		SetNextWindowPos(ImVec2(300.0f, 100.0f), ImGuiCond_Once);
		if (ms.debug_mode && Begin("DEBUG", &ms.debug_mode, ImGuiWindowFlags_AlwaysAutoResize))
		{
			SeparatorText("MOSAICO SIM");
			Text("WINDOW: (w: %.1f, h: %.1f)", ms.main_window.w, ms.main_window.h);
			Text("FRAME TIME: %.1fs", ms.frame_time);

			SeparatorText("INTERFACE > MENU");
			Text("HEIGHT: %.1f", ms.gui.menu_bar.h);

			SeparatorText("INTERFACE > CONTROLES");
			Text(
				"RECT: (x: %.1f, y: %.1f, w: %.1f, h: %.1f)",
				ms.gui.side_bar.rect.x, ms.gui.side_bar.rect.y,
				ms.gui.side_bar.rect.w, ms.gui.side_bar.rect.h);

			SeparatorText("INTERFACE > ÁREA DE TRABALHO");
			Checkbox("DEBUG VERTEX", &viewport_vertex_draw);
			Text(
				"WINDOW RECT: (x: %.1f, y: %.1f, w: %.1f, h: %.1f)",
				ms.gui.workspace.window.x, ms.gui.workspace.window.y,
				ms.gui.workspace.window.w, ms.gui.workspace.window.h);
			Text(
				"VIEWPORT RECT: (x: %.1f, y: %.1f, w: %.1f, h: %.1f)",
				ms.gui.workspace.viewport.x, ms.gui.workspace.viewport.y,
				ms.gui.workspace.viewport.w, ms.gui.workspace.viewport.h);

			SeparatorText("INTERFACE > IMAGEM");
			Text(
				"IMAGE RECT: (x: %.1f, y: %.1f, w: %.1f, h: %.1f)",
				ms.gui.workspace.image.rect.x, ms.gui.workspace.image.rect.y,
				ms.gui.workspace.image.rect.w, ms.gui.workspace.image.rect.h);
			SameLine();
			Checkbox("##DESENHAR", &image_rect_draw);
			End();
		}
	}
}