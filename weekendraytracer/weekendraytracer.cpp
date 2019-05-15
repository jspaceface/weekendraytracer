#include <string>
#include <fstream>
#include <iostream>
#include "ray.h"
#include "simplepmm.h"
#include "hitable_list.h"
#include "sphere.h"
#include "float.h"
#include "constants.h"

vec3 color(const ray& r, hitable *world) {
	hit_record rec;
	if (world->hit(r, 0.0, MAXFLOAT, rec)) {
		return 0.5*(rec.normal + 1.0);
	}
	vec3 unit_direction = unit_vector(r.direction()); // dir vector is now between -1 and 1
	double t = 0.5*(unit_direction.y() + 1.0); // shift y so that: 0.0 < t < 1.0
	return (1.0-t)*WHITE_COLOR_VEC3 + t*BLUE_COLOR_VEC3; // this is linear blending, or a "lerp"
}

int main()
{
	std::cout << "Starting weekendraytracer.cpp..." << std::endl;

	std::string filename;
	std::cout << "What PPM file do you want to write to?" << std::endl;
	std::cin >> filename;
	filename += ".ppm";
	std::cout << "Okay. Going to create " << filename << std::endl;

	int nx = 200;
	int ny = 100;

	// open ppm file
	std::ofstream fs(filename);
	if (!fs) { std::cerr << "Cannot open the output file." << std::endl; return 1;}
	fs << "P3\n" << nx << " " << ny << "\n255\n";
	
	vec3 lower_left_corner(-2.0, -1.0, -1.0);
	vec3 horizontal(4.0, 0.0, 0.0);
	vec3 vertical(0.0, 2.0, 0.0);
	vec3 origin(0.0, 0.0, 0.0);

	int num_spheres = 4;
	hitable *list[num_spheres];
	list[0] = new sphere(vec3(0,0,-1), 0.5); // center sphere
	list[1] = new sphere(vec3(0,-100.5,-1), 100); // large sphere underneath
	list[2] = new sphere(vec3(0.2,0,-0.6), 0.1); // slightly smaller sphere center right, in front
	list[3] = new sphere(vec3(0,0,-1), 0.2); // smaller sphere inside center sphere, should be invisible
	hitable *world = new hitable_list(list, num_spheres);

	std::cout << "Beginning ray tracing..." << std::endl;
	for (int j = ny-1; j >=0; j--) {
		for (int i = 0; i < nx; i++) {

			// u and v go roughly from 0 to 1, representing how far along each axis we are.
			double u = double(i) / double(nx);
			double v = double(j) / double(ny);

			//TODO: try this with and without the -origin
			ray r(origin, lower_left_corner + u*horizontal + v*vertical);
			// ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);

			vec3 col = color(r, world);

			//todo move this to a .h file
			int ir = int(255.99 * col.r());
			int ig = int(255.99 * col.g());
			int ib = int(255.99 * col.b());

			fs << ir << " " << ig << " " << ib << "\n";
		}
	}

	// close pmm file
	fs.close();


	// todo remove for release
	std::cout << "Raytracing complete. Waiting for interrupt to close." << std::endl;
	std::cin.get();
	return 0;
}

