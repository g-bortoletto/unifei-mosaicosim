#if !defined (TFG_VECTOR_H)
#define TFG_VECTOR_H

#include "tfgObject.h"
#include "tfgTypes.h"

class Vector
{
public:
	float x;
	float y;

	Color color;

	Vector(float x = 0.0f, float y = 0.0f);
	virtual ~Vector();

	void Draw(bool showLine = false) const;
	float Distance(const Vector &other) const;
	Vector Normalized() const;

	bool operator==(const Vector &other) const;
	Vector operator+(const Vector &other) const;
	Vector operator-(const Vector &other) const;
};

#endif