#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "vector.h"
#include "spheres.h"
#include "color.h"

typedef struct {

	float height;

        float width;

	float z;

}Viewport;

typedef struct {

	Vec3 light_pos;

	float brightness;

}Light;

Vec3 background_color = {0.0f, 0.0f, 0.0f};

Vec3 sphere_color = {255.0f,255.0f,255.0f};

World world;

World *world_p = &world;

Viewport viewport;

Light lightsource;

void cast_rays(int image_w, int image_h, FILE *ppm_file);
Vec3 shading(Vec3 ray_pos, Vec3 ray_dir, float t, Sphere *sphere);
void MS1_output(World *world_p, FILE *output_file);

int main(int argc, char *argv[]) {

	worldInit(world_p);

	// READ COMMAND LINE INPUT
	
	char *input_file_str,*output_file_str;

	input_file_str = argv[1];

	output_file_str = argv[2];

	// READ FROM INPUT FILE

	FILE *input_file = fopen(input_file_str, "r");

	FILE *output_file = fopen(output_file_str, "w");

	int image_width, image_height;

	fscanf(input_file, "%d %d", &image_width, &image_height);

	float viewport_height, focal_length;

	fscanf(input_file, "%f %f", &viewport_height, &focal_length);

	float lightpos_x, lightpos_y, lightpos_z, brightness;

	fscanf(input_file, "%f %f %f %f", &lightpos_x, &lightpos_y, &lightpos_z, &brightness);

	int num_of_colors;
	
	fscanf(input_file, "%d", &num_of_colors);

	unsigned int hexcolor_list[num_of_colors];

	for (int i = 0; i < num_of_colors; i++) {

		unsigned int hex_color;

		fscanf(input_file, "%i", &hex_color);

		hexcolor_list[i] = hex_color;

	}

	qsort(hexcolor_list, num_of_colors, sizeof(unsigned int), compareColor);

	int background_color_ind;

	fscanf(input_file, "%d", &background_color_ind);
	
	#ifdef FS	
	background_color = unpackRGB(hexcolor_list[background_color_ind]);
	#endif	

	int num_of_spheres;

	fscanf(input_file, "%d", &num_of_spheres);

	float s_posx, s_posy, s_posz, s_radius;

	int s_colorind;

	for (int i = 0; i < num_of_spheres; i++) {


		fscanf(input_file, "%f %f %f %f %d", &s_posx, &s_posy, &s_posz, &s_radius, &s_colorind);

		Vec3 sphere_pos = {s_posx, s_posy, s_posz};	

		#ifdef FS
		Vec3 sphere_color = unpackRGB(hexcolor_list[s_colorind]);
		#endif

		Sphere *sphere_p = createSphere(s_radius, sphere_pos, sphere_color);

		addSphere(world_p, sphere_p);

	}


	// VIEWPORT INITIALIZATION
		
	viewport.height = viewport_height;
		
	viewport.width = viewport_height * ((float)image_width/(float)image_height);

	viewport.z = -focal_length;


	// LIGHTSOURCE INITIALIZATION
	

	Vec3 light_pos = {lightpos_x, lightpos_y, lightpos_z};
	
	lightsource.light_pos = light_pos;

	lightsource.brightness = brightness;
	
	#ifdef MS1	
	MS1_output(world_p, output_file);
	#endif
	
	#ifndef MS1
	cast_rays(image_width, image_height, output_file);
	#endif

	
	freeWorld(world_p);

	fclose(input_file);

	fclose(output_file);

	return 0;

}

void cast_rays(int image_w, int image_h, FILE *ppm_file) {

	if (ppm_file == NULL) {

		printf("not opening!\n");

	}


        fprintf(ppm_file,"P3\n");
	fprintf(ppm_file,"%d %d\n", image_w, image_h);
	fprintf(ppm_file,"255\n");

	for (int y = 0; y < image_h; y++) {

		for (int x = 0 ; x < image_w; x++) {

			//printf("v_width = %f, v_height %f", viewport.width, viewport.height);
			
			float x_v = -((float) viewport.width / 2.0) + (x+0.5) * (viewport.width / (float) image_w);
                        float y_v = ((float) viewport.height / 2.0) - (y+0.5) * (viewport.height / (float) image_h);

			Vec3 ray_dir = {x_v, y_v, viewport.z};

			ray_dir = normalize(ray_dir);

			Vec3 ray_pos = {0.0f, 0.0f, 0.0f};

			float t;
			
			float closest_t = -1.0f;

			Sphere *closest_sphere = NULL;

			for (int i = 0; i < world_p->size; i++) {

				Sphere *sphere = world_p->spheres[i];

				int intersection = doesIntersect(sphere, ray_pos, ray_dir, &t);

				if (intersection && (t < closest_t || closest_t == -1.0f)) {

					closest_t = t;
					closest_sphere = sphere;

				}

			}

			if (closest_sphere) {
				
				Vec3 pixel_color = shading(ray_pos, ray_dir, closest_t, closest_sphere);


				writeColour(ppm_file, pixel_color);


			}

			else {

				writeColour(ppm_file, background_color);
			}

		}
	}
}
		
Vec3 shading(Vec3 ray_pos, Vec3 ray_dir, float t, Sphere *sphere) {
		
	     ray_dir = scalarMultiply(t, ray_dir);

	     Vec3 hit_pos = add(ray_pos, ray_dir);

             Vec3 normal = subtract(hit_pos, sphere->pos);

	     normal = normalize(normal);

             Vec3 light_dir = subtract(lightsource.light_pos, hit_pos);

             light_dir = normalize(light_dir);

             float intensity = fmax((dot(light_dir, normal)), 0.0f) / (distance2(hit_pos, lightsource.light_pos));

             intensity = lightsource.brightness * intensity;

	     intensity = fmin(1.0f, intensity);

             Vec3 pixel_color = scalarMultiply(intensity, sphere->color);

	     Vec3 shadow_pos = add(hit_pos, scalarMultiply(0.001f, normal));

	     Vec3 shadow_dir = subtract(lightsource.light_pos, shadow_pos);

	     shadow_dir = normalize(shadow_dir);

	     float t_shadow;

	     float closest_t = -1.0f;

             Sphere *closest_sphere = NULL;

             for (int i = 0; i < world_p->size; i++) {

                     Sphere *sphere = world_p->spheres[i];

                     int intersection = doesIntersect(sphere, shadow_pos, shadow_dir,&t_shadow);

                     if (intersection && (t_shadow < closest_t || closest_t == -1.0f)) {

                             closest_t = t_shadow;
                             closest_sphere = sphere;

                     }

             }

             if (closest_sphere) {

                     pixel_color = scalarMultiply(0.1f, pixel_color);

		     return pixel_color;

             }

	     else {

		     return pixel_color;

	     }
	    

}

void MS1_output(World *world_p, FILE *output_file) {

	Vec3 l_pos = lightsource.light_pos;

	sphere_color.x = 1.0f;
	sphere_color.y = 1.0f;
	sphere_color.z = 1.0f;

	Vec3 result1 = add(background_color, l_pos);

	fprintf(output_file,"(%.1f, %.1f, %.1f) + (%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n",background_color.x, background_color.y, background_color.z,l_pos.x,l_pos.y,l_pos.z, result1.x, result1.y, result1.z);


	Vec3 result2 = subtract(background_color, l_pos);

	fprintf(output_file,"(%.1f, %.1f, %.1f) - (%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n",background_color.x, background_color.y, background_color.z,l_pos.x,l_pos.y,l_pos.z, result2.x, result2.y, result2.z);

	Vec3 result3 = scalarMultiply(viewport.width, l_pos);

	fprintf(output_file, "%.1f * (%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n", viewport.width,l_pos.x,l_pos.y,l_pos.z, result3.x, result3.y, result3.z);

	Vec3 result4 = normalize(l_pos);

	fprintf(output_file, "normalize(%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n",l_pos.x,l_pos.y,l_pos.z, result4.x, result4.y, result4.z);

	for (int i = 0; i < world_p->size; i++) {

		fprintf(output_file,"\n");

		Sphere *sphere_p = world_p->spheres[i];

		Vec3 result5 = scalarDivide(sphere_color, sphere_p->r);

		float result6 = dot(l_pos, sphere_p->pos);

		float result7 = distance(l_pos, sphere_p->pos);

		float result8 = length(sphere_p->pos);

		fprintf(output_file, "(%.1f, %.1f, %.1f) / %.1f = (%.1f, %.1f, %.1f)\n",
            	sphere_color.x, sphere_color.y, sphere_color.z,
            	sphere_p->r, result5.x, result5.y, result5.z);

    		fprintf(output_file, "dot((%.1f, %.1f, %.1f), (%.1f, %.1f, %.1f)) = %.1f\n",
            	l_pos.x, l_pos.y, l_pos.z,
            	sphere_p->pos.x, sphere_p->pos.y, sphere_p->pos.z,
            	result6);

    		fprintf(output_file, "distance((%.1f, %.1f, %.1f), (%.1f, %.1f, %.1f)) = %.1f\n",
            	l_pos.x, l_pos.y, l_pos.z,
            	sphere_p->pos.x, sphere_p->pos.y, sphere_p->pos.z,
            	result7);

    		fprintf(output_file, "length(%.1f, %.1f, %.1f) = %.1f\n",
            	sphere_p->pos.x, sphere_p->pos.y, sphere_p->pos.z,
            	result8);

	}
}

/*
char *read_string() {

	char *str = NULL;
	int ch;
	int str_len = 0, str_size = 15 + 1;

	str = malloc(str_size * sizeof(char));

	if (str == NULL) {

		printf("read_string malloc failure\n");

		exit(1);

	}

	while ( (ch = getchar()) != '\n' && ch != ' ') {

		str[str_len++] = ch;

		if (str_len >= str_size) {

			str_size *= 2;

			char *temp = realloc(str, str_size * sizeof(char));

			if (str == NULL) {

				printf("read_string realloc failure\n");

				exit(1);

			}

			str = temp;

		}

	}

	str[str_len] = '\0';

	return str;

}
*/




