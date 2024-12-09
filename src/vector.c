#include <math.h>
#include "vector.h"

Vec3 add(Vec3 v1, Vec3 v2) {

	Vec3 v_sum;

	v_sum.x = v1.x + v2.x;
	v_sum.y = v1.y + v2.y;
	v_sum.z = v1.z + v2.z;
	
	return v_sum;

}

Vec3 subtract(Vec3 v1, Vec3 v2) {

	Vec3 v_diff;

	v_diff.x = v1.x - v2.x;
	v_diff.y = v1.y - v2.y;
	v_diff.z = v1.z - v2.z;

	return v_diff;

}

Vec3 scalarMultiply(float s, Vec3 v) {

	Vec3 v_scaled;

	v_scaled.x = (v.x) * s;
	v_scaled.y = (v.y) * s;
	v_scaled.z = (v.z) * s;

	return v_scaled;

}

Vec3 scalarDivide(Vec3 v, float d) {

	Vec3 v_scaled;

	v_scaled.x = (v.x) / d;
	v_scaled.y = (v.y) / d;
	v_scaled.z = (v.z) / d;

	return v_scaled;

}

float dot(Vec3 v1, Vec3 v2) {

	float dot_result;

	dot_result = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);

	return dot_result;

}

float length(Vec3 v) {

	float v_length = (float) sqrt(dot(v,v));

	return v_length;

}

float length2(Vec3 v) {

	float v_length_squared = dot(v,v);

	return v_length_squared;

}

float distance(Vec3 v1, Vec3 v2) {

	Vec3 v_diff = subtract(v1,v2);

	float v_distance = length(v_diff);

	return v_distance;

}

float distance2(Vec3 v1, Vec3 v2) {

	float v_distance_squared;

	Vec3 v_diff = subtract(v1,v2);

	v_distance_squared = length2(v_diff);

	return v_distance_squared;

}

Vec3 normalize(Vec3 v) {

	Vec3 v_norm;

	float v_length = length(v);

	v_norm = scalarDivide(v, v_length);

	return v_norm;

}




