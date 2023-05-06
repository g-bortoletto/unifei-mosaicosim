#include "gui.h"

#include "types.h"

#include "image.h"

#include "triangle.h"

#include "../include/sokol/sokol_app.h"

#include "../include/imgui/imgui.h"

void draw_main_menu_bar(program_state_t *program)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Arquivo"))
		{
			if (ImGui::MenuItem("Abrir imagem...", "CTRL+O")) 
			{
				open_image(&program->main_image);
			}
			if (ImGui::MenuItem("Sair", "ALT+F4")) { sapp_request_quit(); }
			ImGui::EndMenu();
		}
		// if (ImGui::BeginMenu("Editar"))
		// {
		// 	if (ImGui::MenuItem("Desfazer", "CTRL+Z")) {}
		// 	if (ImGui::MenuItem("Refazer", "CTRL+Y", false, false)) {}  // Disabled item
		// 	ImGui::Separator();
		// 	if (ImGui::MenuItem("Recortar", "CTRL+X")) {}
		// 	if (ImGui::MenuItem("Copiar", "CTRL+C")) {}
		// 	if (ImGui::MenuItem("Colar", "CTRL+V")) {}
		// 	ImGui::EndMenu();
		// }
		program->main_menu_bar_height = ImGui::GetWindowSize().y;
		ImGui::EndMainMenuBar();
	}
}

void draw_debug_window(program_state_t *program)
{
	ImGui::SetNextWindowPos(ImVec2(program->viewport_x, program->viewport_y));
	ImGui::SetNextWindowSize(ImVec2(200, 0));
	if (ImGui::Begin("Debug Window"))
	{
		ImGui::Text("frame_time: %.6f", sapp_frame_duration());
		ImGui::Text("is_mouse_moving: %s", program->is_mouse_moving ? "true" : "false");
		ImGui::Text("is_mouse_left_down: %s", program->is_mouse_left_down ? "true" : "false");
		ImGui::Text("is_mouse_right_down: %s", program->is_mouse_right_down ? "true" : "false");
		ImGui::Text("is_mouse_in_viewport: %s", program->is_mouse_in_viewport ? "true" : "false");
		ImGui::Text("mouse_position: (%.2f, %.2f)", program->mouse_position.x, program->mouse_position.y);
		ImGui::Text("mouse_delta: (%.2f, %.2f)", program->mouse_delta.x, program->mouse_delta.y);
		ImGui::Text("shape_lock: %s", program->shape_lock ? "true" : "false");
		ImGui::Text("point_lock: %d", program->point_lock);
		ImGui::Text("id_counter: %lld", program->id_counter);
		ImGui::Text("selected: %lld", program->selected);
		ImGui::Text("last_selected: %lld", program->last_selected);
		ImGui::Text(
			"selection:\n\tx: %.1f\n\ty: %.1f\n\tw: %.1f\n\th: %.1f", 
			program->selection.x,
			program->selection.y,
			program->selection.w,
			program->selection.h);
		ImGui::Text("draw_selection: %s", program->draw_selection ? "true" : "false");
		ImGui::Text("main_menu_bar_height: %d", program->main_menu_bar_height);
		ImGui::Text("side_bar_width: %d", program->side_bar_width);
		ImGui::Text(
			"window_size: (%d, %d)", 
			program->window_width, 
			program->window_height);
		ImGui::Text("window_ratio: %.2f", program->window_ratio);
		ImGui::Text(
			"viewport_pos: (%d, %d)", 
			program->viewport_x, 
			program->viewport_y);
		ImGui::Text("viewport_size: (%d, %d)", program->viewport_width,program->viewport_height);
		ImGui::Text("viewport_ratio: %.2f", program->viewport_ratio);
		ImGui::Text("shape_list.size(): %lld", program->shape_list.size());
		ImGui::Text("offset: (%2f, %2f)", program->offset.x, program->offset.y);
		ImGui::InputFloat("zoom", &program->zoom, 0.1f, 1.0f);
		ImGui::End();
	}
}

// --------------------------------------------------------------------------------------------------------------------

void draw_side_bar(program_state_t *program)
{
	bool sidebar_open = true;
	ImGui::SetNextWindowPos(ImVec2(0, program->main_menu_bar_height));
	ImGui::SetNextWindowSize(ImVec2(program->side_bar_width, sapp_height() - program->main_menu_bar_height));
	if (ImGui::Begin("sidebar", &sidebar_open,
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize 
|       ImGuiWindowFlags_NoTitleBar))
	{
		if (ImGui::Button("Carregar imagem", ImVec2(185.0f, 0.0f)))     { open_image(&program->main_image); }
		if (ImGui::Button("Adicionar triângulo", ImVec2(185.0f, 0.0f))) { add_triangle(program); }	
		if (program->shape_list.contains(program->selected))
		{
			float np0[2] = { program->shape_list[program->selected].p[0].x, program->shape_list[program->selected].p[0].y };
			ImGui::InputFloat2("A", np0);
			program->shape_list[program->selected].p[0].x = np0[0];
			program->shape_list[program->selected].p[0].y = np0[1];
			
			float np1[2] = { program->shape_list[program->selected].p[1].x, program->shape_list[program->selected].p[1].y };
			ImGui::InputFloat2("B", np1);
			program->shape_list[program->selected].p[1].x = np1[0];
			program->shape_list[program->selected].p[1].y = np1[1];
			
			float np2[2] = { program->shape_list[program->selected].p[2].x, program->shape_list[program->selected].p[2].y };
			ImGui::InputFloat2("C", np2);
			program->shape_list[program->selected].p[2].x = np2[0];
			program->shape_list[program->selected].p[2].y = np2[1];
			
			float c[4] = 
			{
				program->shape_list[program->selected].color.r,
				program->shape_list[program->selected].color.g,
				program->shape_list[program->selected].color.b,
				program->shape_list[program->selected].color.a,
			};
			ImGui::ColorPicker3("Cor", c, 
				ImGuiColorEditFlags_NoAlpha | 
				ImGuiColorEditFlags_NoSidePreview);
			program->shape_list[program->selected].color.r = c[0];
			program->shape_list[program->selected].color.g = c[1];
			program->shape_list[program->selected].color.b = c[2];
			program->shape_list[program->selected].color.a = c[3];
		}

		ImGui::End();
	}
}

// --------------------------------------------------------------------------------------------------------------------

void draw_selection(const program_state_t *program)
{
	
}