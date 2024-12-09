#include "spheres.h"
#include "vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void worldInit(World *world) {

	world->spheres = malloc(sizeof(Sphere *) * 1);

	if (world->spheres == NULL) {

		printf("worldInit malloc failure\n");

		exit(1);

	}

	world->size = 0;

	world->capacity = 1;

}

Sphere *createSphere(float radius, Vec3 position, Vec3 color) {

	Sphere *sphere_p = malloc(sizeof(Sphere));

	if (sphere_p == NULL) {

		printf("createSphere malloc failure\n");

		exit(1);

	}

	sphere_p->r = radius;
	
	sphere_p->pos = position;

	sphere_p->color = color;

	return sphere_p;

}

void addSphere(World *world, Sphere *sphere){

	if (world->size >= world->capacity) {

		int new_capacity = world->capacity * 2;

		Sphere **temp = realloc(world->spheres, new_capacity * sizeof(Sphere *));

		if (temp == NULL) {

			printf("realloc failure addSphere\n");

			exit(1);

		}

		world->spheres = temp;

		world->capacity = new_capacity;

	}

	world->spheres[world->size] = sphere;

	world->size += 1;

}


void freeWorld(World *world){


	for (int i = 0; i < world->size; i++){

		free(world->spheres[i]);

	}

	free(world->spheres);

}

int doesIntersect(const Sphere *sphere, Vec3 rayPos, Vec3 rayDir, float *t) {

	Vec3 rs_pos_diff = subtract(rayPos, sphere->pos);

	float a = dot(rayDir, rayDir);

	float b = 2.0f * dot(rayDir, rs_pos_diff);

	float c = dot(rs_pos_diff, rs_pos_diff) - ((sphere->r) * (sphere->r));
	
	float disc = (b*b) - (4.0f*a*c);

	float t1, t2;

	if (disc < 0.0f) {

		return 0;

	}

	if (disc == 0.0f) {

		*t = -b/(2.0f*a);

		return 1;

	}
	
	t1 = (float) (-b + sqrt(disc))/(2.0f*a);
	t2 = (float) (-b - sqrt(disc))/(2.0f*a);
	
	if (t1 > 0 && t2 > 0) {

		if (t1 < t2) {

			*t = t1;

			return 1;

		}

		else {

			*t = t2;

			return 1;

		}

    	} 
    	
	else if (t1 > 0) {

  
  		*t = t1;
        	return 1;
	
    	} 
    
    	else if (t2 > 0) {

    
       		*t = t2;
        	return 1;
    	}

	return 0;
}

