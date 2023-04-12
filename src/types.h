#ifndef TYPES_H
#define TYPES_H

typedef unsigned long long id_t;

typedef struct triangle_t
{
	id_t id;
	float a[2];
	float b[2];
	float c[2];
	float color[4];
} triangle_t;

#endif