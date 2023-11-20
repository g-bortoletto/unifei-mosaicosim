#pragma clang diagnostic ignored "-Waddress-of-temporary"
#pragma clang diagnostic ignored "-Wc99-designator"
#pragma clang diagnostic ignored "-Wreorder-init-list"

#include "../inc/imgui/imgui.h"

#include "../inc/sokol/sokol_app.h"
#include "../inc/sokol/sokol_log.h"
#include "../inc/sokol/sokol_gfx.h"
#include "../inc/sokol/util/sokol_imgui.h"

#include "../res/fonts/segoeui.h"

#include "tfg_types.h"

using namespace ImGui;

namespace mosaico_sim
{
	struct
	{
		struct
		{
			bool show = true;
			int flags;
			rectf rect;
		} 
		control_bar;

		struct
		{
			bool show = true;
			float h;
		}
		menu_bar;
	}
	interface_state;

	void interface_init(void);
	void interface_frame(void);
	void interface_input(const sapp_event *e);
	void interface_cleanup(void);

	inline void interface_render(void);

	void interface_init(void)
	{
		simgui_setup(&(simgui_desc_t)
		{
			.no_default_font = true,
			.logger.func = slog_func,
		});

		StyleColorsLight(&GetStyle());

		auto& io = GetIO();
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

		interface_state.control_bar.rect =
		{
			.x = 0.0f,
			.y = 0.0f,
			.w = 300.0f,
			.h = sapp_heightf(),
		};
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

		if (BeginMainMenuBar())
		{
			interface_state.menu_bar.h = GetWindowHeight();
			interface_state.control_bar.rect.y = interface_state.menu_bar.h;
			interface_state.control_bar.rect.h = sapp_heightf() - interface_state.control_bar.rect.y;

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
					auto style = &GetStyle();
					if (MenuItem("Claro", 0))
					{
						StyleColorsLight(style);
					}
					if (MenuItem("Escuro", 0))
					{
						StyleColorsDark(style);
					}
					EndMenu();
				}

				if (BeginMenu("Tamanho da fonte"))
				{
					EndMenu();
				}
				EndMenu();
			}

			EndMainMenuBar();
		}

		interface_state.control_bar.flags = ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoCollapse
			//| ImGuiWindowFlags_NoBackground
			| ImGuiWindowFlags_NoTitleBar;
		SetNextWindowPos(ImVec2(interface_state.control_bar.rect.x, interface_state.control_bar.rect.y));
		SetNextWindowSizeConstraints(
			ImVec2(250.0f, interface_state.control_bar.rect.h), 
			ImVec2(sapp_widthf() * 0.5f, interface_state.control_bar.rect.h));
		if (Begin("Barra de Controle", &interface_state.control_bar.show, interface_state.control_bar.flags))
		{
			interface_state.control_bar.rect.w = GetWindowWidth();
			float button_size = interface_state.control_bar.rect.w - 3.0f * 10.0f;

			static int vertices = 3;
			static unsigned char step = 1;

			SeparatorText("CONTROLES");
			Spacing();

			Text("Zoom");
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

			static bool hide;
			if (Checkbox("Ocultar", &hide)) 
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

	void interface_input(const sapp_event *e)
	{

	}

	void interface_cleanup(void)
	{

	}

	inline void  interface_render(void)
	{
		simgui_render();
	}
}