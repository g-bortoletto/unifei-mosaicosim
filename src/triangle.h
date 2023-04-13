#pragma once

typedef unsigned long long id_t;

struct triangle_t;

struct program_state_t;

void add_triangle(program_state_t *program);

long long get_triangle(program_state_t *program);

static float sign(float *p1, float *p2, float *p3);

bool point_is_inside_triangle(float *pt, triangle_t triangle);

void draw_triangles(program_state_t *program);