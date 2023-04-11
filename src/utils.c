#include "utils.h"
double rand_d(){ return rand() / (RAND_MAX + 1.0); }
double rand_d_range(double min, double max){ return min + (max - min) * rand_d(); };
double clamp(double x, double min, double max){
	if(x < min) return min;
	if(x > max) return max;
	return x;
}
