#include "sphere.h"

extern Vec3 Ray_at(const Ray * ray, double t);

Sphere * create_sphere(Vec3 center, double r, Material * material){
	Sphere * sphere = malloc(sizeof(Sphere));
	assert(sphere != NULL);

	sphere->r = r;
	sphere->center = center;
	sphere->material = material;

	return sphere;
}

bool Sphere_hit(const Sphere * sphere,
					   const Ray * ray, double t_min,
					   double t_max, Hit_record * rec){

	Vec3 co = Vec3_sub(ray->origin, sphere->center);
	double a = VEC3_LENGTH_SQUARED(ray->direction);
	double half_b = Vec3_dot(co, ray->direction);
	double c = VEC3_LENGTH_SQUARED(co) - (sphere->r * sphere->r);

	double D = half_b * half_b - a * c;
	if(D < 0.0) return false;

	double D_sqrt = sqrt(D);

	double solution = (-half_b - D_sqrt) / a;
	if(solution < t_min || solution > t_max){
		solution = (-half_b + D_sqrt) / a;
		if(solution < t_min || solution > t_max){
			return false;
		}
	}

	rec->p = Ray_at(ray, solution);
	rec->t = solution;
	Vec3 outward_normal  = Vec3_normalize(Vec3_sub(rec->p, sphere->center));
	rec->front_face = Vec3_dot(ray->direction, outward_normal) < 0;
	rec->normal = rec->front_face ? outward_normal : Vec3_neg(outward_normal);
	//rec->attenuation = sphere->attenuation;
	rec->material = sphere->material;

	return true;
}


void destroy_sphere(Sphere * sphere){
	free(sphere);
}
