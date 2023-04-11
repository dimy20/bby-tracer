#pragma once

#include "vec.h"
#include "tracer.h"
#include <assert.h>

// Materials
typedef enum{
	MATERIAL_LAMBERTIAN,
	MATERIAL_METAL,

	MATERIALS_NUM
}Material_type;

typedef struct Material Material;

typedef bool (*Scatter_fn)(const Material * material, const Ray * incoming_ray, const Hit_record* rec, Ray * out_ray);

typedef struct Material{
	double fuzz;
	Color attenuation;
	Material_type type;
	Scatter_fn scatter;
}Material;


Material * create_material(Material_type);
void destroy_material(Material * material);
void initialize_materials();
