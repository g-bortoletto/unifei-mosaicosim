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
		if (MenuItem("Selecionar tudo", "CTRL+A"))
		{
			program.SelectAll();
		}

		Separator();

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

		if (MenuItem("Duplicar", "CTRL+D"))
		{
			program.Copy();
			program.Paste();
		}

		Separator();

		if (ColorEdit3("Alterar cor de fundo", &program.clearColor.r, ImGuiColorEditFlags_PickerHueWheel))
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
		if (MenuItem("Instruções"))
		{
			showHelp = true;
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

	if (showHelp)
	{
		OpenPopup("Sobre##help");
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

	if (!showHelp) return;
	SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	SetNextWindowSize({ 500, 480 }, ImGuiCond_Appearing);
	if (BeginPopupModal("Sobre##help", &showHelp, 
		ImGuiWindowFlags_NoCollapse))
	{
		ImVec4 corDestaque(
			35.0f / 255.0f, 
			108.0f / 255.0f, 
			208.0f / 255.0f,
			1.0f);

		SeparatorText("INSTRUÇÕES");

		TextWrapped(
			"O MosaicoSIM serve para simular a "
			"criação de um mosaico artístico.");

		if (TreeNode("Processo"))
		{
			TextColored(corDestaque, "%s", "1.");
			SameLine();
			TextWrapped(
				"Carregar uma imagem de referência. "
				"Ela aparecerá como o fundo do programa "
				"e tem a finalidade de ser utilizada como um modelo "
				"a ser preenchido por peças.");

			TextColored(corDestaque, "%s", "2.");
			SameLine();
			TextWrapped(
				"Criar peças e manipulá-las de maneira "
				"a formar um mosaico.");

			TextColored(corDestaque, "%s", "3.");
			SameLine();
			TextWrapped("Escolher uma cor para o rejunte.");

			TreePop();
		}

		if (TreeNode("Controles"))
		{
			if (TreeNode("Barra de menu superior"))
			{
				if (TreeNode("Arquivo"))
				{
					TextColored(corDestaque, "Novo projeto:"); SameLine();
					TextWrapped("iniciar um novo projeto."); Spacing();

					TextColored(corDestaque, "Abrir projeto:"); SameLine();
					TextWrapped("abrir um projeto do disco."); Spacing();

					TextColored(corDestaque, "Salvar projeto:"); SameLine();
					TextWrapped("salvar projeto atual no disco."); Spacing();

					TextColored(corDestaque, "Sair:"); SameLine();
					TextWrapped("fecha o programa."); Spacing();

					TreePop();
				}

				if (TreeNode("Editar"))
				{
					TextColored(corDestaque, "Selecionar tudo:"); SameLine();
					TextWrapped("selecionar todas as peças."); Spacing();

					TextColored(corDestaque, "Desfazer:"); SameLine();
					TextWrapped("desfazer última ação."); Spacing();

					TextColored(corDestaque, "Refazer:"); SameLine();
					TextWrapped("refazer última ação."); Spacing();

					TextColored(corDestaque, "Recortar:"); SameLine();
					TextWrapped("recortar peças selecionadas."); Spacing();

					TextColored(corDestaque, "Copiar:"); SameLine();
					TextWrapped("copiar peças selecionadas."); Spacing();

					TextColored(corDestaque, "Colar:"); SameLine();
					TextWrapped("colar peças selecionadas."); Spacing();

					TextColored(corDestaque, "Duplicar:"); SameLine();
					TextWrapped("duplicar peças selecionadas."); Spacing();

					TextColored(corDestaque, "Alterar cor de fundo:"); SameLine();
					TextWrapped("permite alterar a cor do fundo da área "
						"de trabalho."); Spacing();
					
					TreePop();
				}

				if (TreeNode("Opções"))
				{
					TextColored(corDestaque, "Tema da interface:"); SameLine();
					TextWrapped("permite selecionar que a interface "
						"seja clara ou escura."); Spacing();

					TextColored(corDestaque, "Tamanho da fonte:"); SameLine();
					TextWrapped("permite aumentar ou diminuir o tamanho "
						"da letra do programa."); Spacing();

					TreePop();
				}

				if (TreeNode("Ajuda"))
				{
					TextColored(corDestaque, "Instruções:"); SameLine();
					TextWrapped("Abre esta janela."); Spacing();

					TextColored(corDestaque, "Sobre:"); SameLine();
					TextWrapped("Mostra informações sobre o autor do programa."); Spacing();

					TreePop();
				}

				TreePop();
			}
			if (TreeNode("Barra de controle lateral esquerda"))
			{
				if (TreeNode("Imagem"))
				{
					TextColored(corDestaque, "[Adicionar]");
					TextWrapped("Abre uma janela para adicionar uma imagem do disco "
						"como referência no fundo do programa.");
					Spacing();
					
					TextColored(corDestaque, "[Remover]");
					TextWrapped("Remove a imagem atual do fundo do programa.");
					Spacing();
					
					TextColored(corDestaque, "[ ] Ocultar");
					TextWrapped("Oculta a imagem do fundo do programa.");
					TreePop();
				}
				if (TreeNode("Peça"))
				{
					TextColored(corDestaque, "[R][G][B] Cor");
					TextWrapped("Seleciona a cor da peça selecionada ou que será criada. "
						"\n\t- R representa a quantidade de vermelho da cor. Varia de 0 a 255. "
						"\n\t- G representa a quantidade de verde da cor. Varia de 0 a 255. "
						"\n\t- B representa a quantidade de azul da cor. Varia de 0 a 255. "
						"\nClicar no quadrado de previsão de cor abrirá uma janela de seleção "
						"que possibilita a escolha visual da cor com precisão. "
						"\nOBS: É possível clicar nas caixas de R, G, e B e arrastar o mouse "
						"para a esquerda e direita para modificar seu valor.");
					Spacing();
					
					TextColored(corDestaque, "[Número de lados][-][+]");
					TextWrapped("representa o número de lados da peça a ser criada. "
						"Clique no [-] para diminuir e no [+] para aumentar. Os "
						"valores variam de 3 a 9.");
					Spacing();
					
					TextColored(corDestaque, "[Adicionar]");
					TextWrapped("Cria uma peça no centro da tela com as configuraçãoes "
						"selecionadas anteriormente.");
					Spacing();
					
					TextColored(corDestaque, "[Remover]");
					TextWrapped("Remove a(s) peça(s) selecionada da tela.");
					Spacing();
					
					TextColored(corDestaque, "[Limpar]");
					TextWrapped("Remove todas as peças da tela.");
					TreePop();
				}

				if (TreeNode("Rejunte"))
				{
					TextColored(corDestaque, "[ ] Sobrepor imagem de referência");
					TextWrapped("Coloca uma camada colorida entre o fundo e as peças, "
						"simulando um rejunte. Pode ser ligado ou desligado.");
					Spacing();

					TextColored(corDestaque, "[R][G][B] Cor");
					TextWrapped("Seleciona a cor do rejunte que será exibida. "
						"\n\t- R representa a quantidade de vermelho da cor. Varia de 0 a 255. "
						"\n\t- G representa a quantidade de verde da cor. Varia de 0 a 255. "
						"\n\t- B representa a quantidade de azul da cor. Varia de 0 a 255. "
						"\nClicar no quadrado de previsão de cor abrirá uma janela de seleção "
						"que possibilita a escolha visual da cor com precisão."
						"\nOBS: É possível clicar nas caixas de R, G, e B e arrastar o mouse "
						"para a esquerda e direita para modificar seu valor.");
					TreePop();
				}
				TreePop();
			}

			if (TreeNode("Mouse"))
			{
				TextColored(corDestaque, "Botão esquerdo");
				TextWrapped("Realiza a maioria das interações do programa: "
					"\n\t- Clicar em cima de uma peça a tornará selecionada. A "
					"seleção é indicada por um contorno em volta da peça. "
					"\n\t- Segurar a tecla SHIFT ao clicar possibilita selecionar "
					"mais de uma peça ao mesmo tempo. "
					"\n\t- Quando há uma ou mais peças seleciondas, manter "
					"pressionado o botão esquerdo as movimentará.");
				TextWrapped("\nOBS: é possível manipular os pontos de uma peça "
					"selecionada para alterar seu formato com estes mesmos "
					"controles.");
				Spacing();

				TextColored(corDestaque, "Botão direito");
				TextWrapped("Manter o botão direito pressionado movimenta a câmera, "
					"possibilitando visualizar regiões da imagem que não "
					"estão na tela.");
				Spacing();

				TextColored(corDestaque, "Scroll");
				TextWrapped("O scroll (ou rodinha) do mouse tem três funcionalidades: "
					"\n\t- Pressione o scroll para redefinir o zoom e câmera para "
					"o padrão do programa. "
					"\n\t- Girar a rodinha irá aproximar ou afastar a câmera, "
					"deixando a zona de trabalho maior ou menor. "
					"\n\t- Posicionar o cursor do mouse em cima de uma peça e girar "
					"o scroll com a tecla SHIFT pressionada, aumentará ou "
					"diminuirá seu tamanho.");
				TreePop();
			}

			if (TreeNode("Teclado"))
			{
				TextColored(corDestaque, "[CTRL]+N:"); SameLine();
				TextWrapped("iniciar um novo projeto."); Spacing();
				
				TextColored(corDestaque, "[CTRL]+O:"); SameLine();
				TextWrapped("abrir um projeto do disco."); Spacing();

				TextColored(corDestaque, "[CTRL]+S:"); SameLine();
				TextWrapped("salvar projeto atual no disco."); Spacing();

				TextColored(corDestaque, "[CTRL]+A:"); SameLine();
				TextWrapped("selecionar todas as peças."); Spacing();

				TextColored(corDestaque, "[CTRL]+Z:"); SameLine();
				TextWrapped("desfazer última ação."); Spacing();

				TextColored(corDestaque, "[CTRL]+Y:"); SameLine();
				TextWrapped("refazer última ação."); Spacing();

				TextColored(corDestaque, "[CTRL]+X:"); SameLine();
				TextWrapped("recortar peças selecionadas."); Spacing();

				TextColored(corDestaque, "[CTRL]+C:"); SameLine();
				TextWrapped("copiar peças selecionadas."); Spacing();

				TextColored(corDestaque, "[CTRL]+V:"); SameLine();
				TextWrapped("colar peças selecionadas."); Spacing();

				TextColored(corDestaque, "[CTRL]+D:"); SameLine();
				TextWrapped("duplicar peças selecionadas."); Spacing();

				TreePop();
			}
			TreePop();
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