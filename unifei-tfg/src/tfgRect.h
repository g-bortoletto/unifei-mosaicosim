#if !defined (TFG_RECT_H)
#define TFG_RECT_H

#include "tfgTypes.h"

class Rect
{
public:
	float x;
	float y;
	float w;
	float h;

	Color color;

	void Draw(void) const;
};

#endif
