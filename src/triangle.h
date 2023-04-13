#pragma once

typedef unsigned long long id_t;

struct triangle_t;

struct program_state_t;

void add_triangle(program_state_t *program);

long long get_triangle(program_state_t *program);

const static float sign(const float *p1, const float *p2, const float *p3);

const bool point_is_inside_triangle(const float *pt, const triangle_t triangle);

void draw_triangles(const program_state_t *program);