#include "tfgMouse.h"

#include <sokol/sokol_app.h>
#include <sokol/sokol_gfx.h>
#include <sokol/sokol_gp.h>
#include <imgui/imgui.h>
using namespace ImGui;

#include "tfgProgram.h"
#include "tfgShape.h"

Mouse::Mouse(Program &program) :
	program(program)
{
}

Mouse::~Mouse()
{
}

void Mouse::Init()
{
}

void Mouse::Frame()
{
	imGuiWantsMouse = GetIO().WantCaptureMouse;
	if (mouseCursor != ImGuiMouseCursor_Arrow)
	{
		SetMouseCursor(mouseCursor);
	}
	if (drawSelection)
	{
		selection.color = { 0.6f, 0.6f, 0.6f, 0.5f };
		selection.Draw(SGP_BLENDMODE_BLEND);
	}
}

void Mouse::Cleanup()
{
}

void Mouse::Input(const sapp_event *e)
{
	lastPosition = position;

	position = Vector
	{
		((e->mouse_x - program.viewport.x) / program.zoom) - program.translation.x,
		(e->mouse_y / program.zoom) - program.translation.y,
	};

	delta = Vector
	{
		e->mouse_dx / program.zoom,
		e->mouse_dy / program.zoom,
	};

	if (e->type == SAPP_EVENTTYPE_MOUSE_DOWN &&
		e->mouse_button == SAPP_MOUSEBUTTON_LEFT)
	{
		leftButtonDown = true;
		lastLeftDownPosition = position;
		selection.x = position.x;
		selection.y = position.y;
	}

	if (e->type == SAPP_EVENTTYPE_MOUSE_UP &&
		e->mouse_button == SAPP_MOUSEBUTTON_LEFT)
	{
		leftButtonDown = false;
	}

	if (e->type == SAPP_EVENTTYPE_MOUSE_DOWN &&
		e->mouse_button == SAPP_MOUSEBUTTON_RIGHT)
	{
		mouseCursor = ImGuiMouseCursor_ResizeAll;
		rightButtonDown = true;
	}

	if (e->type == SAPP_EVENTTYPE_MOUSE_UP &&
		e->mouse_button == SAPP_MOUSEBUTTON_RIGHT)
	{
		mouseCursor = ImGuiMouseCursor_Arrow;
		rightButtonDown = false;
	}

	if (e->type == SAPP_EVENTTYPE_MOUSE_UP &&
		e->mouse_button == SAPP_MOUSEBUTTON_MIDDLE)
	{
		program.zoom = 1.0f;
		program.translation = {};
	}

	if (e->type == SAPP_EVENTTYPE_MOUSE_SCROLL)
	{
		scroll = e->scroll_y;
		if (e->modifiers == SAPP_MODIFIER_SHIFT)
		{
			if (program.Hot())
			{
				program.shapeList[program.Hot()]->Scale((-scroll) * 0.1f);
			}
		}
		else
		{
			if (scroll > 0)
			{
				program.zoom *= 1.1f;
			}
			if (scroll < 0)
			{
				program.zoom /= 1.1f;
			}
		}
	}

	if (e->type == SAPP_EVENTTYPE_MOUSE_MOVE)
	{
		if (leftButtonDown)
		{
			selection.w = position.x - selection.x;
			selection.h = position.y - selection.y;
		}

		if (rightButtonDown)
		{
			program.translation.x += delta.x / program.zoom;
			program.translation.y += delta.y / program.zoom;
		}
	}
}
