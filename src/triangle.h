#pragma once

// --------------------------------------------------------------------------------------------------------------------

struct point_t;

struct shape_t;

struct program_state_t;

// --------------------------------------------------------------------------------------------------------------------

const void add_triangle(program_state_t *program);

const static float sign(const float *p1, const float *p2, const float *p3);

const bool point_is_inside_triangle(const point_t pt, const shape_t triangle);

const void triangle_move(shape_t *tri, const point_t new_position);

const void draw_triangles(const program_state_t *program);

// --------------------------------------------------------------------------------------------------------------------
