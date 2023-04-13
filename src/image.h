#pragma once

struct sg_image;

sg_image load_image(const char *filename);

void open_image(sg_image *main_image);
