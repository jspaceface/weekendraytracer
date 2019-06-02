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
#include "material.h"
#include "lambertian.h"
#include "metal.h"
#include "dialectric.h"


hitable *random_spheres_scene() {
	int n = 500;
	hitable **list = new hitable*[n + 1];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
	int i = 1;
	unit_rand ur;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			double choose_mat = ur.gen();
			vec3 center(a + 0.9*ur.gen(), 0.2, b + 0.9*ur.gen());
			if ((center - vec3(4, 0.2, 0)).length() > 0.9) { // check if away from 3 center spheres. TODO try without this check
				if (choose_mat < 0.8) { // diffuse
					list[i++] = new sphere(center, 0.2, new lambertian(vec3(ur.gen()*ur.gen(), ur.gen()*ur.gen(), ur.gen()*ur.gen())));
				}
				else if (choose_mat < 0.95) { // metal
					list[i++] = new sphere(center, 0.2, new metal(vec3(0.5*(1 + ur.gen()), 0.5*(1 + ur.gen()), 0.5*(1 + ur.gen())), 0.5*ur.gen()));
				}
				else { //glass
					list[i++] = new sphere(center, 0.2, new dialectric(1.5));
				}
			}
		}
	}

	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dialectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));
	return new hitable_list(list, i);
}

vec3 color(const ray& r, hitable *world, int depth) {
	hit_record rec;
	// check for hits (0.00001 as t_min to avoid shadow acne, which is when the hit
	// is at t=0 and interceptor math gives small values just left or right of 0)
	if (world->hit(r, 0.00001, std::numeric_limits<double>::max(), rec)) {
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation*color(scattered, world, depth+1);
		}
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

	int nx = 1200;
	int ny = 800;
	int ns = 10;

	// open ppm file
	std::ofstream fs(filename);
	if (!fs) { std::cerr << "Cannot open the output file." << std::endl; return 1;}
	fs << "P3\n" << nx << " " << ny << "\n255\n";

	const int NUM_SPHERES = 5;
	hitable *list[NUM_SPHERES];
	
	// multiple spheres
	//list[0] = new sphere(vec3(0,0,-1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5))); // center sphere
	//list[1] = new sphere(vec3(0,-100.5,-1), 100, new lambertian(vec3(0.8, 0.8, 0.0))); // large sphere underneath
	//list[2] = new sphere(vec3(1,0,-1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.3));
	//list[3] = new sphere(vec3(-1,0,-1), 0.5, new dialectric(1.5));
	//list[4] = new sphere(vec3(-1,0,-1), -0.45, new dialectric(1.5));

	//hitable *world = new hitable_list(list, NUM_SPHERES);
	hitable *world = random_spheres_scene();
	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	float dist_to_focus = 10.0;
	float aperture = 0.1;

	camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, double(nx) / double(ny), aperture, dist_to_focus);
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

				col += color(r, world, 0);
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

