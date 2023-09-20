#if !defined(TFG_PROGRAM_H)
#define TFG_PROGRAM_H

#include "tfgTypes.h"
#include "tfgRect.h"
#include "tfgObject.h"
#include "tfgVector.h"
#include "tfgShape.h"

#include <set>
#include <map>
#include <vector>

class ControlBar;
class BackgroundImage;
class Mouse;
class DebugInfo;

class Program : public TfgObject
{
private:
	u64 hot = 0;
	u64 hotPrevious = 0;

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
	u64 idCounter = 1;

	Rect window;
	Rect viewport;

	ControlBar *controlBar = 0;
	BackgroundImage *image = 0;
	Mouse *mouse = 0;
#ifdef _DEBUG
	DebugInfo *debugInfo = 0;
#endif

	std::map<u64, Shape> shapeList;
	std::set<u64> selectionList;
	std::vector<Shape> shapeClipboard;
	std::vector<std::map<u64, Shape>> undoBuffer;
	std::vector<std::map<u64, Shape>> redoBuffer;

	float zoom = 1.0f;
	Vector translation = {};

	Program();
	virtual ~Program();

	virtual void Init() override;
	virtual void Frame() override;
	virtual void Cleanup() override;
	virtual void Input(const sapp_event *e) override;

	u64 CreateShape(u32 vertices);
	u64 CopyShape(Shape &other);
	void DestroyShape();
	void SetHot(u64 id);
	const u64 &Hot(void) const;
	const u64 &HotPrevious(void) const;
	void LoadBackgroundImage(void);
	void Copy(void);
	void Paste(void);

	void UpdateUndoBuffer(void);
	void Undo(void);
	void Redo(void);
};

#endif