#include "vec.h"

inline Vec3 Vec3_scale(Vec3 v, double t){ return VEC3(v.x * t, v.y * t, v.z * t);}

inline Vec3 Vec3_neg(Vec3 v){ return VEC3(-v.x, -v.y, -v.z); }

inline Vec3 Vec3_add(Vec3 a, Vec3 b){ return VEC3(a.x + b.x, a.y + b.y, a.z + b.z); }

inline Vec3 Vec3_sub(Vec3 a, Vec3 b){ return VEC3(a.x - b.x, a.y - b.y, a.z - b.z); }

inline Vec3 Vec3_mult(Vec3 a, Vec3 b){ return VEC3(a.x * b.x, a.y * b.y, a.z * b.z); }

inline double Vec3_dot(Vec3 a, Vec3 b){ return a.x * b.x + a.y * b.y + a.z * b.z; }

inline Vec3 cross(Vec3 a , Vec3 b) {
	return VEC3(a.A[1] * b.A[2] - a.A[2] * b.A[1],
				a.A[2] * b.A[0] - a.A[0] * b.A[2],
				a.A[0] * b.A[1] - a.A[1] * b.A[0]);
}

inline Vec3 Vec3_normalize(Vec3 v){
	double len = VEC3_LENGTH(v);
	return VEC3(v.x / len, v.y / len, v.z / len);
}

Vec3 unit_sphere_random_point(){
	while(true){
		Point3 p = VEC3_RANDOM_RANGE(-1, 1);
		if(VEC3_LENGTH_SQUARED(p) >= 1) continue;
		return p;
	}
}
