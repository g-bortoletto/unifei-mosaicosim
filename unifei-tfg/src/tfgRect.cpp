#include "tfgRect.h"

#include <sokol/sokol_gfx.h>
#include <sokol/sokol_gp.h>

void Rect::Draw(void) const
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
	sgp_draw_filled_rect(x, y, w, h);
	sgp_reset_color();
}
