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
	SetNextWindowPos(ImVec2(0.0f, program.MenuBarHeight()));
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
			.y = program.MenuBarHeight(),
			.w = program.window.w < width
					? width
					: program.window.w - width,
			.h = program.window.h < program.MenuBarHeight()
					? program.MenuBarHeight()
					: program.window.h - program.MenuBarHeight(),
		};

		static int vertices = 3;
		static unsigned char step = 1;

		SeparatorText("IMAGEM DE REFERÊNCIA");
		Spacing();

		BeginDisabled(!program.image->path.empty());
		if (Button("Adicionar##refencia", ImVec2(buttonSize, 0.0f))) 
		{
			program.image->path.clear();
			program.LoadBackgroundImage();
		}
		EndDisabled();

		BeginDisabled(program.image->path.empty());
		if (Button("Remover##refencia", ImVec2(buttonSize, 0.0f)))
		{
			program.image->path.clear();
			sg_destroy_image(program.image->img);
		}
		EndDisabled();

		if (Checkbox("Ocultar", &program.image->hide)) 
		{}

		for (int i = 0; i < 10; ++i)
		{
			Spacing();
		}
		SeparatorText("PEÇA");
		Spacing();

		if (!program.selectionList.empty())
		{
			currentColor = program
				.shapeList
				.find(*program.selectionList.begin())
				->second.color;
		}
		Text("Cor");
		SetNextItemWidth(buttonSize);
		if (ColorEdit3(
			"## COR PEÇA",
			&currentColor.r,
			ImGuiColorEditFlags_PickerHueWheel))
		{
			for (auto &s : program.selectionList)
			{
				program.shapeList.find(s)->second.color = currentColor;
			}
		}

		Spacing();
		Text("%s", "Número de lados");
		SetNextItemWidth(buttonSize);
		if (InputScalar("## ADD_PEÇA", ImGuiDataType_U8, &vertices, &step))
		{
			if (vertices < 3) vertices = 3;
			if (vertices > 9) vertices = 9;
		}
		
		if (Button("Adicionar##peça", ImVec2(buttonSize, 0.0f))) 
		{ 
			u64 created = program.CreateShape(vertices);
			program.shapeList.find(created)->second.color = currentColor;
		}

		BeginDisabled(program.selectionList.empty());
		if (Button("Remover##peça", ImVec2(buttonSize, 0.0f)))
		{
			program.DestroyShape();
		}
		EndDisabled();

		BeginDisabled(program.shapeList.empty());
		if (Button("Limpar##peça", ImVec2(buttonSize, 0.0f)))
		{
			program.SelectAll();
			program.DestroyShape();
		}
		EndDisabled();

		for (int i = 0; i < 10; ++i) Spacing();
		SeparatorText("REJUNTE");
		Spacing();
		Checkbox("Sobrepor imagem de referência", &program.showImageOverlap);
		Text("Cor");
		SetNextItemWidth(buttonSize);
		ColorEdit3("##CORREJUNTE", &program.imageOverlapColor.r, ImGuiColorEditFlags_PickerHueWheel);

		End();
	}
}

void ControlBar::Cleanup()
{
}

void ControlBar::Input(const sapp_event *e)
{
}
