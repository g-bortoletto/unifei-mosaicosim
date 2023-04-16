#pragma once

// --------------------------------------------------------------------------------------------------------------------

struct point_t;

struct shape_t;

// --------------------------------------------------------------------------------------------------------------------

const point_t point(const float x, const float y);

const float point_distance(const point_t p1, const point_t p2);

const void point_move(shape_t *triangle, const int index, const point_t new_position);

// --------------------------------------------------------------------------------------------------------------------
