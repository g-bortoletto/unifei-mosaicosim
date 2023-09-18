#if !defined(TFG_PROGRAM_H)
#define TFG_PROGRAM_H

#include "tfgTypes.h"
#include "tfgRect.h"
#include "tfgObject.h"

#include <set>
#include <map>

class ControlBar;
class Mouse;
class Shape;
class DebugInfo;

class Program : public TfgObject
{
private:
	void InitSokolGfx();
	void InitSokolGp();
	void InitImGui();
	void InitWindow();
	void InitViewport();

	void ResetHot();
	void BeginFrame();
	void EndFrame();

public:
	bool showDebugInfo = false;
	unsigned long long idCounter = 1;
	unsigned long long hot = 0;

	Rect window;
	Rect viewport;

	ControlBar *controlBar = 0;
	Mouse *mouse = 0;
#ifdef _DEBUG
	DebugInfo *debugInfo = 0;
#endif

	std::map<u64, Shape *> shapeList;
	std::set<u64> selectionList;

	Program();
	virtual ~Program();

	virtual void Init() override;
	virtual void Frame() override;
	virtual void Cleanup() override;
	virtual void Input(const sapp_event *e) override;

	u64 CreateShape(u32 vertices);
	void DestroyShape();
};

#endif
