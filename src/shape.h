#ifndef SHAPE_H
#define SHAPE_H

// --------------------------------------------------------------------------------------------------------------------

struct sgp_vec2;

typedef sgp_vec2 v2f;

struct shape;

struct program_state;

// --------------------------------------------------------------------------------------------------------------------

static void shape_add(program_state *program);

// POINT --------------------------------------------------------------------------------------------------------------

const float point_distance(const v2f a, const v2f b);

static void point_set(v2f *p, const v2f new_position);

void point_move(v2f *p, const v2f offset);

static const v2f point_difference(const v2f a, const v2f b);

static const v2f point_sum(const v2f a, const v2f b);

void point_draw(const v2f p);

// TRIANGLE -----------------------------------------------------------------------------------------------------------

const void triangle_add(program_state *program);

const static float triangle_sign_util(const float *p1, const float *p2, const float *p3);

const bool triangle_has_point_inside(const v2f point, const shape triangle, const float point_tolerance = 30.0f);

const void triangle_move(shape *tri, const v2f new_position);

const void triangles_update(program_state *program);

const void triangles_draw(program_state *program);

const static void triangle_highlight_hot(const shape *tri);

const static void triangle_highlight_selected(const shape *tri);

// --------------------------------------------------------------------------------------------------------------------

#endif