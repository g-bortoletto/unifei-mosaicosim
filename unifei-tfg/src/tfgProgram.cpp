#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Waddress-of-temporary"
#pragma clang diagnostic ignored "-Wc99-designator"
#pragma clang diagnostic ignored "-Wreorder-init-list"

#include "tfgProgram.h"
#include "tfgControlBar.h"
#include "tfgBackgroundImage.h"
#include "tfgMouse.h"
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

#include <parson/parson.h>

#include <nfd/nfd.h>

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
	if (hot && !shapeList.find(hot)->second.isHot)
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
		s.second.Frame();
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
		s.second.Input(e);
	}
#ifdef _DEBUG
	debugInfo->Input(e);
#endif

	if (e->type == SAPP_EVENTTYPE_MOUSE_DOWN
		&& e->mouse_button == SAPP_MOUSEBUTTON_LEFT
		&& !mouse->imGuiWantsMouse)
	{
		UpdateUndoBuffer();
	}

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

	if (e->type == SAPP_EVENTTYPE_KEY_DOWN
		&& e->key_code == SAPP_KEYCODE_Z
		&& e->modifiers == SAPP_MODIFIER_CTRL)
	{
		Undo();
	}

	if (e->type == SAPP_EVENTTYPE_KEY_DOWN
		&& e->key_code == SAPP_KEYCODE_Y
		&& e->modifiers == SAPP_MODIFIER_CTRL)
	{
		Redo();
	}
}
u64 Program::CreateShape(u32 vertices)
{
	UpdateUndoBuffer();
	std::pair<u64, Shape> shapePair(idCounter, Shape(*this, vertices));
	shapeList.emplace(shapePair);
	return idCounter++;
}

u64 Program::CopyShape(Shape &other)
{
	UpdateUndoBuffer();
	std::pair<u64, Shape> shapePair(idCounter, Shape(other, idCounter));
	shapeList.insert(shapePair);
	for (auto &v : shapeList.find(idCounter)->second.vertexList)
	{
		v.x += 20.0f;
		v.y += 20.0f;
	}
	return idCounter++;
}

void Program::DestroyShape()
{
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
		shapeClipboard.push_back(shapeList.find(s)->second);
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
		CopyShape(s);
	}
}

void Program::UpdateUndoBuffer(void)
{
	redoBuffer.clear();
	bool result = false;
	if (!undoBuffer.empty())
	{
		result = true;
		for (auto &s : shapeList)
		{
			auto last = undoBuffer.back().find(s.first);
			result = result 
				&& last != undoBuffer.back().end()
				&& s.second == last->second;
		}
	}
	if (!result)
	{
		undoBuffer.push_back(std::map<u64, Shape>(shapeList));
	}
}

void Program::Undo(void)
{
	if (undoBuffer.empty())
	{
		return;
	}
	redoBuffer.push_back(std::map<u64, Shape>(shapeList));
	shapeList = undoBuffer.back();
	undoBuffer.pop_back();
	selectionList.clear();
}

void Program::Redo(void)
{
	if (redoBuffer.empty())
	{
		return;
	}
	undoBuffer.push_back(std::map<u64, Shape>(shapeList));
	shapeList = redoBuffer.back();
	redoBuffer.pop_back();
	selectionList.clear();
}

void Program::Save(void)
{
	nfdchar_t *pp = 0;
	if (projectPath.empty())
	{
		nfdresult_t result = NFD_SaveDialog("tfg", 0, &pp);
		if (result == NFD_OKAY)
		{
			projectPath = std::string(pp);
		}
	}

	JSON_Value *rootVal = json_value_init_object();
	JSON_Object *rootObj = json_value_get_object(rootVal);
	char *serializedString = 0;

	JSON_Value *shapeListVal = json_value_init_array();
	JSON_Array *shapeListArr = json_value_get_array(shapeListVal);

	for (auto &s : shapeList)
	{
		JSON_Value *shapeVal = json_value_init_object();
		JSON_Object *shapeObj = json_value_get_object(shapeVal);


		JSON_Value *vertexVal = json_value_init_array();
		JSON_Array *vertexArr = json_value_get_array(vertexVal);

		for (auto &v : s.second.vertexList)
		{
			JSON_Value *vectorVal = json_value_init_object();
			JSON_Object *vectorObj = json_value_get_object(vectorVal);

			json_object_set_number(vectorObj, "x", v.x);
			json_object_set_number(vectorObj, "y", v.y);

			json_array_append_value(vertexArr, vectorVal);
		}
		json_object_set_value(shapeObj, "vertexList", vertexVal);

		json_object_dotset_number(shapeObj, "color.r", s.second.color.r);
		json_object_dotset_number(shapeObj, "color.g", s.second.color.g);
		json_object_dotset_number(shapeObj, "color.b", s.second.color.b);
		json_object_dotset_number(shapeObj, "color.a", s.second.color.a);
		
		json_array_append_value(shapeListArr, shapeVal);
	}
	json_object_set_value(rootObj, "shapeList", shapeListVal);

	serializedString = json_serialize_to_string_pretty(rootVal);

	FILE *handle = fopen(projectPath.c_str(), "w");
	if (handle)
	{
		fprintf(handle, "%s", serializedString);
		fclose(handle);
	}
	json_free_serialized_string(serializedString);
	json_value_free(rootVal);
}

void Program::Load(void)
{
	nfdchar_t *pp = 0;
	nfdresult_t result = NFD_OpenDialog("tfg", 0, &pp);
	if (result == NFD_OKAY)
	{
		projectPath = std::string(pp);
	}

	JSON_Value *jv = json_parse_file(projectPath.c_str());
	JSON_Object *rootObj = json_value_get_object(jv);
	JSON_Array *shapeListArr = json_object_get_array(rootObj, "shapeList");
	JSON_Object *shapeObj = 0;
	for (size_t i = 0; i < json_array_get_count(shapeListArr); ++i)
	{
		shapeObj = json_array_get_object(shapeListArr, i);
		JSON_Array *vertexList = json_object_get_array(shapeObj, "vertexList");
		JSON_Object *vertexObj = 0;
		size_t vertexCount = json_array_get_count(vertexList);
		std::vector<Vector> vertexVector;
		for (size_t j = 0; j < vertexCount; ++j)
		{
			vertexObj = json_array_get_object(vertexList, j);
			vertexVector.push_back(Vector(
				json_object_get_number(vertexObj, "x"), 
				json_object_get_number(vertexObj, "y")));
		}
		Color c = Color
		{
			.r = (float)json_object_dotget_number(shapeObj, "color.r"),
			.g = (float)json_object_dotget_number(shapeObj, "color.g"),
			.b = (float)json_object_dotget_number(shapeObj, "color.b"),
			.a = (float)json_object_dotget_number(shapeObj, "color.a"),
		};
		u64 ns = CreateShape(vertexCount);
		for (int j = 0; j < vertexCount; ++j)
		{
			shapeList.find(ns)->second.vertexList[j] = vertexVector[j];
		}
		shapeList.find(ns)->second.color = c;
	}
}

#pragma clang diagnostic pop