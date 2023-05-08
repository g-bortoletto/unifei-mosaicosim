#ifndef IMAGE_H
#define IMAGE_H

// -------------------------------------------------------------------------------------------------

struct sg_image;

struct program_state;

// -------------------------------------------------------------------------------------------------

const sg_image main_image_load(const char *filename);

void main_image_open(sg_image *main_image);

void main_image_draw(const program_state *program);

// -------------------------------------------------------------------------------------------------

#endif