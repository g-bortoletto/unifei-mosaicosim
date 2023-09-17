#include "tfgDebugInfo.h"

#include <sokol/sokol_app.h>

#include <imgui/imgui.h>

using namespace ImGui;

#include "tfgProgram.h"
#include "tfgMouse.h"
#include "tfgShape.h"

#include <map>

DebugInfo::DebugInfo(const Program &program) :
	program(program)
{
}

DebugInfo::~DebugInfo()
{
}

void DebugInfo::DebugTextBool(const char *text, bool condition)
{
	Text(text);
	SameLine();
	TextColored(
		condition
			? ImVec4(0, 255, 0, 255)
			: ImVec4(255, 0, 0, 255),
		condition
			? "TRUE"
			: "FALSE");
}

void DebugInfo::Init()
{
}

void DebugInfo::Frame()
{
	Begin("Debug Info", &show, ImGuiWindowFlags_AlwaysAutoResize);

	Separator();
	TextColored(ImVec4(0.0f, 255.0f, 255.0f, 255.0f), "%s", "PERFORMANCE");
	Separator();
	Text("FRAME TIME: %.6f", sapp_frame_duration());
	Text("FPS: %d", (unsigned int)(1 / sapp_frame_duration()));

	Separator();
	ImGui::TextColored(ImVec4(0, 255, 255, 255), "GERAL");
	Separator();
	Text("ENTITIES: %lld", program.idCounter - 1);
	Text("HOT: %lld", program.hot);
	Text("SELECTED:");
	for (u64 i : program.selectionList)
	{
		Text("\t+ %lld (%d)", i, program.shapeList.at(i)->hotVertex);
	}

	Separator();
	ImGui::TextColored(ImVec4(0, 255, 255, 255), "MOUSE");
	Separator();
	Text(
		"POSITION: (%.2f, %.2f)", 
		program.mouse->position.x, 
		program.mouse->position.y);
	Text(
		"DELTA: (%.2f, %.2f)",
		program.mouse->delta.x,
		program.mouse->delta.y);
	Text(
		"TRANSLATION: (%.2f, %.2f)",
		program.mouse->translation.x,
		program.mouse->translation.y);
	Text("ZOOM: %f", program.mouse->zoom);
	Text(
		"LAST LEFT DOWN: (%.2f, %.2f)", 
		program.mouse->lastLeftDownPosition.x, 
		program.mouse->lastLeftDownPosition.y);
	DebugTextBool("LEFT BUTTON DOWN:", program.mouse->leftButtonDown);
	DebugTextBool("RIGHT BUTTON DOWN:", program.mouse->rightButtonDown);
	DebugTextBool("IMGUI FOCUS:", program.mouse->imGuiWantsMouse);
	Text("SCROLL: %d", program.mouse->scroll);

	End();
}

void DebugInfo::Cleanup()
{
}

void DebugInfo::Input(const sapp_event *e)
{
	if (e->type == SAPP_EVENTTYPE_KEY_UP &&
		e->key_code == SAPP_KEYCODE_0)
	{
		show = !show;
	}
}
