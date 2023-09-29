#pragma once

#include "tfgObject.h"

class Program;

class MenuBar : public TfgObject
{
private:
	Program &program;
	bool showAboutMenu = false;
	bool showHelp = false;

	void FileMenu();
	void EditMenu();
	void SettingsMenu();
	void AboutMenu();
	void UpdateFontSize();
	void SetDarkTheme();
	void SetLightTheme();
	bool ButtonCenteredOnLine(const char *label, float alignment = 0.5f);

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