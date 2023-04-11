#include "material.h"

Material materials[MATERIALS_NUM];

bool lambertian_scatter(const Ray * incoming_ray, const Hit_record* rec, Ray * out_ray){
	// aint this a point?
	Vec3 target = Vec3_add(rec->normal, VEC3_RAND_UNIT);

	Vec3 scatter_direction = VEC3_NEAR_ZERO(target) ? rec->normal : Vec3_sub(target, rec->p);
	out_ray->origin = rec->p;
	out_ray->direction = scatter_direction;

	return true;
}

bool metal_scatter(const Ray * incoming_ray, const Hit_record* rec, Ray * out_ray){
	out_ray->origin = rec->p;
	out_ray->direction = Vec3_reflect(incoming_ray->direction, rec->normal);
	return Vec3_dot(out_ray->direction, rec->normal) > 0; // same side
}

void initialize_materials(){
	materials[MATERIAL_LAMBERTIAN].scatter = lambertian_scatter;
	materials[MATERIAL_METAL].scatter = metal_scatter;
}
