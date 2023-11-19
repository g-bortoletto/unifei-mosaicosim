#include "tfgBackgroundImage.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

//#include <nfd/nfd.h>

#include <sokol/sokol_gp.h>


BackgroundImage::BackgroundImage()
{
	img = { SG_INVALID_ID };
}

BackgroundImage::~BackgroundImage()
{
	// TODO(guilherme): investigar esse bug.
	/*if (img.id != SG_INVALID_ID)
	{
		sg_destroy_image(img);
	}*/
}

void BackgroundImage::Init()
{
}

void BackgroundImage::Frame()
{
	if (!hide && img.id != SG_INVALID_ID && !path.empty())
	{
		sgp_set_image(0, img);
		sgp_draw_textured_rect(
			0.0f,
			0.0f,
			w,
			h);
		sgp_reset_image(0);
	}
}

void BackgroundImage::Cleanup()
{
}

void BackgroundImage::Input(const sapp_event *e)
{
}

void BackgroundImage::LoadBackgroundImage(void)
{ 
	char *filename = 0;
	if (path.empty())
	{
		//nfdresult_t result = NFD_OpenDialog("png;jpg;jpeg", 0, &filename);
		//if (result == NFD_ERROR || result == NFD_CANCEL) return;
		//path = std::string(filename);
	}

	if (path.empty())
	{
		return;
	}

	int channels = 0;
	unsigned char *data = stbi_load(path.empty() ? filename : path.c_str(), &w, &h, &channels, 4);
	sg_image_desc image_desc =
	{
		.width = w,
		.height = h,
		.min_filter = SG_FILTER_LINEAR,
		.mag_filter = SG_FILTER_LINEAR,
		.wrap_u = SG_WRAP_CLAMP_TO_EDGE,
		.wrap_v = SG_WRAP_CLAMP_TO_EDGE,
	};
	image_desc.data.subimage[0][0].ptr = data;
	image_desc.data.subimage[0][0].size = (size_t)(w * h * 4);
	img = sg_make_image(&image_desc);
	stbi_image_free(data);
	
	if (path.empty())
	{
		path = std::string(filename);
	}
}
