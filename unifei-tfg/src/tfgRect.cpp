#include "tfgRect.h"

#include <sokol/sokol_gfx.h>
#include <sokol/sokol_gp.h>

void Rect::Draw(sgp_blend_mode blendMode = SGP_BLENDMODE_NONE) const
{
	if (!sgp_is_valid())
	{
		return;
	}

	sgp_push_transform();
	sgp_set_blend_mode(blendMode);
	sgp_set_color(
		color.r,
		color.g,
		color.b,
		color.a);
	sgp_draw_filled_rect(x, y, w, h);
	sgp_reset_color();
	sgp_reset_blend_mode();
	sgp_pop_transform();
}
