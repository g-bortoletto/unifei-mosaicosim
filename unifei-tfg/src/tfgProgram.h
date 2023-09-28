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
#include <string>

class MenuBar;
class ControlBar;
class BackgroundImage;
class Mouse;
class DebugInfo;

class Program : public TfgObject
{
private:
	bool showDemo = false;

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
	i32 hotVertex = -1;
	std::string projectPath;
	bool showDebugInfo = false;
	u64 idCounter = 1;

	Rect window;
	Rect viewport;

	MenuBar *menuBar = 0;
	ControlBar *controlBar = 0;
	BackgroundImage *image = 0;
	Mouse *mouse = 0;
	DebugInfo *debugInfo = 0;

	std::map<u64, Shape> shapeList;
	std::set<u64> selectionList;
	std::vector<Shape> shapeClipboard;
	std::vector<std::map<u64, Shape>> undoBuffer;
	std::vector<std::map<u64, Shape>> redoBuffer;

	float zoom = 1.0f;
	Vector translation = {};

	bool darkMode = true;

	Color clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	unsigned int fontSize = 16;

	bool showImageOverlap = false;
	Color imageOverlapColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	Program();
	virtual ~Program();

	virtual void Init() override;
	virtual void Frame() override;
	virtual void Cleanup() override;
	virtual void Input(const sapp_event *e) override;

	float MenuBarHeight() const;

	u64 CreateShape(u32 vertices);
	u64 CopyShape(Shape &other);
	void DestroyShape();
	void SetHot(u64 id);
	const u64 &Hot(void) const;
	const u64 &HotPrevious(void) const;
	void LoadBackgroundImage(void);
	void Cut(void);
	void Copy(void);
	void Paste(void);

	void UpdateUndoBuffer(void);
	void Undo(void);
	void Redo(void);

	void NewProject();
	void Save(void);
	void Load(void);
};

#endif