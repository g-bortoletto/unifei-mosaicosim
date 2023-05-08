#include "image.h"

#include "../include/sokol/sokol_gfx.h"

#include "../include/sokol/sokol_gp.h"

#include "../include/stb/stb_image.h"

#include "../include/nfd/nfd.h"

#include "types.h"

// --------------------------------------------------------------------------------------------------------------------

const sg_image main_image_load(const char *filename)
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

void main_image_open(sg_image *main_image)
{
	nfdchar_t *file_path = 0;
	nfdresult_t result = NFD_OpenDialog("png;jpg;jpeg", 0, &file_path);
	if (result == NFD_OKAY)
	{
		(*main_image) = main_image_load(file_path);
	}
}

// --------------------------------------------------------------------------------------------------------------------

void main_image_draw(const program_state *program)
{
	if (program->main_image.id != SG_INVALID_ID)
	{
		sg_image_desc main_image_desc = sg_query_image_desc(program->main_image);
		const float image_ratio = main_image_desc.width / (float)main_image_desc.height;
		sgp_set_image(0, program->main_image);
		// TODO(guilherme): rever isso com a adição de zoom...
		float main_image_width = program->viewport.ratio <= image_ratio ? program->viewport.w : (program->viewport.h * (float)image_ratio);
		float main_image_height = program->viewport.ratio > image_ratio ? program->viewport.h : (program->viewport.w / (float)image_ratio);
		sgp_push_transform();
		sgp_scale(program->scale, program->scale);
		sgp_draw_textured_rect(
			0.0f, 
			0.0f, 
			main_image_width, 
			main_image_height);
		sgp_reset_image(0);
		sgp_pop_transform();
	}
}

// --------------------------------------------------------------------------------------------------------------------
