#pragma once

#include <stdbool.h>
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include "vec.h"
#include "tracer.h"

typedef struct{
	Point3 center;
	double r;
}Sphere;

bool Sphere_hit(const Sphere * sphere,
					   const Ray * ray, double t_min,
					   double t_max, Hit_record * rec);

Sphere * create_sphere(Vec3 center, double r);
void destroy_sphere(Sphere * sphere);
