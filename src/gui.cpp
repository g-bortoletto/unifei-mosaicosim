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
		ImGui::Text("id: %lld", program->selected);
		ImGui::SameLine();
		ImGui::Text("l_id: %lld", program->last_selected);
		
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