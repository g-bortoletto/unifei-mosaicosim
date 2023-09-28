#if !defined (TFG_CONTROLBAR_H)
#define TFG_CONTROLBAR_H

#include "tfgObject.h"
#include "tfgTypes.h"

class Program;

class ControlBar : public TfgObject
{
private:
	Program &program;
public:
	float width;
	float buttonSize;
	float buttonPadding = 5.0f;
	float inputScalarWidth = 80.0f;
	Color currentColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	
	ControlBar(Program &program);
	virtual ~ControlBar();

	virtual void Init() override;
	virtual void Frame() override;
	virtual void Cleanup() override;
	virtual void Input(const sapp_event *e) override;
};

#endif
