#if !defined (TFG_RECT_H)
#define TFG_RECT_H

#include "tfgTypes.h"

enum sgp_blend_mode;

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
