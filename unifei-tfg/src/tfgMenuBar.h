#pragma once

#include "tfgObject.h"

class Program;

class MenuBar : public TfgObject
{
private:
	Program &program;
	void FileMenu();
	void EditMenu();
	void SettingsMenu();
	void UpdateFontSize();
	void SetDarkTheme();
	void SetLightTheme();

public:
	bool show;
	float height;

	MenuBar(Program &program) : program(program) {}
	virtual ~MenuBar() {}

	virtual void Init() override;
	virtual void Frame() override;
	virtual void Cleanup() override;
	virtual void Input(const sapp_event *e) override;
};