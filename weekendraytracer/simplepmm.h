#ifndef SIMPLEPMMH
#define SIMPLEPMMH

#include <iostream>
#include "vec3.h"

/*
 * Writes a simple pmm file to the provided ostream
 */
void generate_simple_pmm(std::ostream& fs, int nx, int ny) {
    
    // Generate RGB values and write a line for each
	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			vec3 col(float(i) / float(nx), float(j) / float(ny), 0.2);
			int ir = int(255.99 * col.r());
			int ig = int(255.99 * col.g());
			int ib = int(255.99 * col.b());

			fs << ir << " " << ig << " " << ib << "\n";
		}
	}
}

#endif