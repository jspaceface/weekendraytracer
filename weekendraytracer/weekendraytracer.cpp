#include <fstream>
#include <iostream>
#include "vec3.h"

int main()
{
	// open ppm file
	std::ofstream fs("generated-vec3-version1.ppm");
	if (!fs)
	{
		std::cerr << "Cannot open the output file." << std::endl;
		return 1;
	}
	
	int nx = 200;
	int ny = 100;

	fs << "P3\n" << nx << " " << ny << "\n255\n";

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

	fs.close();

	// todo remove for release
	std::cin.get();
	return 0;
}

