#pragma once

// --------------------------------------------------------------------------------------------------------------------

struct sgp_vec2;

struct shape_t;

struct program_state_t;

// --------------------------------------------------------------------------------------------------------------------

const void add_triangle(program_state_t *program);

const static float sign(const float *p1, const float *p2, const float *p3);

const bool point_is_inside_triangle(const sgp_vec2 pt, const shape_t triangle, const float point_tolerance = 30.0f);

const void triangle_move(shape_t *tri, const sgp_vec2 new_position);

const void draw_triangles(const program_state_t *program);

// --------------------------------------------------------------------------------------------------------------------
