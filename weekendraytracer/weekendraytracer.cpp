#include <string>
#include <fstream>
#include <iostream>
#include <random>
#include "ray.h"
#include "simplepmm.h"
#include "hitable_list.h"
#include "sphere.h"
#include "float.h"
#include "constants.h"
#include "camera.h"
#include "unit_rand.h"


vec3 color(const ray& r, hitable *world) {

	//todo resume page 27


	hit_record rec;
	// check for hits (0.00001 as t_min to avoid shadow acne, which is when the hit
	// is at t=0 and interceptor math gives small values just left or right of 0)
	if (world->hit(r, 0.00001, std::numeric_limits<double>::max(), rec)) {
		// generate a reflected ray and follow it
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		return 0.5*color(ray(rec.p, target-rec.p), world);

		// map the normal vector to a color, and return that.
		// return 0.5*(rec.normal + 1.0);
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
	int ns = 100;

	// open ppm file
	std::ofstream fs(filename);
	if (!fs) { std::cerr << "Cannot open the output file." << std::endl; return 1;}
	fs << "P3\n" << nx << " " << ny << "\n255\n";

	const int NUM_SPHERES = 2;
	hitable *list[NUM_SPHERES];
	list[0] = new sphere(vec3(0,0,-1), 0.5); // center sphere
	list[1] = new sphere(vec3(0,-100.5,-1), 100); // large sphere underneath
	// list[2] = new sphere(vec3(0.2,0,-0.6), 0.1); // slightly smaller sphere center right, in front
	// list[3] = new sphere(vec3(0,0,-1), 0.2); // smaller sphere inside center sphere, should be invisible
	hitable *world = new hitable_list(list, NUM_SPHERES);
	camera cam;
	unit_rand ur;
	std::cout << "Beginning ray tracing..." << std::endl;
	for (int j = ny-1; j >=0; j--) {
		for (int i = 0; i < nx; i++) {
			vec3 col(0,0,0);

			// Do ns samples of pixel (i,j)
			for (int s = 0; s < ns; s++) {
				double irand = ur.gen();
				double jrand = ur.gen();

				// u and v go roughly from 0 to 1, representing how far along each axis we are.
				double u = double(i + irand) / double(nx);
				double v = double(j + jrand) / double(ny);

				ray r = cam.get_ray(u, v);

				col += color(r, world);
			}
			col /= double(ns);
			// fix gamma correction in image viewers
			col = vec3(sqrt(col.r()), sqrt(col.g()), sqrt(col.b()));
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

