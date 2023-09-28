#include "tfgMenuBar.h"

#include "tfgProgram.h"
#include "tfgBackgroundImage.h"

#include <imgui/imgui.h>
using namespace ImGui;

#include <sokol/sokol_gfx.h>
#include <sokol/sokol_app.h>

void MenuBar::FileMenu()
{
	if (BeginMenu("Arquivo"))
	{
		if (MenuItem("Novo projeto", "CTRL+N"))
		{
			program.NewProject();
		}

		if (MenuItem("Abrir projeto", "CTRL+O"))
		{
			program.Load();
		}

		if (MenuItem("Salvar projeto", "CTRL+S"))
		{
			program.Save();
		}

		Separator();

		if (MenuItem("Sair", "ALT+F4"))
		{
			sapp_quit();
		}

		EndMenu();
	}
}

void MenuBar::EditMenu()
{
	if (BeginMenu("Editar"))
	{
		if (MenuItem("Desfazer", "CTRL+Z"))
		{
			program.Undo();
		}

		if (MenuItem("Refazer", "CTRL+Y"))
		{
			program.Redo();
		}

		Separator();

		if (MenuItem("Recortar", "CTRL+X"))
		{
			program.Copy();
			program.DestroyShape();
		}

		if (MenuItem("Copiar", "CTRL+C"))
		{
			program.Copy();
		}


		if (MenuItem("Colar", "CTRL+V"))
		{
			program.Paste();
		}

		Separator();

		if (ColorEdit4("Alterar cor de fundo", &program.clearColor.r, ImGuiColorEditFlags_PickerHueWheel))
		{
		}

		EndMenu();
	}
}

void MenuBar::SettingsMenu()
{
	if (BeginMenu("Opções"))
	{
		if (BeginMenu("Tema da interface"))
		{
			if (MenuItem("Claro", 0, !program.darkMode))
			{
				SetLightTheme();
			}
			if (MenuItem("Escuro", 0, &program.darkMode))
			{
				SetDarkTheme();
			}
			EndMenu();
		}
		if (BeginMenu("Tamanho da fonte"))
		{
			int step = 1;
			if (InputScalar("##fontSize", ImGuiDataType_U32, &program.fontSize, &step))
			{
				UpdateFontSize();
			}
			EndMenu();
		}
		EndMenu();
	}
}

void MenuBar::AboutMenu()
{
	if (BeginMenu("Ajuda"))
	{
		if (MenuItem("Comandos"))
		{

		}

		if (MenuItem("Sobre##menu"))
		{
			showAboutMenu = true;
		}
		EndMenu();
	}

	if (showAboutMenu)
	{
		OpenPopup("Sobre##about");
	}

	ImVec2 center = GetMainViewport()->GetCenter();
	SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	if (BeginPopupModal("Sobre##about", 0, ImGuiWindowFlags_AlwaysAutoResize))
	{
		SeparatorText("MOSAICOSIM");
		Text("Autor: Guilherme Malaquias Bortoletto");
		Text("Contato: gmb_er@unifei.edu.br");
		SetItemDefaultFocus();
		Spacing();
		Spacing();
		Spacing();
		Spacing();
		if (ButtonCenteredOnLine("Fechar"))
		{
			CloseCurrentPopup();
			showAboutMenu = false;
		}
		EndPopup();
	}
}

void MenuBar::UpdateFontSize()
{
	if (program.fontSize < 8) { program.fontSize = 8; }
	ImGuiIO &io = GetIO();
	io.FontGlobalScale = program.fontSize / 16.0f;
}

void MenuBar::SetDarkTheme()
{
	ImGuiStyle &style = GetStyle();
	StyleColorsDark(&style);
}

void MenuBar::SetLightTheme()
{
	if (program.darkMode)
	{
		ImGuiStyle &style = GetStyle();
		StyleColorsLight(&style);
	}
	program.darkMode = false;
}

void MenuBar::Init()
{
}

void MenuBar::Frame()
{
	BeginMainMenuBar();

	height = GetWindowHeight();

	FileMenu();
	EditMenu();
	SettingsMenu();
	AboutMenu();

	EndMainMenuBar();
}

void MenuBar::Cleanup()
{
}

void MenuBar::Input(const sapp_event *e)
{
}

bool MenuBar::ButtonCenteredOnLine(const char *label, float alignment)
{
	ImGuiStyle &style = ImGui::GetStyle();

	float size = ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f;
	float avail = ImGui::GetContentRegionAvail().x;

	float off = (avail - size) * alignment;
	if (off > 0.0f)
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

	return ImGui::Button(label);
}