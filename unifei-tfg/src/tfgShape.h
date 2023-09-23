#if !defined(TFG_SHAPE_H)
#define TFG_SHAPE_H

#include "tfgObject.h"
#include "tfgTypes.h"
#include "tfgVector.h"

#include <vector>

class Program;

class Shape : public TfgObject
{
private:
	float size = 100.0f;
	Program &program;
	u64 id = 0;
	u32 vertexCount = 0;

	int IsLeft(Vector p0, Vector p1, Vector p2) const;
	i32 HotVertex(const Vector &p) const;
	bool IsHot(const Vector &p) const;
	bool LightColor(void) const;
	void Highlight();
	void DrawDebugLineToVertices();
	void DrawWireTriangle(Vector a, Vector b, Vector c) const;
	void DrawShape();
	void Draw();
	bool CanMove(const sapp_event *e) const;
	void Move();

public:
	std::vector<Vector> vertexList = {};
	Color color = {};
	bool isHot = false;
	i32 hotVertex = -1;
	float vertexInitialRadius = 10.0f;
	float vertexRadius = 10.0f;

	Shape(Program &program, u32 vertexCount, Color color = { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f });
	Shape(const Shape &other);
	Shape(const Shape &other, u64 id);
	virtual ~Shape();

	const u64 Id(void) const;
	void Scale(float amount);
	Vector Center() const;

	virtual void Init() override;
	virtual void Frame() override;
	virtual void Cleanup() override;
	virtual void Input(const sapp_event *e) override;

	bool operator==(const Shape &other) const;
};

#endif