#include "color.h"


void writeColour(FILE *ppmFile, Vec3 color) {	

	fprintf(ppmFile, "%d %d %d ", (int) color.x , (int) color.y , (int) color.z);

}

Vec3 unpackRGB(unsigned int packedRGB) {

	unsigned int red = (packedRGB & 0xFF0000) >> 16;

	unsigned int green = (packedRGB & 0x00FF00) >> 8;

	unsigned int blue = (packedRGB & 0x0000FF);

	/*

	float redf = red / 255.0f;

	float greenf = green/255.0f;

	float bluef = blue/255.0f;

	*/

	Vec3 sphere_color = {red, green, blue};

	return sphere_color;
}

int compareColor(const void *a, const void *b){

    int a1 = 0, b1 = 0;

    for (int i = 0; i < sizeof(int); i++) {

        a1 |= (*((unsigned char*)a + i) & 0x0F) << (i * 8);
        b1 |= (*((unsigned char*)b + i) & 0x0F) << (i * 8);

    }

    return (a1 < b1) ? -1 : (b1 < a1) ? 1 : (*((int*)a) < *((int*)b)) ? -1 : (*((int*)a) > *((int*)b)) ? 1 : 0;
}

