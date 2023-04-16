#pragma once

// --------------------------------------------------------------------------------------------------------------------

struct point_t;

struct shape_t;

struct program_state_t;

// --------------------------------------------------------------------------------------------------------------------

const void add_triangle(program_state_t *program);

const static float sign(const float *p1, const float *p2, const float *p3);

const bool point_is_inside_triangle(const point_t pt, const shape_t triangle);

const void draw_triangles(const program_state_t *program);

// --------------------------------------------------------------------------------------------------------------------
