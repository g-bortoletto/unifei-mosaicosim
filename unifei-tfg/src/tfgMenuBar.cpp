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
	if (Begin("Sobre##help", &showHelp, ImGuiWindowFlags_NoCollapse))
	{
		ImVec4 corDestaque(
			35.0f / 255.0f, 
			108.0f / 255.0f, 
			208.0f / 255.0f,
			1.0f);

		SeparatorText("INSTRUÇÕES");

		Text("%s", "O MosaicoSIM serve para simular"
			"a criação de um mosaico artístico.");
		if (TreeNode("Processo"))
		{
			TextColored(corDestaque, "%s", "1.");
			SameLine();
			Text("Carregar uma imagem de referência. "
				"Ela aparecerá como o fundo do programa\n"
				"e tem a finalidade de ser utilizada como um modelo"
				" a ser preenchido por peças.");

			TextColored(corDestaque, "%s", "2.");
			SameLine();
			Text("Criar peças e manipulá-las de maneira a formar um mosaico.");

			TextColored(corDestaque, "%s", "3.");
			SameLine();
			Text("Escolher uma cor para o rejunte.");

			TreePop();
		}

		if (TreeNode("Controles"))
		{
			if (TreeNode("Barra de menu superior"))
			{
				if (TreeNode("Arquivo"))
				{
					TextColored(corDestaque, "Novo projeto:"); SameLine();
					Text("iniciar um novo projeto."); Spacing();

					TextColored(corDestaque, "Abrir projeto:"); SameLine();
					Text("abrir um projeto do disco."); Spacing();

					TextColored(corDestaque, "Salvar projeto:"); SameLine();
					Text("salvar projeto atual no disco."); Spacing();

					TextColored(corDestaque, "Sair:"); SameLine();
					Text("fecha o programa."); Spacing();

					TreePop();
				}

				if (TreeNode("Editar"))
				{
					TextColored(corDestaque, "Selecionar tudo:"); SameLine();
					Text("selecionar todas as peças."); Spacing();

					TextColored(corDestaque, "Desfazer:"); SameLine();
					Text("desfazer última ação."); Spacing();

					TextColored(corDestaque, "Refazer:"); SameLine();
					Text("refazer última ação."); Spacing();

					TextColored(corDestaque, "Recortar:"); SameLine();
					Text("recortar peças selecionadas."); Spacing();

					TextColored(corDestaque, "Copiar:"); SameLine();
					Text("copiar peças selecionadas."); Spacing();

					TextColored(corDestaque, "Colar:"); SameLine();
					Text("colar peças selecionadas."); Spacing();

					TextColored(corDestaque, "Duplicar:"); SameLine();
					Text("duplicar peças selecionadas."); Spacing();

					TextColored(corDestaque, "Alterar cor de fundo:"); SameLine();
					Text("permite alterar a cor do fundo da área de trabalho."); Spacing();
					
					TreePop();
				}

				if (TreeNode("Opções"))
				{
					TextColored(corDestaque, "Tema da interface:"); SameLine();
					Text("permite selecionar que a interface seja clara ou escura."); Spacing();

					TextColored(corDestaque, "Tamanho da fonte:"); SameLine();
					Text("permite aumentar ou diminuir o tamanho da letra\ndo programa."); Spacing();

					TreePop();
				}

				if (TreeNode("Ajuda"))
				{
					TextColored(corDestaque, "Comandos:"); SameLine();
					Text("Abre esta janela."); Spacing();

					TextColored(corDestaque, "Sobre:"); SameLine();
					Text("Mostra informações sobre o autor do programa."); Spacing();

					TreePop();
				}

				TreePop();
			}
			if (TreeNode("Barra de controle lateral esquerda"))
			{
				if (TreeNode("Imagem"))
				{
					TextColored(corDestaque, "[Adicionar]");
					Text("Abre uma janela para adicionar uma imagem do disco\n"
						"como referência no fundo do programa.");
					Spacing();
					
					TextColored(corDestaque, "[Remover]");
					Text("Remove a imagem atual do fundo do programa.");
					Spacing();
					
					TextColored(corDestaque, "[ ] Ocultar");
					Text("Oculta a imagem do fundo do programa.");
					TreePop();
				}
				if (TreeNode("Peça"))
				{
					TextColored(corDestaque, "[R][G][B] Cor");
					Text("Seleciona a cor da peça selecionada ou que será criada.\n"
						"\t- R representa a quantidade de vermelho da cor. Varia de 0 a 255.\n"
						"\t- G representa a quantidade de verde da cor. Varia de 0 a 255.\n"
						"\t- B representa a quantidade de azul da cor. Varia de 0 a 255.\n"
						"Clicar no quadrado de previsão de cor abrirá uma janela de seleção\n"
						" que possibilita a escolha visual da cor com precisão.\n"
						"OBS: É possível clicar nas caixas de R, G, e B e arrastar o mouse\n"
						"para a esquerda e direita para modificar seu valor.");
					Spacing();
					
					TextColored(corDestaque, "[Número de lados][-][+]");
					Text("representa o número de lados da peça a ser criada.\n"
						"Clique no [-] para diminuir e no [+] para aumentar.\nOs "
						"valores variam de 3 a 9.");
					Spacing();
					
					TextColored(corDestaque, "[Adicionar]");
					Text("Cria uma peça no centro da tela com as configuraçãoes\n"
						"selecionadas anteriormente.");
					Spacing();
					
					TextColored(corDestaque, "[Remover]");
					Text("Remove a(s) peça(s) selecionada da tela.");
					Spacing();
					
					TextColored(corDestaque, "[Limpar]");
					Text("Remove todas as peças da tela.");
					TreePop();
				}

				if (TreeNode("Rejunte"))
				{
					TextColored(corDestaque, "[ ] Sobrepor imagem de referência");
					Text("Coloca uma camada colorida entre o fundo e as peças,\n"
						"simulando um rejunte. Pode ser ligado ou desligado.");
					Spacing();

					TextColored(corDestaque, "[R][G][B] Cor");
					Text("Seleciona a cor do rejunte que será exibida.\n"
						"\t- R representa a quantidade de vermelho da cor. Varia de 0 a 255.\n"
						"\t- G representa a quantidade de verde da cor. Varia de 0 a 255.\n"
						"\t- B representa a quantidade de azul da cor. Varia de 0 a 255.\n"
						"Clicar no quadrado de previsão de cor abrirá uma janela de seleção\n"
						" que possibilita a escolha visual da cor com precisão.\n"
						"OBS: É possível clicar nas caixas de R, G, e B e arrastar o mouse\n"
						"para a esquerda e direita para modificar seu valor.");
					TreePop();
				}
				TreePop();
			}

			if (TreeNode("Mouse"))
			{
				TextColored(corDestaque, "Botão esquerdo");
				Text("Realiza a maioria das interações do programa:\n"
					"\t- Clicar em cima de uma peça a tornará selecionada. A\n"
					"\t  seleção é indicada por um contorno em volta da peça.\n"
					"\t- Segurar a tecla SHIFT ao clicar possibilita selecionar\n"
					"\t  mais de uma peça ao mesmo tempo.\n"
					"\t- Quando há uma ou mais peças seleciondas, manter\n"
					"\t  pressionado o botão esquerdo as movimentará.");
				Text("OBS: é possível manipular os pontos de uma peça\n"
					"selecionada para alterar seu formato com estes mesmos\n"
					"controles.");
				Spacing();

				TextColored(corDestaque, "Botão direito");
				Text("Manter o botão direito pressionado movimenta a câmera,\n"
					"possibilitando visualizar regiões da imagem que não\n"
					"estão na tela.");
				Spacing();

				TextColored(corDestaque, "Scroll");
				Text("O scroll (ou rodinha) do mouse tem três funcionalidades:\n"
					"\t- Pressione o scroll para redefinir o zoom e câmera para\n"
					"\t  o padrão do programa.\n"
					"\t- Girar a rodinha irá aproximar ou afastar a câmera,\n"
					"\t  deixando a zona de trabalho maior ou menor.\n"
					"\t- Posicionar o cursor do mouse em cima de uma peça e girar\n"
					"\t  o scroll com a tecla SHIFT pressionada, aumentará ou\n"
					"\t  diminuirá seu tamanho.");
				TreePop();
			}

			if (TreeNode("Teclado"))
			{
				TextColored(corDestaque, "[CTRL]+N:"); SameLine();
				Text("iniciar um novo projeto."); Spacing();
				
				TextColored(corDestaque, "[CTRL]+O:"); SameLine();
				Text("abrir um projeto do disco."); Spacing();

				TextColored(corDestaque, "[CTRL]+S:"); SameLine();
				Text("salvar projeto atual no disco."); Spacing();

				TextColored(corDestaque, "[CTRL]+A:"); SameLine();
				Text("selecionar todas as peças."); Spacing();

				TextColored(corDestaque, "[CTRL]+Z:"); SameLine();
				Text("desfazer última ação."); Spacing();

				TextColored(corDestaque, "[CTRL]+Y:"); SameLine();
				Text("refazer última ação."); Spacing();

				TextColored(corDestaque, "[CTRL]+X:"); SameLine();
				Text("recortar peças selecionadas."); Spacing();

				TextColored(corDestaque, "[CTRL]+C:"); SameLine();
				Text("copiar peças selecionadas."); Spacing();

				TextColored(corDestaque, "[CTRL]+V:"); SameLine();
				Text("colar peças selecionadas."); Spacing();

				TextColored(corDestaque, "[CTRL]+D:"); SameLine();
				Text("duplicar peças selecionadas."); Spacing();

				TreePop();
			}
			TreePop();
		}
		End();
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