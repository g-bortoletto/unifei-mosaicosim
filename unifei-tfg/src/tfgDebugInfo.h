#if !defined (TFG_DEBUGINFO_H)
#define TFG_DEBUGINFO_H

#include "tfgObject.h"

class Program; 
struct sapp_event;

class DebugInfo : public TfgObject
{
private:
	const Program &program;
public:
	bool show = false;
	
	DebugInfo(const Program &program);
	virtual ~DebugInfo();

	void DebugTextBool(const char *text, bool condition);

	virtual void Init() override;
	virtual void Frame() override;
	virtual void Cleanup() override;
	virtual void Input(const sapp_event *e) override;
};

#endif
