#include "image.h"

#include "../include/sokol/sokol_gfx.h"

#include "../include/sokol/sokol_gp.h"

#include "../include/stb/stb_image.h"

#include "../include/nfd/nfd.h"

#include "types.h"

// --------------------------------------------------------------------------------------------------------------------

sg_image load_image(const char *filename)
{
	int width, height, channels;
	sg_image img = { SG_INVALID_ID };
	unsigned char *data = stbi_load(filename, &width, &height, &channels, 4);
	sg_image_desc image_desc = 
	{
		.width = width,
		.height = height,
		.min_filter = SG_FILTER_LINEAR,
		.mag_filter = SG_FILTER_LINEAR,
		.wrap_u = SG_WRAP_CLAMP_TO_EDGE,
		.wrap_v = SG_WRAP_CLAMP_TO_EDGE,
	};
	image_desc.data.subimage[0][0].ptr = data;
	image_desc.data.subimage[0][0].size = (size_t)(width * height * 4);
	img = sg_make_image(&image_desc);
	stbi_image_free(data);    
	return img;
}

// --------------------------------------------------------------------------------------------------------------------

void open_image(sg_image *main_image)
{
	nfdchar_t *file_path = 0;
	nfdresult_t result = NFD_OpenDialog("png;jpg;jpeg", 0, &file_path);
	if (result == NFD_OKAY)
	{
		(*main_image) = load_image(file_path);
	}
}

// --------------------------------------------------------------------------------------------------------------------

void draw_main_image(program_state_t *program)
{
	if (program->main_image.id != SG_INVALID_ID)
	{
		sg_image_desc main_image_desc = sg_query_image_desc(program->main_image);
		const float image_ratio = main_image_desc.width / (float)main_image_desc.height;
		sgp_set_image(0, program->main_image);
		float main_image_width = program->viewport_ratio <= image_ratio ? program->viewport_width : (program->viewport_height * (float)image_ratio);
		float main_image_height = program->viewport_ratio > image_ratio ? program->viewport_height : (program->viewport_width / (float)image_ratio);
		sgp_draw_textured_rect(
			program->viewport_x, 
			program->viewport_y, 
			main_image_width, 
			main_image_height);
		sgp_reset_image(0);

#ifdef DEBUG
		if (ImGui::Begin("Main image debug"))
		{
			ImGui::Text("image width:     %f", main_image_width);
			ImGui::Text("image height:    %f", main_image_height);
			ImGui::Text("image ratio:     %f", image_ratio);
			ImGui::Text("viewport width:  %d", viewport_width);
			ImGui::Text("viewport height: %d", viewport_height);
			ImGui::Text("viewport ratio:  %f", viewport_ratio);
			ImGui::End();
		}
#endif
	}
}

// --------------------------------------------------------------------------------------------------------------------
