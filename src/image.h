#pragma once

struct sg_image;

struct program_state_t;

sg_image load_image(const char *filename);

void open_image(sg_image *main_image);

void draw_main_image(program_state_t *program);
