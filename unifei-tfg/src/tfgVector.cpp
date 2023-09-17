#include "tfgVector.h"

#include <sokol/sokol_gfx.h>
#include <sokol/sokol_gp.h>

#include <math.h>

Vector::Vector(float x, float y) :
	x(x), y(y)
{
}

Vector::~Vector()
{
}

void Vector::Draw(bool showLine) const
{
	if (!sgp_is_valid())
	{
		return;
	}

	sgp_set_color(
		color.r,
		color.g,
		color.b,
		color.a);
	sgp_draw_filled_rect(
		x - 2.5f,
		y - 2.5f,
		5.0f,
		5.0f);
	if (showLine)
	{
		sgp_draw_line(0.0f, 0.0f, x, y);
	}
	sgp_reset_color();
}

float Vector::Distance(const Vector &other) const
{
	float deltaX = other.x - x;
	float deltaY = other.y - y;
	return sqrt(deltaX * deltaX + deltaY * deltaY);
}

Vector Vector::Normalized() const
{
	float magnitude = sqrtf(x * x + y * y);
	return Vector(
		x / magnitude,
		y / magnitude);
}

bool Vector::operator==(const Vector &other) const
{
	return x == other.x
		&& y == other.y;
}

Vector Vector::operator+(const Vector &other) const
{
	return Vector
	(
		x + other.x,
		y + other.y
	);
}

Vector Vector::operator-(const Vector &other) const
{
	return Vector
	(
		x - other.x,
		y - other.y
	);
}
