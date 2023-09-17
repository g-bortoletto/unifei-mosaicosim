#if !defined(TFG_OBJECT_H)
#define TFG_OBJECT_H

struct sapp_event;

class TfgObject
{
public:
	virtual void Init() = 0;
	virtual void Frame() = 0;
	virtual void Cleanup() = 0;
	virtual void Input(const sapp_event *e) = 0;
};

#endif
