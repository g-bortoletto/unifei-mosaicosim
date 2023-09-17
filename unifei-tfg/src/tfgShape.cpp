#include "tfgShape.h"
#include "tfgProgram.h"
#include "tfgMouse.h"

#include <sokol/sokol_app.h>
#include <sokol/sokol_gfx.h>
#include <sokol/sokol_gp.h>

#ifdef _DEBUG
#include "tfgDebugInfo.h"
#endif

static const float PI = 3.14159265359f;

int Shape::IsLeft(Vector p0, Vector p1, Vector p2) const
{
#ifdef _DEBUG
	if (program.debugInfo->show && sgp_is_valid())
	{
		sgp_push_transform();
		p2.color = 
		{ 
			1.0f - color.r, 
			1.0f - color.g, 
			1.0f - color.b, 
			1.0f,
		};
		p2.Draw();
		sgp_set_color(p2.color.r, p2.color.g, p2.color.b, p2.color.a);
		sgp_draw_line(p0.x, p0.y, p1.x, p1.y);
		sgp_draw_line(p0.x + 1, p0.y, p1.x + 1, p1.y);
		sgp_draw_line(p0.x - 1, p0.y, p1.x - 1, p1.y);
		sgp_reset_color();
		sgp_pop_transform();
	}
#endif

	return ((p1.x - p0.x) * (p2.y - p0.y)
		- (p2.x - p0.x) * (p1.y - p0.y));	
}

i32 Shape::HotVertex(const Vector &p) const
{
	if (program.selectionList.find(id) == program.selectionList.end())
	{
		return -1;
	}

	if (hotVertex >= 0 && p.Distance(vertexList[hotVertex]) <= vertexRadius)
	{
		return hotVertex;
	}
	
	for (int i = 0; i < vertexCount; ++i)
	{
		if (p.Distance(vertexList[i]) <= vertexRadius)
		{
			return i;
		}
	}
	
	return -1;
}

bool Shape::IsHot(const Vector &p) const
{
	int wn = 0;

	std::vector<Vector> vertices = vertexList;
	vertices.push_back(vertices[0]);
	for (int i = 0; i < vertexCount; ++i)
	{
		if (vertices[i].y <= p.y)
		{
			if (vertices[i + 1].y > p.y)
			{
				if (IsLeft(vertices[i], vertices[i + 1], p) > 0)
				{
					++wn;
				}
			}
		}
		else
		{
			if (vertices[i + 1].y <= p.y)
			{
				if (IsLeft(vertices[i], vertices[i + 1], p) < 0)
				{
					--wn;
				}
			}
		}
	}

	return wn != 0;
}

bool Shape::LightColor(void) const
{
	float c = (color.r + color.g + color.b) / 3;
	return c >= 0.8f;
}

void Shape::Highlight(int i)
{
	sgp_set_blend_mode(SGP_BLENDMODE_BLEND);
	if (LightColor())
	{
		sgp_set_color(0.0f, 0.0f, 0.0f, 0.5f);
	}
	else
	{
		sgp_set_color(1.0f, 1.0f, 1.0f, 0.5f);
	}
	sgp_draw_filled_triangle(
		vertexList[0].x, vertexList[0].y,
		vertexList[i].x, vertexList[i].y,
		vertexList[i + 1].x, vertexList[i + 1].y);
	sgp_reset_blend_mode();
	sgp_reset_color();
}

void Shape::DrawDebugLineToVertices()
{
#ifdef _DEBUG
	if (program.debugInfo->show)
	{
		sgp_push_transform();
		sgp_set_blend_mode(SGP_BLENDMODE_BLEND);
		for (auto &v : vertexList)
		{
			sgp_set_color(
				1.0f - color.r,
				1.0f - color.g,
				1.0f - color.b,
				1.0f);
			sgp_draw_line(
				program.mouse->position.x,
				program.mouse->position.y,
				v.x,
				v.y);
			sgp_reset_color();
		}
		sgp_reset_blend_mode();
		sgp_pop_transform();
	}
#endif
}

void Shape::Draw()
{
	for (int i = 1; i < (vertexCount - 1); ++i)
	{
		sgp_set_color(color.r, color.g, color.b, color.a);
		sgp_draw_filled_triangle(
			vertexList[0].x, vertexList[0].y,
			vertexList[i].x, vertexList[i].y,
			vertexList[i + 1].x, vertexList[i + 1].y);
		
		hotVertex = HotVertex(program.mouse->position);
		if (hotVertex >= 0)
		{
			vertexList[hotVertex].color = Color
			{
				.r = 1.0f,
				.g = 1.0f,
				.b = 1.0f,
				.a = 1.0f,
			};
			vertexList[hotVertex].Draw();
		}
		else
		{
			isHot = IsHot(program.mouse->position);
			if (isHot)
			{
				program.hot = this->id;
				Highlight(i);
				DrawDebugLineToVertices();
			}
		}
		sgp_reset_color();
	}
	if (program.selectionList.find(id) != program.selectionList.end())
	{
		sgp_push_transform();
		sgp_set_color(1.0f, 1.0f, 1.0f, 1.0f);

		for (int i = 0; i < (vertexCount - 1); ++i)
		{
			sgp_draw_line(
				vertexList[i].x,
				vertexList[i].y,
				vertexList[i + 1].x,
				vertexList[i + 1].y);
		}
		sgp_draw_line(
			vertexList[vertexCount - 1].x,
			vertexList[vertexCount - 1].y,
			vertexList[0].x,
			vertexList[0].y);

		sgp_pop_transform();
	}
}

bool Shape::CanMove(const sapp_event *e) const
{
	return hotVertex < 0
		&& isHot
		&& program.selectionList.find(id) != program.selectionList.end()
		&& program.mouse->leftButtonDown
		&& e->type == SAPP_EVENTTYPE_MOUSE_MOVE;
}

void Shape::Move()
{
	for (int i = 0; i < vertexCount; ++i)
	{
		vertexList[i].x += program.mouse->delta.x;
		vertexList[i].y += program.mouse->delta.y;
	}
}

Shape::Shape(
	Program &program,
	u32 vertexCount,
	Color color) :
	program(program),
	vertexCount(vertexCount),
	color(color)
{
	id = program.idCounter;
	float hw = program.viewport.w * 0.5f;
	float hh = program.viewport.h * 0.5f;
	for (int i = 0; i < vertexCount; ++i)
	{
		vertexList.push_back((Vector)
		{
			size * cosf(i * 2 * PI / vertexCount) + hw,
			size * sinf(i * 2 * PI / vertexCount) + hh,
		});
	}
}

Shape::~Shape()
{
}

const u64 Shape::Id(void) const
{
	return this->id;
}

void Shape::Scale(float amount)
{
	Vector center = {};
	for (auto &v : vertexList)
	{
		center.x += v.x;
		center.y += v.y;
	}
	center.x /= vertexCount;
	center.y /= vertexCount;

	for (auto &v : vertexList)
	{
		Vector scaleDirection = (center - v);
		v.x += scaleDirection.x * amount;
		v.y += scaleDirection.y * amount;
	}
}

void Shape::Init()
{
}

void Shape::Frame()
{
	if (!sgp_is_valid())
	{
		return;
	}

	Draw();
}

void Shape::Cleanup()
{
}

void Shape::Input(const sapp_event *e)
{
	if (program.mouse->imGuiWantsMouse)
	{
		return;
	}

	if (e->type == SAPP_EVENTTYPE_MOUSE_UP
		&& e->mouse_button == SAPP_MOUSEBUTTON_LEFT)
	{
		hotVertex = -1;
	}
	
	if (hotVertex >= 0
		&& program.mouse->leftButtonDown
		&& e->type == SAPP_EVENTTYPE_MOUSE_MOVE)
	{
		vertexList[hotVertex].x += program.mouse->delta.x;
		vertexList[hotVertex].y += program.mouse->delta.y;
	}

	if (CanMove(e))
	{
		Move();
	}

	if (e->type == SAPP_EVENTTYPE_MOUSE_DOWN 
		&& e->mouse_button == SAPP_MOUSEBUTTON_LEFT)
	{
		bool isSelected = program.selectionList.find(id) 
			!= program.selectionList.end();

		if (isHot)
		{
			if (!isSelected)
			{
				program.selectionList.clear();
				program.selectionList.insert(id);
			}
		}
		else
		{
			if (isSelected)
			{
				if (hotVertex < 0)
				{
					program.selectionList.clear();
				}
			}
		}
	}
}
