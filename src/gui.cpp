#include "gui.h"

#include "types.h"

#include "image.h"

#include "shape.h"

#include "../include/sokol/sokol_app.h"

#include "../include/imgui/imgui.h"

static void debug_text_bool(const char *text, const bool *condition)
{
	ImGui::Text(text, 0);
	ImGui::SameLine();
	ImGui::TextColored(
		(*condition) ? ImVec4(0, 255, 0, 255) : ImVec4(255, 0, 0, 255), 
		(*condition) ? "true" : "false");
}

// -------------------------------------------------------------------------------------------------

void debug_window_draw(program_state *program)
{
	ImGui::SetNextWindowPos(ImVec2(program->viewport.x, program->viewport.y));
	ImGui::SetNextWindowSize(ImVec2(200, 0));
	if (ImGui::Begin("Debug Window"))
	{
		ImGui::Separator();
		ImGui::TextColored(ImVec4(0, 255, 255, 255), "PERFORMANCE");
		ImGui::Separator();
		ImGui::Text("frame time: %.6f", sapp_frame_duration());
		ImGui::Text("fps: %d", (i32)(1 / sapp_frame_duration()));

		ImGui::Separator();
		ImGui::TextColored(ImVec4(0, 255, 255, 255), "GERAL");
		ImGui::Separator();
		ImGui::Text("id counter: %lld", program->id_counter);
		ImGui::Text("shape list size: %lld", program->shape_list_size);
		ImGui::Text("hot: %lld", program->hot);
		ImGui::Text("selected:");
		if (program->selected.empty()) { ImGui::SameLine(); ImGui::Text("none"); }
		else
		{
			for (auto i : program->selected)
			{
				ImGui::Text("\tid: %lld", i);
			}
		}
		ImGui::Text("last selected:");
		if (program->last_selected.empty()) { ImGui::SameLine(); ImGui::Text("none"); }
		else
		{
			for (auto i : program->last_selected)
			{
				ImGui::Text("\tid: %lld", i);
			}
		}

		ImGui::Separator();
		ImGui::TextColored(ImVec4(0, 255, 255, 255), "VIEWPORT");
		ImGui::Separator();
		ImGui::Text("scale: %.2f", program->scale);
		ImGui::Text("translation: (%.2f, %.2f)", program->translation.x, program->translation.y); 

		ImGui::Separator();
		ImGui::TextColored(ImVec4(0, 255, 255, 255), "MOUSE");
		ImGui::Separator();
		debug_text_bool("is mouse moving:", &program->mouse.is_moving);
		debug_text_bool("left button down", &program->mouse.is_left_button_down);
		debug_text_bool("right button down:", &program->mouse.is_right_button_down);
		debug_text_bool("middle button down:", &program->mouse.is_middle_button_down);
		ImGui::Text("position: (%.2f, %.2f)", program->mouse.position.x, program->mouse.position.y);
		ImGui::Text("delta: (%.2f, %.2f)", program->mouse.delta.x, program->mouse.delta.y);
		ImGui::Text("scroll: %d", program->mouse.scroll);

		// ImGui:program->mouse.is_moving ? ImVec4(0, 255, 0, 255) : ImVec4(255, 0, 0, 255), :Text("shape_lock: %s", program->shape_lock ? "true" : "false");
		// ImGui::Text("point_lock: %d", program->point_lock);
		// ImGui::Text(
		// 	"selection:\n\tx: %.1f\n\ty: %.1f\n\tw: %.1f\n\th: %.1f", 
		// 	program->selection.x,
		// 	program->selection.y,
		// 	program->selection.w,
		// 	program->selection.h);
		// ImGui:program->mouse.is_moving ? ImVec4(0, 255, 0, 255) : ImVec4(255, 0, 0, 255), :Text("draw_selection: %s", program->draw_selection ? "true" : "false");
		// ImGui::Text("main_menu_bar_height: %d", program->main_menu_bar_height);
		// ImGui::Text("side_bar_width: %d", program->side_bar_width);
		// ImGui::Text(
		// 	"window_size: (%d, %d)", 
		// 	program->window_width, 
		// 	program->window_height);
		// ImGui::Text("window_ratio: %.2f", program->window_ratio);
		// ImGui::Text(
		// 	"viewport_pos: (%d, %d)", 
		// 	program->viewport_x, 
		// 	program->viewport_y);
		// ImGui::Text("viewport_size: (%d, %d)", program->viewport_width,program->viewport_height);
		// ImGui::Text("viewport_ratio: %.2f", program->viewport_ratio);
		// ImGui::Text("shape_list.size(): %lld", program->shape_list.size());
		// ImGui::Text("offset: (%2f, %2f)", program->offset.x, program->offset.y);
		// ImGui::InputFloat("zoom", &program->zoom, 0.1f, 1.0f);
		ImGui::End();
	}
}

// -------------------------------------------------------------------------------------------------

void side_bar_draw(program_state *program)
{
	bool sidebar_open = true;
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize(ImVec2(program->viewport.x, program->viewport.h));
	if (ImGui::Begin("sidebar", &sidebar_open,
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize 
|       ImGuiWindowFlags_NoTitleBar))
	{
		if (ImGui::Button("Carregar imagem", ImVec2(185.0f, 0.0f)))     { main_image_open(&program->main_image); }
		if (ImGui::Button("Adicionar triângulo", ImVec2(185.0f, 0.0f))) { triangle_add(program); }

		if (!program->selected.empty())
		{

			float pa[] = { program->shape_list[program->selected[0]].p[0].x, program->shape_list[program->selected[0]].p[0].y };
			ImGui::InputFloat2("A", pa);
			program->shape_list[program->selected[0]].p[0] = { .x = pa[0], .y = pa[1] };

			float pb[] = { program->shape_list[program->selected[0]].p[1].x, program->shape_list[program->selected[0]].p[1].y };
			ImGui::InputFloat2("B", pb);
			program->shape_list[program->selected[0]].p[1] = { .x = pb[0], .y = pb[1] };

			float pc[] = { program->shape_list[program->selected[0]].p[2].x, program->shape_list[program->selected[0]].p[2].y };
			ImGui::InputFloat2("C", pc);
			program->shape_list[program->selected[0]].p[2] = { .x = pc[0], .y = pc[1] };

			float color[] =
			{
				program->shape_list[program->selected[0]].color.r,
				program->shape_list[program->selected[0]].color.g,
				program->shape_list[program->selected[0]].color.b,
				program->shape_list[program->selected[0]].color.a,
			};

			ImGui::ColorPicker3("Cor", color, 
				ImGuiColorEditFlags_NoAlpha | 
				ImGuiColorEditFlags_NoSidePreview);
			program->shape_list[program->selected[0]].color.r = color[0];
			program->shape_list[program->selected[0]].color.g = color[1];
			program->shape_list[program->selected[0]].color.b = color[2];
			program->shape_list[program->selected[0]].color.a = color[3];

#ifdef DEBUG
			ImGui::Text("locked: %d", program->shape_list[program->selected[0]].locked);
#endif
		}

		ImGui::End();
	}
}