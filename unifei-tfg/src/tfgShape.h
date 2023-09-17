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
	const float size = 100.0f;
	Program &program;
	u64 id = 0;
	u32 vertexCount = 0;

	int IsLeft(Vector p0, Vector p1, Vector p2) const;
	i32 HotVertex(const Vector &p) const;
	bool IsHot(const Vector &p) const;
	bool LightColor(void) const;
	void Highlight(int i);
	void DrawDebugLineToVertices();
	void Draw();
	bool CanMove(const sapp_event *e) const;
	void Move();

public:
	std::vector<Vector> vertexList = {};
	Color color = {};
	bool isHot = false;
	i32 hotVertex = -1;
	float vertexRadius = 5.0f;

	Shape(Program &program, u32 vertexCount, Color color = { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f });
	virtual ~Shape();

	const u64 Id(void) const;
	void Scale(float amount);

	virtual void Init() override;
	virtual void Frame() override;
	virtual void Cleanup() override;
	virtual void Input(const sapp_event *e) override;
};

#endif