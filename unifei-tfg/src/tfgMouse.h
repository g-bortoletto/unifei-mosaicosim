#if !defined (TFG_MOUSE_H)
#define TFG_MOUSE_H

#include "tfgObject.h"
#include "tfgVector.h"

#include <imgui/imgui.h>

class Program;
struct sapp_event;

class Mouse : public TfgObject
{
private:
	Program &program;

public:
	Vector position = {};
	Vector delta = {};
	Vector lastPosition = {};
	Vector lastLeftDownPosition = {};
	Vector translation = {};

	ImGuiMouseCursor mouseCursor = ImGuiMouseCursor_Arrow;

	bool leftButtonDown = false;
	bool rightButtonDown = false;
	bool imGuiWantsMouse = false;

	int scroll = 0;
	
	float zoom = 1.0f;

	Mouse(Program &program);
	virtual ~Mouse();
	virtual void Init() override;
	virtual void Frame() override;
	virtual void Cleanup() override;
	virtual void Input(const sapp_event *e) override;
};

#endif