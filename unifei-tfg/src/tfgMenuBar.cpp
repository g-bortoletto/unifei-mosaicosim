#include "tfgMenuBar.h"

#include "tfgProgram.h"

#include <imgui/imgui.h>
using namespace ImGui;

void MenuBar::FileMenu()
{
	if (BeginMenu("Arquivo"))
	{
		if (MenuItem("Novo projeto", "CTRL+N"))
		{
		}

		if (MenuItem("Abrir projeto", "CTRL+O"))
		{
			program.Load();
		}

		if (MenuItem("Salvar projeto", "CTRL+S"))
		{
			program.Save();
		}

		if (MenuItem("Fechar projeto"))
		{
		}

		Separator();

		if (MenuItem("Sair", "ALT+F4"))
		{
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

	EndMainMenuBar();
}

void MenuBar::Cleanup()
{
}

void MenuBar::Input(const sapp_event *e)
{
}
