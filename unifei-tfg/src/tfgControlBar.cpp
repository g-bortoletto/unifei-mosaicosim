#include "tfgControlBar.h"

#include "tfgProgram.h"
#include "tfgBackgroundImage.h"
#include "tfgShape.h"

#include <imgui/imgui.h>

using namespace ImGui;

ControlBar::ControlBar(Program &program) :
	program(program)
{
}

ControlBar::~ControlBar()
{
}

void ControlBar::Init()
{
}

static void InstructionText(const char *first, const char *second)
{
	TextColored(ImVec4(255.0f, 255.0f, 0.0f, 255.0f), "%s", first);
	SameLine();
	Text("%s", second);
}

void ControlBar::Frame()
{
	ImGuiWindowFlags barFlags = ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoCollapse
		//| ImGuiWindowFlags_NoBackground
		| ImGuiWindowFlags_NoTitleBar;
	SetNextWindowPos(ImVec2(0.0f, 0.0f));
	SetNextWindowSizeConstraints(
		ImVec2(250.0f, program.window.h), 
		ImVec2(program.window.w / 2, program.window.h));
	if (Begin("BarraControle", nullptr, barFlags))
	{
		width = GetWindowSize().x;
		buttonSize = width - 3.0f * buttonPadding;
		// update viewport size
		program.viewport = (Rect)
		{
			.x = width,
			.y = 0.0f,
			.w = program.window.w - width,
			.h = program.window.h,
		};

		static int vertices = 3;
		static unsigned char step = 1;

		if (Button("CARREGAR IMAGEM", ImVec2(buttonSize, 0.0f))) 
		{
			program.image->path.clear();
			program.LoadBackgroundImage();
		}

		SetNextItemWidth(inputScalarWidth);
		if (InputScalar("## ADD_PEÇA", ImGuiDataType_U8, &vertices, &step))
		{
			if (vertices < 3) vertices = 3;
			if (vertices > 9) vertices = 9;
		}
		SameLine();
		if (Button(
			"ADICIONAR PEÇA", 
			ImVec2(buttonSize - inputScalarWidth - 8.0f, 0.0f))) 
		{ 
			program.CreateShape(vertices); 
		}

		if (Button("REMOVER PEÇA", ImVec2(buttonSize, 0.0f))) 
		{
			program.DestroyShape();
		}
		if (Button("SALVAR PROJETO", ImVec2(buttonSize, 0.0f))) 
		{
			program.Save();
		}
		if (Button("CARREGAR PROJETO", ImVec2(buttonSize, 0.0f)))
		{
			program.Load();
		}

		if (!program.selectionList.empty())
		{
			SetNextItemWidth(buttonSize);
			ColorPicker4(
				"## COR PEÇA",
				&program
					.shapeList
					.find(*program.selectionList.begin())
					->second.color.r,
				ImGuiColorEditFlags_NoSidePreview);
		}

		if (Button("SOBRE", ImVec2(buttonSize, 0.0f)))
		{
			showAboutWindow = true;
		}

		if (showAboutWindow)
		{
			Begin("Sobre", &showAboutWindow);
			
			SeparatorText("Simulador de Mosaicos");
			Text("%s", "Desenvolvido por: Guilherme Bortoletto");
			Text("%s", "Contato: gmb_er@unifei.edu.br");
			SeparatorText("Controles");
			InstructionText("Copiar:", "CTRL+C");
			InstructionText("Colar:", "CTRL+V");
			InstructionText("Desfazer:", "CTRL+Z");
			InstructionText("Refazer:", "CTRL+Y");
			InstructionText("Adicionar/Remover da seleção:", "SHIFT+CLICK_ESQUERDO");
			InstructionText("Mover múltiplas peças:", "SHIFT+SEGURAR_BOTAO_ESQUERDO");
			InstructionText("Redimensionar peça em destaque:", "SHIFT+SCROLL");
			InstructionText("Zoom:", "SCROLL");
			InstructionText("Movimentar a câmera:", "SEGURAR_BOTAO_DIREITO");
			InstructionText("Restaurar câmera original:", "CLICK_SCROLL");

			End();
		}

		End();
	}
}

void ControlBar::Cleanup()
{
}

void ControlBar::Input(const sapp_event *e)
{
}
