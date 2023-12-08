#include "tfg_mosaicosim.hpp"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#pragma clang diagnostic ignored "-Wall"
#include "../inc/imgui/imgui.h"

#include "../res/fonts/segoeui.h"
#include "../res/fonts/fontawesome85.h"
#include "../inc/icons/icons_font_awesome.h"

#include "../inc/sokol/sokol_log.h"
#pragma clang diagnostic pop

using namespace ImGui;

namespace mosaico_sim
{
	extern ms_state ms;

	void        gui_init(void);
	void        gui_frame(void);
	void        gui_input(const sapp_event *e);
	void        gui_cleanup(void);
	inline void gui_render(void);
	static void gui_font_main_load(const ImGuiIO &io);
	static void gui_font_icon_load(const ImGuiIO &io);
	static void gui_menubar_frame(void);
	static void gui_sidebar_frame(void);
	static void gui_workspacewindow_frame(void);

	void gui_init(void)
	{
		simgui_setup(&(simgui_desc_t)
		{
			.no_default_font = true,
			.logger.func = slog_func,
		});
		ms.offscreen.resolution = 4096;
		// StyleColorsDark(&GetStyle());
		StyleColorsLight(&GetStyle());
		ImGuiIO &io = GetIO();
		gui_font_main_load(io);
		gui_font_icon_load(io);
		sg_image offscreen_image = sg_make_image(&(sg_image_desc)
		{
			.render_target = true,
			.width = ms.offscreen.resolution,
			.height = ms.offscreen.resolution,
			.pixel_format = SG_PIXELFORMAT_RGBA8,
		});
		ms.offscreen.texture = simgui_make_image(&(simgui_image_desc_t)
		{
			.image = offscreen_image,
			.sampler = sg_make_sampler(&(sg_sampler_desc)
			{
				.min_filter = SG_FILTER_LINEAR,
				.mag_filter = SG_FILTER_LINEAR,
				.wrap_u = SG_WRAP_CLAMP_TO_EDGE,
				.wrap_v = SG_WRAP_CLAMP_TO_EDGE,
			}),
		});
		ms.offscreen.pass = sg_make_pass(&(sg_pass_desc)
		{
			.color_attachments[0].image = offscreen_image,
			.depth_stencil_attachment.image = sg_make_image(&(sg_image_desc)
			{
				.render_target = true,
				.width = ms.offscreen.resolution,
				.height = ms.offscreen.resolution,
				.pixel_format = (sg_pixel_format)sapp_depth_format(),
			}),
		});
		ms.offscreen.pass_action =
		{
			.colors[0].load_action = SG_LOADACTION_CLEAR,
			.colors[0].clear_value = { 0.0f, 0.0f, 0.0f, 1.0f },
		};

		ms.onscreen.pass_action =
		{
			.colors[0].load_action = SG_LOADACTION_CLEAR,
			.colors[0].clear_value = { 1.0f, 1.0f, 1.0f, 1.0f },
		};

		ms.menubar = ms_menubar
		{
			.height = 22.0f,
			.hide = false,
		};

		ms.sidebar = ms_sidebar
		{
			.button_padding = 5.0f,
			.rect = { 0.0f, 0.0f, 300.0f, ms.mainwindow.h },
			.flags = ImGuiWindowFlags_None
				| ImGuiWindowFlags_NoMove
				| ImGuiWindowFlags_NoCollapse
				| ImGuiWindowFlags_NoTitleBar,
			.hide = false,
		};
	}

	void gui_frame()
	{
		simgui_new_frame(&(simgui_frame_desc_t)
		{
			.width = sapp_width(),
			.height = sapp_height(),
			.delta_time = sapp_frame_duration(),
			.dpi_scale = sapp_dpi_scale(),
		});

		gui_menubar_frame();
		gui_sidebar_frame();
		gui_workspacewindow_frame();
		// if (Begin("idk"))
		// {
		// 	Text("%s ", ICON_FA_GLASS); SameLine();
		// 	End();
		// }
	}

	void gui_input(const sapp_event *e)
	{
		simgui_handle_event(e);
	}

	void gui_cleanup(void)
	{
		simgui_shutdown();
	}

	inline void gui_render(void)
	{
		simgui_render();
	}

	static void gui_font_main_load(const ImGuiIO &io)
	{
		ImFontConfig font_main_cfg = { };
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
		io.Fonts->TexID = simgui_imtextureid(simgui_make_image(&(simgui_image_desc_t)
		{
			.image = sg_make_image(&(sg_image_desc)
			{
				.width = font_main_width,
				.height = font_main_height,
				.pixel_format = SG_PIXELFORMAT_RGBA8,
				.data.subimage[0][0].ptr = font_main_pixels,
				.data.subimage[0][0].size = (size_t)(font_main_width * font_main_height * 4),
			}),
			.sampler = sg_make_sampler(&(sg_sampler_desc)
			{
				.min_filter = SG_FILTER_LINEAR,
				.mag_filter = SG_FILTER_LINEAR,
				// .min_filter = SG_FILTER_NEAREST; // high dpi displays
				// .mag_filter = SG_FILTER_NEAREST; // high dpi displays
				.wrap_u = SG_WRAP_CLAMP_TO_EDGE,
				.wrap_v = SG_WRAP_CLAMP_TO_EDGE,
			}),
		}));
	}

	static void gui_font_icon_load(const ImGuiIO &io)
	{
		ImFontConfig font_icon_cfg;
		font_icon_cfg.MergeMode = true;
		font_icon_cfg.GlyphMinAdvanceX = 13.0f; // Use if you want to make the icon monospaced
		static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };

		ms.font_icon = io.Fonts->AddFontFromMemoryCompressedBase85TTF(
			fontawesome_compressed_data_base85,
			13.0f,
			&font_icon_cfg,
			icon_ranges);

		unsigned char* font_icon_pixels;
		int font_icon_width, font_icon_height;
		io.Fonts->GetTexDataAsRGBA32(&font_icon_pixels, &font_icon_width, &font_icon_height);
		io.Fonts->TexID = simgui_imtextureid(simgui_make_image(&(simgui_image_desc_t)
		{
			.image = sg_make_image(&(sg_image_desc)
			{
				.width = font_icon_width,
				.height = font_icon_height,
				.pixel_format = SG_PIXELFORMAT_RGBA8,
				.data.subimage[0][0].ptr = font_icon_pixels,
				.data.subimage[0][0].size = (size_t)(font_icon_width * font_icon_height * 4),
			}),
			.sampler = sg_make_sampler(&(sg_sampler_desc)
			{
				.min_filter = SG_FILTER_LINEAR,
				.mag_filter = SG_FILTER_LINEAR,
				.wrap_u = SG_WRAP_CLAMP_TO_EDGE,
				.wrap_v = SG_WRAP_CLAMP_TO_EDGE,
			}),
		}));
	}

	static void gui_menubar_frame(void)
	{
		if (!ms.menubar.hide && BeginMainMenuBar())
		{
			ms.menubar.height = GetWindowHeight();

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

	static void gui_sidebar_frame(void)
	{
		ms.sidebar.rect.x = ms.mainwindow.x;
		ms.sidebar.rect.y = ms.menubar.height;
		ms.sidebar.rect.h = ms.mainwindow.h - ms.sidebar.rect.y;

		SetNextWindowPos(ImVec2(ms.sidebar.rect.x, ms.sidebar.rect.y));
		SetNextWindowSizeConstraints(
			ImVec2(250.0f, ms.sidebar.rect.h),
			ImVec2(ms.mainwindow.w * 0.5f, ms.sidebar.rect.h));
		bool show = !ms.sidebar.hide;
		if (!ms.sidebar.hide && Begin("Barra de Controle", &show, ms.sidebar.flags))
		{
			ms.sidebar.rect.w = GetWindowWidth();
			float button_size = ms.sidebar.rect.w - 3.0f * ms.sidebar.button_padding;

			static int vertices = 3;
			static unsigned char step = 1;

			SeparatorText("CONTROLES");
			Spacing();

			Text("Zoom %s", ICON_FA_SEARCH);
			SetNextItemWidth(button_size);
			InputFloat("##Zoom", &ms.workspace.scale, 0.1f, 1.0f, "%.2f");
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

			if (Checkbox("Ocultar", &ms.workspace.background_image.hide))
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

	static void gui_workspacewindow_frame()
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
		SetNextWindowPos(ImVec2(ms.sidebar.rect.w, ms.sidebar.rect.y), posCond);
		SetNextWindowSize(ImVec2(ms.mainwindow.w - ms.sidebar.rect.w, ms.sidebar.rect.h), sizeCond);
		if (Begin("workspace", NULL, workspace_flags))
		{
			ImVec2 window_pos = GetWindowPos();
			ImVec2 window_size = GetWindowSize();
			ms.workspace.window_rect =
			{
				.x = window_pos.x,
				.y = window_pos.y,
				.w = window_size.x,
				.h = window_size.y,
			};
			ms.workspace.window_ratio = ms.workspace.window_rect.w / ms.workspace.window_rect.h;

			ImVec2 content_min = GetWindowContentRegionMin();
			ImVec2 content_max = GetWindowContentRegionMax();
			ms.workspace.viewport_rect =
			{
				.x = 0.0f,
				.y = 0.0f,
				.w = content_max.x - content_min.x,
				.h = content_max.y - content_min.y,
			};
			ms.workspace.viewport_ratio = ms.workspace.viewport_rect.w / ms.workspace.viewport_rect.h;

			if (!ms.workspace.hide)
			{
				sg_image_desc image = sg_query_image_desc(ms.workspace.background_image.texture);
				float image_ratio = (float)image.width / (float)image.height;
				ImTextureID texture_id = simgui_imtextureid(ms.offscreen.texture);
				if (ms.workspace.background_image.loadstate == LOADSTATE_SUCCESS)
				{
					ImVec2 image_rect = ImVec2(ms.workspace.viewport_rect.w, ms.workspace.viewport_rect.h);
					bool image_is_larger_than_viewport = image.width > image_rect.x || image.height > image_rect.y;
					bool width_is_larger_than_height = (float)image.width / (float)image.height > 1;

					if (image_is_larger_than_viewport && width_is_larger_than_height)
					{
						image_rect.x = image.width;
						image_rect.y = image.width / ms.workspace.viewport_ratio;
					}
					else
					{
						image_rect.x = image.height * ms.workspace.viewport_ratio;
						image_rect.y = image.height;
					}

					image_rect.x *= ms.workspace.scale;
					image_rect.y *= ms.workspace.scale;
					Image(texture_id, image_rect);
				}
				else
				{
					Image(texture_id, ImVec2(ms.workspace.viewport_rect.w, ms.workspace.viewport_rect.h));
				}
			}
			End();
		}

		if (ms.workspace_frame_callback)
		{
			ImColor bg = GetStyle().Colors[ImGuiCol_WindowBg];
			ms.workspace.background_color =
			{
				.r = bg.Value.x,
				.g = bg.Value.y,
				.b = bg.Value.z,
				.a = bg.Value.w,
			};
			ms.workspace_frame_callback();
		}

		if (ms.debug_mode)
		{
			SetNextWindowPos(ImVec2(300.0f, 22.0f), ImGuiCond_Once);
			if (Begin("DEBUG", &ms.debug_mode, ImGuiWindowFlags_AlwaysAutoResize))
			{
				SeparatorText("GENERAL");
				Text("WINDOW: (%.1f, %.1f, %.1f, %.1f)",
					ms.mainwindow.x,
					ms.mainwindow.y,
					ms.mainwindow.w,
					ms.mainwindow.h);
				Text("WS_WINDOW: (%.1f, %.1f, %.1f, %.1f)",
					ms.workspace.window_rect.x,
					ms.workspace.window_rect.y,
					ms.workspace.window_rect.w,
					ms.workspace.window_rect.h);
				Text("WS_VIEWPORT: (%.1f, %.1f, %.1f, %.1f)",
					ms.workspace.viewport_rect.x,
					ms.workspace.viewport_rect.y,
					ms.workspace.viewport_rect.w,
					ms.workspace.viewport_rect.h);

				sg_image_desc image = sg_query_image_desc(ms.workspace.background_image.texture);
				Text("OFFSCREEN_IMAGE: (0, 0, %d, %d)",
					image.width,
					image.height);

				SeparatorText("INPUT");
				Text("MOUSE_POSITION: (%.1f, %.1f)",
					ms.mouse.x,
					ms.mouse.y);
				Text("MOUSE_POSITION_PREVIOUS: (%.1f, %.1f)",
					ms.mouse.x_previous,
					ms.mouse.y_previous);

				End();
			}
		}
	}
}