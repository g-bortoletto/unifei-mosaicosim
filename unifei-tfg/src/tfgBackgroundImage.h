#pragma once

#include "tfgObject.h"

#include <sokol/sokol_gfx.h>
#include <string>

class BackgroundImage : public TfgObject
{
public:
	std::string path;
	sg_image img = {};
	int w;
	int h;

	BackgroundImage();
	virtual ~BackgroundImage();

	virtual void Init() override;
	virtual void Frame() override;
	virtual void Cleanup() override;
	virtual void Input(const sapp_event *e) override;

	void LoadBackgroundImage(void);
};