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
	sgp_scale(zoom, zoom);
	sgp_translate(translation.x, translation.y);
}

void Mouse::Cleanup()
{
}

void Mouse::Input(const sapp_event *e)
{
	lastPosition = position;

	position = Vector
	{
		((e->mouse_x - program.viewport.x) / zoom) - translation.x,
		(e->mouse_y / zoom) - translation.y,
	};

	delta = Vector
	{
		e->mouse_dx / zoom,
		e->mouse_dy / zoom,
	};

	if (e->type == SAPP_EVENTTYPE_MOUSE_DOWN &&
		e->mouse_button == SAPP_MOUSEBUTTON_LEFT)
	{
		leftButtonDown = true;
		lastLeftDownPosition = position;
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
		zoom = 1.0f;
		translation = {};
	}

	if (e->type == SAPP_EVENTTYPE_MOUSE_SCROLL)
	{
		scroll = e->scroll_y;
		if (e->modifiers == SAPP_MODIFIER_SHIFT)
		{
			if (program.hot)
			{
				program.shapeList[program.hot]->Scale((-scroll) * 0.1f);
			}
		}
		else
		{
			if (scroll > 0)
			{
				zoom += 0.1f;
			}
			if (scroll < 0)
			{
				zoom -= 0.1f;
			}
		}
		if (zoom <= 0.01f)
		{
			zoom = 0.01f;
		}
	}

	if (e->type == SAPP_EVENTTYPE_MOUSE_MOVE)
	{
		if (leftButtonDown)
		{
		}

		if (rightButtonDown)
		{
			translation.x += delta.x / zoom;
			translation.y += delta.y / zoom;
		}
	}
}
