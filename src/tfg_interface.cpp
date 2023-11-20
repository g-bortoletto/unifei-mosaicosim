#include <stdio.h>

#pragma clang diagnostic ignored "-Waddress-of-temporary"
#pragma clang diagnostic ignored "-Wc99-designator"
#pragma clang diagnostic ignored "-Wreorder-init-list"

#include "../inc/imgui/imgui.h"

#include "tfg_types.h"
#include "../inc/sokol/util/sokol_imgui.h"

#include "../res/fonts/segoeui.h"
#include "../res/fonts/fontawesome.h"
#include "../res/fonts/fontawesome85.h"
#include "../res/fonts/fontawesomenc.h"
#include "../inc/icons/icons_font_awesome.h"

using namespace ImGui;

namespace mosaico_sim
{
	extern mosaico_sim_state state;

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
		ImFontConfig font_main_cfg;
		font_main_cfg.FontDataOwnedByAtlas = false;
		font_main_cfg.OversampleH = 2;
		font_main_cfg.OversampleV = 2;
		font_main_cfg.RasterizerMultiply = 1.0f;
		state.font_main = io.Fonts->AddFontFromMemoryCompressedTTF(
			segoeui_compressed_data,
			segoeui_compressed_size,
			state.font_size,
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

		state.interface.control_bar.rect =
		{
			.x = 0.0f,
			.y = 0.0f,
			.w = 300.0f,
			.h = sapp_heightf(),
		};
		state.interface.control_bar.button_padding = 5.0f;
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
			state.interface.menu_bar.h = GetWindowHeight();
			state.interface.control_bar.rect.y = state.interface.menu_bar.h;
			state.interface.control_bar.rect.h = sapp_heightf() - state.interface.control_bar.rect.y;

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
					if (MenuItem("Clássico", 0))
					{
						// Microsoft style by usernameiwantedwasalreadytaken from ImThemes
						style.Alpha = 1.0f;
						style.DisabledAlpha = 0.6000000238418579f;
						style.WindowPadding = ImVec2(4.0f, 6.0f);
						style.WindowRounding = 0.0f;
						style.WindowBorderSize = 0.0f;
						style.WindowMinSize = ImVec2(32.0f, 32.0f);
						style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
						style.WindowMenuButtonPosition = ImGuiDir_Left;
						style.ChildRounding = 0.0f;
						style.ChildBorderSize = 1.0f;
						style.PopupRounding = 0.0f;
						style.PopupBorderSize = 1.0f;
						style.FramePadding = ImVec2(8.0f, 6.0f);
						style.FrameRounding = 0.0f;
						style.FrameBorderSize = 1.0f;
						style.ItemSpacing = ImVec2(8.0f, 6.0f);
						style.ItemInnerSpacing = ImVec2(8.0f, 6.0f);
						style.CellPadding = ImVec2(4.0f, 2.0f);
						style.IndentSpacing = 20.0f;
						style.ColumnsMinSpacing = 6.0f;
						style.ScrollbarSize = 20.0f;
						style.ScrollbarRounding = 0.0f;
						style.GrabMinSize = 5.0f;
						style.GrabRounding = 0.0f;
						style.TabRounding = 4.0f;
						style.TabBorderSize = 0.0f;
						style.TabMinWidthForCloseButton = 0.0f;
						style.ColorButtonPosition = ImGuiDir_Right;
						style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
						style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

						style.Colors[ImGuiCol_Text] = ImVec4(0.09803921729326248f, 0.09803921729326248f, 0.09803921729326248f, 1.0f);
						style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.4980392158031464f, 0.4980392158031464f, 0.4980392158031464f, 1.0f);
						style.Colors[ImGuiCol_WindowBg] = ImVec4(0.9490196108818054f, 0.9490196108818054f, 0.9490196108818054f, 1.0f);
						style.Colors[ImGuiCol_ChildBg] = ImVec4(0.9490196108818054f, 0.9490196108818054f, 0.9490196108818054f, 1.0f);
						style.Colors[ImGuiCol_PopupBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
						style.Colors[ImGuiCol_Border] = ImVec4(0.6000000238418579f, 0.6000000238418579f, 0.6000000238418579f, 1.0f);
						style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
						style.Colors[ImGuiCol_FrameBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
						style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.0f, 0.4666666686534882f, 0.8392156958580017f, 0.2000000029802322f);
						style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.0f, 0.4666666686534882f, 0.8392156958580017f, 1.0f);
						style.Colors[ImGuiCol_TitleBg] = ImVec4(0.03921568766236305f, 0.03921568766236305f, 0.03921568766236305f, 1.0f);
						style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.1568627506494522f, 0.2862745225429535f, 0.47843137383461f, 1.0f);
						style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 0.5099999904632568f);
						style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.8588235378265381f, 0.8588235378265381f, 0.8588235378265381f, 1.0f);
						style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.8588235378265381f, 0.8588235378265381f, 0.8588235378265381f, 1.0f);
						style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.686274528503418f, 0.686274528503418f, 0.686274528503418f, 1.0f);
						style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.0f, 0.0f, 0.0f, 0.2000000029802322f);
						style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.0f, 0.0f, 0.0f, 0.5f);
						style.Colors[ImGuiCol_CheckMark] = ImVec4(0.09803921729326248f, 0.09803921729326248f, 0.09803921729326248f, 1.0f);
						style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.686274528503418f, 0.686274528503418f, 0.686274528503418f, 1.0f);
						style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.0f, 0.0f, 0.0f, 0.5f);
						style.Colors[ImGuiCol_Button] = ImVec4(0.8588235378265381f, 0.8588235378265381f, 0.8588235378265381f, 1.0f);
						style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.0f, 0.4666666686534882f, 0.8392156958580017f, 0.2000000029802322f);
						style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.0f, 0.4666666686534882f, 0.8392156958580017f, 1.0f);
						style.Colors[ImGuiCol_Header] = ImVec4(0.8588235378265381f, 0.8588235378265381f, 0.8588235378265381f, 1.0f);
						style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.0f, 0.4666666686534882f, 0.8392156958580017f, 0.2000000029802322f);
						style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.0f, 0.4666666686534882f, 0.8392156958580017f, 1.0f);
						style.Colors[ImGuiCol_Separator] = ImVec4(0.4274509847164154f, 0.4274509847164154f, 0.4980392158031464f, 0.5f);
						style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.09803921729326248f, 0.4000000059604645f, 0.7490196228027344f, 0.7799999713897705f);
						style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.09803921729326248f, 0.4000000059604645f, 0.7490196228027344f, 1.0f);
						style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.2000000029802322f);
						style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.6700000166893005f);
						style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.949999988079071f);
						style.Colors[ImGuiCol_Tab] = ImVec4(0.1764705926179886f, 0.3490196168422699f, 0.5764706134796143f, 0.8619999885559082f);
						style.Colors[ImGuiCol_TabHovered] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.800000011920929f);
						style.Colors[ImGuiCol_TabActive] = ImVec4(0.196078434586525f, 0.407843142747879f, 0.6784313917160034f, 1.0f);
						style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.06666667014360428f, 0.1019607856869698f, 0.1450980454683304f, 0.9724000096321106f);
						style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1333333402872086f, 0.2588235437870026f, 0.4235294163227081f, 1.0f);
						style.Colors[ImGuiCol_PlotLines] = ImVec4(0.6078431606292725f, 0.6078431606292725f, 0.6078431606292725f, 1.0f);
						style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.4274509847164154f, 0.3490196168422699f, 1.0f);
						style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8980392217636108f, 0.6980392336845398f, 0.0f, 1.0f);
						style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.6000000238418579f, 0.0f, 1.0f);
						style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
						style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
						style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
						style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
						style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
						style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.3499999940395355f);
						style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.8999999761581421f);
						style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f);
						style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
						style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
						style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);
					}
					if (MenuItem("Escuro", 0))
					{
						StyleColorsDark(&style);
					}
					if (MenuItem("Moonlight", 0))
					{
						style.Alpha = 1.0f;
						style.DisabledAlpha = 1.0f;
						style.WindowPadding = ImVec2(12.0f, 12.0f);
						style.WindowRounding = 11.5f;
						style.WindowBorderSize = 0.0f;
						style.WindowMinSize = ImVec2(20.0f, 20.0f);
						style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
						style.WindowMenuButtonPosition = ImGuiDir_Right;
						style.ChildRounding = 0.0f;
						style.ChildBorderSize = 1.0f;
						style.PopupRounding = 0.0f;
						style.PopupBorderSize = 1.0f;
						style.FramePadding = ImVec2(20.0f, 3.400000095367432f);
						style.FrameRounding = 11.89999961853027f;
						style.FrameBorderSize = 0.0f;
						style.ItemSpacing = ImVec2(4.300000190734863f, 5.5f);
						style.ItemInnerSpacing = ImVec2(7.099999904632568f, 1.799999952316284f);
						style.CellPadding = ImVec2(12.10000038146973f, 9.199999809265137f);
						style.IndentSpacing = 0.0f;
						style.ColumnsMinSpacing = 4.900000095367432f;
						style.ScrollbarSize = 11.60000038146973f;
						style.ScrollbarRounding = 15.89999961853027f;
						style.GrabMinSize = 3.700000047683716f;
						style.GrabRounding = 20.0f;
						style.TabRounding = 0.0f;
						style.TabBorderSize = 0.0f;
						style.TabMinWidthForCloseButton = 0.0f;
						style.ColorButtonPosition = ImGuiDir_Right;
						style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
						style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

						style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
						style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.2745098173618317f, 0.3176470696926117f, 0.4509803950786591f, 1.0f);
						style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
						style.Colors[ImGuiCol_ChildBg] = ImVec4(0.09411764889955521f, 0.1019607856869698f, 0.1176470592617989f, 1.0f);
						style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
						style.Colors[ImGuiCol_Border] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
						style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
						style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1137254908680916f, 0.125490203499794f, 0.1529411822557449f, 1.0f);
						style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
						style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
						style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
						style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
						style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
						style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
						style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
						style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
						style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
						style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
						style.Colors[ImGuiCol_CheckMark] = ImVec4(0.9725490212440491f, 1.0f, 0.4980392158031464f, 1.0f);
						style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.9725490212440491f, 1.0f, 0.4980392158031464f, 1.0f);
						style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 0.7960784435272217f, 0.4980392158031464f, 1.0f);
						style.Colors[ImGuiCol_Button] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
						style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1803921610116959f, 0.1882352977991104f, 0.196078434586525f, 1.0f);
						style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.1529411822557449f, 0.1529411822557449f, 0.1529411822557449f, 1.0f);
						style.Colors[ImGuiCol_Header] = ImVec4(0.1411764770746231f, 0.1647058874368668f, 0.2078431397676468f, 1.0f);
						style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.105882354080677f, 0.105882354080677f, 0.105882354080677f, 1.0f);
						style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
						style.Colors[ImGuiCol_Separator] = ImVec4(0.1294117718935013f, 0.1490196138620377f, 0.1921568661928177f, 1.0f);
						style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
						style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
						style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1450980454683304f, 1.0f);
						style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.9725490212440491f, 1.0f, 0.4980392158031464f, 1.0f);
						style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
						style.Colors[ImGuiCol_Tab] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
						style.Colors[ImGuiCol_TabHovered] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
						style.Colors[ImGuiCol_TabActive] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
						style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
						style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.125490203499794f, 0.2745098173618317f, 0.572549045085907f, 1.0f);
						style.Colors[ImGuiCol_PlotLines] = ImVec4(0.5215686559677124f, 0.6000000238418579f, 0.7019608020782471f, 1.0f);
						style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.03921568766236305f, 0.9803921580314636f, 0.9803921580314636f, 1.0f);
						style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8823529481887817f, 0.7960784435272217f, 0.5607843399047852f, 1.0f);
						style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.95686274766922f, 0.95686274766922f, 0.95686274766922f, 1.0f);
						style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
						style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
						style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
						style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
						style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
						style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.9372549057006836f, 0.9372549057006836f, 0.9372549057006836f, 1.0f);
						style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
						style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2666666805744171f, 0.2901960909366608f, 1.0f, 1.0f);
						style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
						style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f);
						style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f);
					}
					if (MenuItem("Soft Cherry", 0))
					{
						style.Alpha = 1.0f;
						style.DisabledAlpha = 0.4000000059604645f;
						style.WindowPadding = ImVec2(10.0f, 10.0f);
						style.WindowRounding = 4.0f;
						style.WindowBorderSize = 0.0f;
						style.WindowMinSize = ImVec2(50.0f, 50.0f);
						style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
						style.WindowMenuButtonPosition = ImGuiDir_Left;
						style.ChildRounding = 0.0f;
						style.ChildBorderSize = 1.0f;
						style.PopupRounding = 1.0f;
						style.PopupBorderSize = 1.0f;
						style.FramePadding = ImVec2(5.0f, 3.0f);
						style.FrameRounding = 3.0f;
						style.FrameBorderSize = 0.0f;
						style.ItemSpacing = ImVec2(6.0f, 6.0f);
						style.ItemInnerSpacing = ImVec2(3.0f, 2.0f);
						style.CellPadding = ImVec2(3.0f, 3.0f);
						style.IndentSpacing = 6.0f;
						style.ColumnsMinSpacing = 6.0f;
						style.ScrollbarSize = 13.0f;
						style.ScrollbarRounding = 16.0f;
						style.GrabMinSize = 20.0f;
						style.GrabRounding = 4.0f;
						style.TabRounding = 4.0f;
						style.TabBorderSize = 1.0f;
						style.TabMinWidthForCloseButton = 0.0f;
						style.ColorButtonPosition = ImGuiDir_Right;
						style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
						style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

						style.Colors[ImGuiCol_Text] = ImVec4(0.8588235378265381f, 0.929411768913269f, 0.886274516582489f, 1.0f);
						style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.5215686559677124f, 0.5490196347236633f, 0.5333333611488342f, 1.0f);
						style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1294117718935013f, 0.1372549086809158f, 0.168627455830574f, 1.0f);
						style.Colors[ImGuiCol_ChildBg] = ImVec4(0.1490196138620377f, 0.1568627506494522f, 0.1882352977991104f, 1.0f);
						style.Colors[ImGuiCol_PopupBg] = ImVec4(0.2000000029802322f, 0.2196078449487686f, 0.2666666805744171f, 1.0f);
						style.Colors[ImGuiCol_Border] = ImVec4(0.1372549086809158f, 0.1137254908680916f, 0.1333333402872086f, 1.0f);
						style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
						style.Colors[ImGuiCol_FrameBg] = ImVec4(0.168627455830574f, 0.1843137294054031f, 0.2313725501298904f, 1.0f);
						style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
						style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
						style.Colors[ImGuiCol_TitleBg] = ImVec4(0.2313725501298904f, 0.2000000029802322f, 0.2705882489681244f, 1.0f);
						style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.501960813999176f, 0.07450980693101883f, 0.2549019753932953f, 1.0f);
						style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.2000000029802322f, 0.2196078449487686f, 0.2666666805744171f, 1.0f);
						style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.2000000029802322f, 0.2196078449487686f, 0.2666666805744171f, 1.0f);
						style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.239215686917305f, 0.239215686917305f, 0.2196078449487686f, 1.0f);
						style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.3882353007793427f, 0.3882353007793427f, 0.3725490272045135f, 1.0f);
						style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.6941176652908325f, 0.6941176652908325f, 0.686274528503418f, 1.0f);
						style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.6941176652908325f, 0.6941176652908325f, 0.686274528503418f, 1.0f);
						style.Colors[ImGuiCol_CheckMark] = ImVec4(0.658823549747467f, 0.1372549086809158f, 0.1764705926179886f, 1.0f);
						style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.6509804129600525f, 0.1490196138620377f, 0.3450980484485626f, 1.0f);
						style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.7098039388656616f, 0.2196078449487686f, 0.2666666805744171f, 1.0f);
						style.Colors[ImGuiCol_Button] = ImVec4(0.6509804129600525f, 0.1490196138620377f, 0.3450980484485626f, 1.0f);
						style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
						style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
						style.Colors[ImGuiCol_Header] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
						style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.6509804129600525f, 0.1490196138620377f, 0.3450980484485626f, 1.0f);
						style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.501960813999176f, 0.07450980693101883f, 0.2549019753932953f, 1.0f);
						style.Colors[ImGuiCol_Separator] = ImVec4(0.4274509847164154f, 0.4274509847164154f, 0.4980392158031464f, 1.0f);
						style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.09803921729326248f, 0.4000000059604645f, 0.7490196228027344f, 1.0f);
						style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.09803921729326248f, 0.4000000059604645f, 0.7490196228027344f, 1.0f);
						style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.6509804129600525f, 0.1490196138620377f, 0.3450980484485626f, 1.0f);
						style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
						style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
						style.Colors[ImGuiCol_Tab] = ImVec4(0.1764705926179886f, 0.3490196168422699f, 0.5764706134796143f, 1.0f);
						style.Colors[ImGuiCol_TabHovered] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f);
						style.Colors[ImGuiCol_TabActive] = ImVec4(0.196078434586525f, 0.407843142747879f, 0.6784313917160034f, 1.0f);
						style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.06666667014360428f, 0.1019607856869698f, 0.1450980454683304f, 1.0f);
						style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1333333402872086f, 0.2588235437870026f, 0.4235294163227081f, 1.0f);
						style.Colors[ImGuiCol_PlotLines] = ImVec4(0.8588235378265381f, 0.929411768913269f, 0.886274516582489f, 1.0f);
						style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
						style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.3098039329051971f, 0.7764706015586853f, 0.196078434586525f, 1.0f);
						style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
						style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
						style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
						style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
						style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
						style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
						style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.3843137323856354f, 0.6274510025978088f, 0.9176470637321472f, 1.0f);
						style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
						style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f);
						style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
						style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 1.0f);
						style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.300000011920929f);
					}
					EndMenu();
				}

				if (BeginMenu("Tamanho da fonte"))
				{
					int step = 1;
					if (InputScalar("##font_size", ImGuiDataType_U32, &state.font_size, &step))
					{
						if (state.font_size < 8) { state.font_size = 8; }
						ImGuiIO &io = GetIO();
						io.FontGlobalScale = state.font_size / 16.0f;
					}
					EndMenu();
				}
				EndMenu();
			}

			EndMainMenuBar();
		}

		state.interface.control_bar.flags = ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoCollapse
			//| ImGuiWindowFlags_NoBackground
			| ImGuiWindowFlags_NoTitleBar;
		SetNextWindowPos(ImVec2(state.interface.control_bar.rect.x, state.interface.control_bar.rect.y));
		SetNextWindowSizeConstraints(
			ImVec2(250.0f, state.interface.control_bar.rect.h), 
			ImVec2(sapp_widthf() * 0.5f, state.interface.control_bar.rect.h));
		if (Begin("Barra de Controle", &state.interface.control_bar.show, state.interface.control_bar.flags))
		{
			state.interface.control_bar.rect.w = GetWindowWidth();
			float button_size = state
				.interface
				.control_bar
				.rect
				.w
				- 3.0f
				* state.interface.control_bar.button_padding;

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
		if (Begin("idk"))
		{
			Text("%s ", ICON_FA_GLASS); SameLine();
			Text("%s ", ICON_FA_MUSIC); SameLine();
			Text("%s ", ICON_FA_SEARCH); SameLine();
			Text("%s ", ICON_FA_ENVELOPE_O); SameLine();
			Text("%s ", ICON_FA_HEART); SameLine();
			Text("%s ", ICON_FA_STAR); SameLine();
			Text("%s ", ICON_FA_STAR_O); SameLine();
			Text("%s ", ICON_FA_USER); SameLine();
			Text("%s ", ICON_FA_FILM); SameLine();
			Text("%s ", ICON_FA_TH_LARGE); SameLine();
			Text("%s ", ICON_FA_TH); SameLine();
			Text("%s ", ICON_FA_TH_LIST); SameLine();
			Text("%s ", ICON_FA_CHECK); SameLine();
			Text("%s ", ICON_FA_TIMES); SameLine();
			Text("%s ", ICON_FA_SEARCH_PLUS); SameLine();
			Text("%s ", ICON_FA_SEARCH_MINUS); SameLine();
			Text("%s ", ICON_FA_POWER_OFF); SameLine();
			Text("%s ", ICON_FA_SIGNAL); SameLine();
			Text("%s ", ICON_FA_COG); SameLine();
			Text("%s ", ICON_FA_TRASH_O); SameLine();
			Text("%s ", ICON_FA_HOME); SameLine();
			Text("%s ", ICON_FA_FILE_O); SameLine();
			Text("%s ", ICON_FA_CLOCK_O); SameLine();
			Text("%s ", ICON_FA_ROAD); SameLine();
			Text("%s ", ICON_FA_DOWNLOAD); SameLine();
			Text("%s ", ICON_FA_ARROW_CIRCLE_O_DOWN); SameLine();
			Text("%s ", ICON_FA_ARROW_CIRCLE_O_UP); SameLine();
			Text("%s ", ICON_FA_INBOX); SameLine();
			Text("%s ", ICON_FA_PLAY_CIRCLE_O); SameLine();
			Text("%s ", ICON_FA_REPEAT); SameLine();
			Text("%s ", ICON_FA_REFRESH); SameLine();
			Text("%s ", ICON_FA_LIST_ALT); SameLine();
			Text("%s ", ICON_FA_LOCK); SameLine();
			Text("%s ", ICON_FA_FLAG); SameLine();
			Text("%s ", ICON_FA_HEADPHONES); SameLine();
			Text("%s ", ICON_FA_VOLUME_OFF); SameLine();
			Text("%s ", ICON_FA_VOLUME_DOWN); SameLine();
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