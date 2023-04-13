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
		ImGui::InputInt("ID", (int *)(&program->selected));
		int tri_index = get_triangle(program);
		if (tri_index >= 0)
		{
			ImGui::InputFloat2("A", program->tri_list[(id_t)tri_index].a);
			ImGui::InputFloat2("B", program->tri_list[(id_t)tri_index].b);
			ImGui::InputFloat2("C", program->tri_list[(id_t)tri_index].c);
			ImGui::ColorPicker3("Cor", program->tri_list[(id_t)tri_index].color, 
				ImGuiColorEditFlags_NoAlpha | 
				ImGuiColorEditFlags_NoSidePreview);
		}
		ImGui::End();
	}
}

// --------------------------------------------------------------------------------------------------------------------