#pragma once

#include <stdbool.h>
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include "vec.h"
#include "tracer.h"
#include "material.h"

typedef struct{
	Point3 center;
	double r;
	Material_type material;
	Color attenuation;
}Sphere;

bool Sphere_hit(const Sphere * sphere,
					   const Ray * ray, double t_min,
					   double t_max, Hit_record * rec);

Sphere * create_sphere(Vec3 center, double r, Material_type material_type, Color attenuation);
void destroy_sphere(Sphere * sphere);
