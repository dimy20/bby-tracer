#pragma once
#include <math.h>

typedef struct{
	union{
		struct{
			double x, y, z;
		};
		double A[3];
	};
}Vec3;

typedef Vec3 Color;
typedef Vec3 Point3;

#define VEC3(x, y, z) ((Vec3){ .A[0] = (x), .A[1] = (y), .A[2] = (z) })
#define VEC3_ZERO VEC3(0, 0, 0)
#define COLOR(r, g, b) VEC3((r), (g), (b))
#define VEC3_PRINT(v) do{ printf("(%f, %f, %f)\n", v.x, v.y, v.z); } while(0);
#define VEC3_LENGTH(v) ({ sqrt(v.x * v.x + v.y * v.y + v.z * v.z); })
#define VEC3_LENGTH_SQUARED(v) ({ v.x * v.x + v.y * v.y + v.z * v.z; })

Vec3 Vec3_scale(Vec3 v, double t);
Vec3 Vec3_neg(Vec3 v);
Vec3 Vec3_add(Vec3 a, Vec3 b);
Vec3 Vec3_sub(Vec3 a, Vec3 b);
Vec3 Vec3_mult(Vec3 a, Vec3 b);
double Vec3_dot(Vec3 a, Vec3 b);
Vec3 Vec3_normalize(Vec3 v);
