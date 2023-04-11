#include "material.h"

bool lambertian_scatter(const Material * material, const Ray * incoming_ray, const Hit_record* rec, Ray * out_ray);
bool metal_scatter(const Material * material, const Ray * incoming_ray, const Hit_record* rec, Ray * out_ray);

static Scatter_fn material_scatter[MATERIALS_NUM] = {
	lambertian_scatter,
	metal_scatter
};

bool lambertian_scatter(const Material * material, const Ray * incoming_ray, const Hit_record* rec, Ray * out_ray){
	// aint this a point?
	Vec3 target = Vec3_add(rec->normal, VEC3_RAND_UNIT);

	Vec3 scatter_direction = VEC3_NEAR_ZERO(target) ? rec->normal : Vec3_sub(target, rec->p);
	out_ray->origin = rec->p;
	out_ray->direction = scatter_direction;

	return true;
}

bool metal_scatter(const Material * material, const Ray * incoming_ray, const Hit_record* rec, Ray * out_ray){
	out_ray->origin = rec->p;

	Vec3 reflection = Vec3_reflect(incoming_ray->direction, rec->normal);
	Vec3 fuzziness = Vec3_scale(VEC3_RAND_UNIT, material->fuzz);

	out_ray->direction = Vec3_add(reflection, fuzziness);

	return Vec3_dot(out_ray->direction, rec->normal) > 0; // same side
}

Material * create_material(Material_type type){
	Material * mat = malloc(sizeof(Material));

	assert(mat != NULL);
	assert(type >=0 && type < MATERIALS_NUM);

	mat->scatter = material_scatter[type];
	return mat;
}

void destroy_material(Material * mat){
	if(mat != NULL){
		free(mat);
	}
}

void initialize_materials(){}
