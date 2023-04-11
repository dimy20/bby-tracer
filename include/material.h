#pragma once

#include "vec.h"
#include "tracer.h"

// Materials
typedef enum{
	MATERIAL_LAMBERTIAN,
	MATERIAL_METAL,

	MATERIALS_NUM
}Material_type;

typedef bool (*Scatter_fn)(const Ray * incoming_ray, const Hit_record* rec, Ray * out_ray);
typedef struct{
	Scatter_fn scatter;
}Material;

void initialize_materials();
