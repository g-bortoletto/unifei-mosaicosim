#if !defined (TFG_TYPES_H)
#define TFG_TYPES_H

typedef unsigned int u32;
typedef unsigned long long u64;

typedef int i32;
typedef long long i64;

struct Color
{
	float r;
	float g;
	float b;
	float a;

	bool operator==(const Color &other) const 
	{
		return r == other.r
			&& g == other.g
			&& b == other.b
			&& a == other.a;
	}
};

#endif
