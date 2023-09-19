#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Waddress-of-temporary"
#pragma clang diagnostic ignored "-Wc99-designator"
#pragma clang diagnostic ignored "-Wreorder-init-list"

#include "tfgProgram.h"
#include "tfgControlBar.h"
#include "tfgBackgroundImage.h"
#include "tfgMouse.h"
#include "tfgShape.h"
#include "tfgDebugInfo.h"

#include <imgui/imgui.h>

#define SOKOL_IMPL
#define SOKOL_GLCORE33
#include <sokol/sokol_app.h>
#include <sokol/sokol_time.h>
#include <sokol/sokol_gfx.h>
#include <sokol/sokol_gp.h>
#include <sokol/sokol_log.h>
#include <sokol/sokol_glue.h>
#include <fonts/segoeui.h>
#include <sokol/sokol_imgui.h>

void Program::InitSokolGfx()
{
	sg_setup(&(sg_desc)
	{
		.context = sapp_sgcontext(),
		.logger.func = slog_func,
	});
}

void Program::InitSokolGp()
{
	sgp_setup(&(sgp_desc) {});
	if (!sgp_is_valid())
	{
		fprintf(
			stderr,
			"Failed to create Sokol GP context: %s\n",
			sgp_get_error_message(sgp_get_last_error()));
		exit(-1);
	}
}

void Program::InitImGui()
{
	simgui_setup(&(simgui_desc_t)
	{
		.no_default_font = true,
	});
	auto &io = ImGui::GetIO();
	ImFontConfig fontCfg;
	fontCfg.FontDataOwnedByAtlas = false;
	fontCfg.OversampleH = 2;
	fontCfg.OversampleV = 2;
	fontCfg.RasterizerMultiply = 1.5f;
	io.Fonts->AddFontFromMemoryCompressedTTF(
		segoeui_compressed_data,
		segoeui_compressed_size,
		16.0f,
		&fontCfg);
	unsigned char *font_pixels;
	int font_width, font_height;
	io.Fonts->GetTexDataAsRGBA32(&font_pixels, &font_width, &font_height);
	sg_image_desc img_desc = {
		.width = font_width,
		.height = font_height,
		.pixel_format = SG_PIXELFORMAT_RGBA8,
		.min_filter = SG_FILTER_LINEAR,
		.mag_filter = SG_FILTER_LINEAR,
		.wrap_u = SG_WRAP_CLAMP_TO_BORDER,
		.wrap_v = SG_WRAP_CLAMP_TO_BORDER,
		.data.subimage[0][0].ptr = font_pixels,
		.data.subimage[0][0].size = (size_t)(font_width * font_height * 4),
	};
	io.Fonts->TexID = (ImTextureID)(uintptr_t)sg_make_image(&img_desc).id;
}

void Program::InitWindow()
{
	window = (Rect)
	{
		.x = 0.0f,
		.y = 0.0f,
		.w = 1280.0f,
		.h = 720.0f,
	};
}

void Program::InitViewport()
{
	viewport = (Rect)
	{
		.x = 250.0f,
		.y = 0.0f,
		.w = sapp_widthf() - 250.0f,
		.h = sapp_heightf(),
	};
}

void Program::ResetHot()
{
	if (hot && !shapeList[hot]->isHot)
	{
		SetHot(0);
	}
}

void Program::BeginFrame()
{
	window.w = sapp_widthf();
	window.h = sapp_heightf();

	sgp_begin(window.w, window.h);
	simgui_new_frame(&(simgui_frame_desc_t)
	{
		.width = (int)window.w,
		.height = (int)window.h,
		.delta_time = sapp_frame_duration(),
		.dpi_scale = sapp_dpi_scale(),
	});

	sgp_set_color(0.0f, 0.0f, 0.0f, 1.0f);
	sgp_clear();
	sgp_reset_color();

	sgp_viewport(
		viewport.x,
		viewport.y,
		viewport.w,
		viewport.h);

	ResetHot();
	
	sgp_scale(zoom, zoom);
	sgp_translate(translation.x, translation.y);
}

void Program::EndFrame()
{
	sg_pass_action passAction = {};
	sg_begin_default_pass(
		&passAction,
		this->window.w,
		this->window.h);
	sgp_reset_pipeline();
	sgp_flush();
	sgp_end();
	simgui_render();
	sg_end_pass();
	sg_commit();
}

Program::Program()
{
	controlBar = new ControlBar(*this);
	image = new BackgroundImage();
	mouse = new Mouse(*this);
#ifdef _DEBUG
	debugInfo = new DebugInfo(*this);
#endif
}

Program::~Program()
{
#ifdef _DEBUG
	delete debugInfo;
#endif
	delete mouse;
	delete image;
	delete controlBar;
}
void Program::Init()
{
	this->InitSokolGfx();
	this->InitSokolGp();
	this->InitImGui();
	this->InitWindow();
	this->InitViewport();
	stm_setup();
}

void Program::Frame()
{
	BeginFrame();

	controlBar->Frame();
	image->Frame();
	mouse->Frame();
	for (auto &s : shapeList)
	{
		s.second->Frame();
	}
#ifdef _DEBUG
	if (debugInfo->show)
	{
		debugInfo->Frame();
	}
#endif

	EndFrame();
}

void Program::Cleanup()
{
}

void Program::Input(const sapp_event *e)
{
	simgui_handle_event(e);
	mouse->Input(e);
	for (auto &s : shapeList)
	{
		s.second->Input(e);
	}
#ifdef _DEBUG
	debugInfo->Input(e);

	if (e->type == SAPP_EVENTTYPE_KEY_DOWN
		&& e->key_code == SAPP_KEYCODE_C
		&& e->modifiers == SAPP_MODIFIER_CTRL)
	{
		Copy();
	}

	if (e->type == SAPP_EVENTTYPE_KEY_DOWN
		&& e->key_code == SAPP_KEYCODE_V
		&& e->modifiers == SAPP_MODIFIER_CTRL)
	{
		Paste();
	}
#endif
}
u64 Program::CreateShape(u32 vertices)
{
	shapeList.insert(
	{
		idCounter,
		new Shape(*this, vertices) 
	});
	return idCounter++;
}

u64 Program::CopyShape(Shape &other)
{
	shapeList.insert(
		{
			idCounter,
			new Shape(other, idCounter)
		});
	for (auto &v : shapeList[idCounter]->vertexList)
	{
		v.x += 20.0f;
		v.y += 20.0f;
	}
	return idCounter++;
}

void Program::DestroyShape()
{
	for (auto &i : selectionList)
	{
		delete shapeList[i];
		shapeList.erase(i);
	}
	selectionList.clear();
}

void Program::SetHot(u64 id)
{
	hotPrevious = hot;
	hot = id;
}

const u64 &Program::Hot(void) const
{
	return hot;
}

const u64 &Program::HotPrevious(void) const
{
	return hotPrevious;
}

void Program::LoadBackgroundImage(void)
{
	image->LoadBackgroundImage();
}

void Program::Copy(void)
{
	if (selectionList.empty())
	{
		return;
	}

	shapeClipboard.clear();
	for (auto &s : selectionList)
	{
		shapeClipboard.push_back(shapeList[s]);
	}
}

void Program::Paste(void)
{
	if (shapeClipboard.empty())
	{
		return;
	}

	for (auto &s : shapeClipboard)
	{
		CopyShape(*s);
	}
}

#pragma clang diagnostic pop