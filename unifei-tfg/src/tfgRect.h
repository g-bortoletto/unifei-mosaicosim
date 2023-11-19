#if !defined (TFG_RECT_H)
#define TFG_RECT_H

#include <sokol/sokol_gfx.h>
#include <sokol/sokol_gp.h>
#include "tfgTypes.h"

class Rect
{
public:
	float x;
	float y;
	float w;
	float h;

	Color color;

	void Draw(sgp_blend_mode blendMode) const;
};

#endif
