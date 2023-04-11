#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <float.h>

#include "tracer.h"
#include "sphere.h"

#define RT_DEF inline static
#define MAX_SCENE_OBJS 50
#define MAX_DEPTH 50
#define SAMPLES_PER_PIXEL 50 // for antialiasing

const Vec3 WHITE = COLOR(1, 1, 1);
const Vec3 BLUE = COLOR(0.5, 0.7, 1.0);
// image stuff
const int img_w = 800;
const double aspect_ratio = 16.0 / 9.0;
const int img_h = (int)((double)img_w / aspect_ratio);


//Camera
typedef struct{
	double viewport_h;
	double viewport_w;
	double focal_length;

	Point3 origin;
	Vec3 horizontal;
	Vec3 vertical;
	Point3 lower_left_corner;
}Camera;

typedef struct{
	Obj_type type;
	void * obj;
}Obj;

// gobals
Camera * camera;
Obj scene_objs[MAX_SCENE_OBJS];
int objs_count = 0;
extern Material materials[MATERIALS_NUM];


Point3 Ray_at(const Ray* ray, double t);
RT_DEF bool Scene_ray_hits_obj(const Ray * ray, double t_min, double t_max, Hit_record * rec);

RT_DEF void write_color(Color pixel_color){
	uint8_t r, g, b;
	double tmp_r, tmp_g, tmp_b;

	// divide the pixel color by the number of samples and gamma correct for gamma = 2.0
	tmp_r = sqrt(clamp(pixel_color.x / SAMPLES_PER_PIXEL, 0, 0.999));
	tmp_g = sqrt(clamp(pixel_color.y / SAMPLES_PER_PIXEL, 0, 0.999));
	tmp_b = sqrt(clamp(pixel_color.z / SAMPLES_PER_PIXEL, 0, 0.999));

	r = (uint8_t)(255 * tmp_r);
	g = (uint8_t)(255 * tmp_g);
	b = (uint8_t)(255 * tmp_b);

	fprintf(stdout, "%d %d %d\n", r, g, b);
}

Camera * create_camera(const double aspect_ratio){
	Camera * camera = malloc(sizeof(Camera));
	assert(camera != NULL);

	camera->viewport_h = 2.0;
	camera->viewport_w = aspect_ratio * camera->viewport_h;
	camera->focal_length = 1.0;

	camera->origin = VEC3_ZERO;
	camera->horizontal = VEC3(camera->viewport_w, 0, 0);
	camera->vertical = VEC3(0, camera->viewport_h, 0);


	Vec3 half_horizontal = Vec3_scale(camera->horizontal, 0.5);
	Vec3 half_vertical  = Vec3_scale(camera->vertical, 0.5);

	Vec3 tmp = Vec3_sub(camera->origin, half_horizontal);
	tmp = Vec3_sub(tmp, half_vertical);
	tmp = Vec3_sub(tmp, VEC3(0, 0, camera->focal_length));

	camera->lower_left_corner = tmp;

	return camera;
}

Point3 camera_point(double u, double v){
	assert(camera != NULL);

	Point3 p = VEC3_ZERO;

	Vec3 u_component = Vec3_scale(camera->horizontal, u);
	Vec3 v_component = Vec3_scale(camera->vertical, v);

	p = Vec3_add(camera->lower_left_corner, u_component);
	p = Vec3_add(p, v_component);

	return p;
}


RT_DEF Vec3 lerp_color(Color start, Color end, double t){
	Vec3 lerp_factor = Vec3_scale(Vec3_sub(end, start), t);
	return Vec3_add(start, lerp_factor);
}

Color ray_color(const Ray* ray, int depth){
	Hit_record rec;

	if(depth <= 0){
		return COLOR(0, 0, 0);
	}

	if(Scene_ray_hits_obj(ray, 0.001, DBL_MAX, &rec)){
		Point3 target = Vec3_add(rec.p, rec.normal);
		target = Vec3_add(target, VEC3_RAND_UNIT);

		Ray scatter_ray;
		scatter_ray.origin = rec.p;
		scatter_ray.direction = Vec3_sub(target, rec.p);

		if(materials[rec.material].scatter(ray, &rec, &scatter_ray)){
			return Vec3_mult(ray_color(&scatter_ray, depth - 1), rec.attenuation);
		}
		return COLOR(0, 0, 0);
		//Vec3 color = COLOR(rec.normal.x + 1.0, rec.normal.y + 1.0, rec.normal.z + 1.0);

	}
	Vec3 dir = Vec3_normalize(ray->direction);
	double t = 0.5 * (dir.y + 1.0);
	return lerp_color(WHITE, BLUE, t);
}

Point3 Ray_at(const Ray* ray, double t){
	return Vec3_add(ray->origin, Vec3_scale(ray->direction, t));
}

bool Scene_ray_hits_obj(const Ray * ray, double t_min, double t_max, Hit_record * rec){
	Hit_record tmp_rec;
	bool hit = false;
	double curr_max = t_max;
	for(int i = 0; i < objs_count; i++){
		if(scene_objs[i].type == OBJ_SPHERE){
			Sphere * sphere = (Sphere *)scene_objs[i].obj;
			assert(sphere != NULL);

			if(Sphere_hit(sphere, ray, t_min, curr_max, &tmp_rec)){
				hit = true;
				curr_max = tmp_rec.t;
				*rec = tmp_rec;
			}
		}
	}
	return hit;
}

RT_DEF void renderer_add_sphere(Vec3 center, double r, int material, Color attenuation){
	if(objs_count + 1 < MAX_SCENE_OBJS){
		Obj * obj = &scene_objs[objs_count++];
		obj->type = OBJ_SPHERE;
		obj->obj = create_sphere(center, r, material, attenuation);
	}
}

RT_DEF void renderer_destroy_objs(){
	for(int i = 0; i < objs_count; i++){
		if(scene_objs[i].obj != NULL){
			if(scene_objs[i].type == OBJ_SPHERE){
				destroy_sphere(scene_objs[i].obj);
			}
		}
	}
}

int main(){
	memset(scene_objs, 0, sizeof(Obj));
	initialize_materials();

	Color ground = COLOR(0.8, 0.8, 0.0);
	// ground
	renderer_add_sphere(VEC3(0, -100.5, -1), 100, MATERIAL_LAMBERTIAN, ground);
	// center sphere
	renderer_add_sphere(VEC3(0, 0, -1), 0.5, MATERIAL_LAMBERTIAN, COLOR(0.7, 0.3, 0.3));
	// metal left
	renderer_add_sphere(VEC3(-1.0, 0, -1), 0.5, MATERIAL_METAL, COLOR(0.8, 0.8, 0.8));
	// metal right
	renderer_add_sphere(VEC3(1.0, 0, -1), 0.5, MATERIAL_METAL, COLOR(0.8, 0.6, 0.2));

	// Camera
	camera = create_camera(aspect_ratio);

	fprintf(stdout, "P3\n%d %d\n255\n", img_w, img_h);
	for(int y = img_h -1; y >= 0; y--){
		fprintf(stderr, "Scanlines remaining: %d\n", y);
		for(int x = 0; x < img_w; x++){
			Color pixel_color = COLOR(0, 0, 0);

			for(int s = 0; s < SAMPLES_PER_PIXEL; s++){
				double u = (double)(x + rand_d()) / (double)(img_w - 1);
				double v = (double)(y + rand_d()) / (double)(img_h - 1);
				Ray ray;
				ray.origin = camera->origin;
				ray.direction = Vec3_sub(camera_point(u, v), camera->origin);

				pixel_color = Vec3_add(pixel_color, ray_color(&ray, MAX_DEPTH));
			}
			write_color(pixel_color);
		}
	}
	fprintf(stderr, "\nDone.\n");

	free(camera);
	renderer_destroy_objs();
}
