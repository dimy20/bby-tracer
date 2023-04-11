#pragma once

#include "vec.h"
#include "utils.h"

typedef struct Material Material;

typedef enum{
	OBJ_SPHERE
}Obj_type;

typedef struct{
	Point3 origin;
	Vec3 direction;
}Ray;

typedef struct{
	Point3 p;
	Vec3 normal;
	double t;
	bool front_face;
	Material * material;
	Color attenuation;
}Hit_record;

