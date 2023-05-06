#pragma once

// --------------------------------------------------------------------------------------------------------------------

struct sgp_vec2;

struct shape_t;

// --------------------------------------------------------------------------------------------------------------------

const sgp_vec2 point(const float x, const float y);

const float point_distance(const sgp_vec2 p1, const sgp_vec2 p2);

const sgp_vec2 point_difference(const sgp_vec2 p1, const sgp_vec2 p2);

const sgp_vec2 point_sum(const sgp_vec2 p1, const sgp_vec2 p2);

const void point_move(shape_t *triangle, const int index, const sgp_vec2 amount);

const void point_set(shape_t *teiangle, const int index, const sgp_vec2 new_position);

const sgp_vec2 operator-(const sgp_vec2 other);

// --------------------------------------------------------------------------------------------------------------------
