#pragma once

// --------------------------------------------------------------------------------------------------------------------

struct point_t;

struct shape_t;

// --------------------------------------------------------------------------------------------------------------------

const point_t point(const float x, const float y);

const float point_distance(const point_t p1, const point_t p2);

const point_t point_difference(const point_t p1, const point_t p2);

const point_t point_sum(const point_t p1, const point_t p2);

const void point_move(shape_t *triangle, const int index, const point_t amount);

const void point_set(shape_t *teiangle, const int index, const point_t new_position);

const point_t operator-(const point_t other);

// --------------------------------------------------------------------------------------------------------------------
