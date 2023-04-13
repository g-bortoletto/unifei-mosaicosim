#include "image.h"

#include "../include/sokol/sokol_gfx.h"

#include "../include/sokol/sokol_gp.h"

#include "../include/stb/stb_image.h"

#include "../include/nfd/nfd.h"

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
