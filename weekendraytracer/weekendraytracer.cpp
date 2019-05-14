#include <fstream>
#include <iostream>
#include "ray.h"
#include "simplepmm.h"


// bool hit_sphere(const vec3& center, double radius, const ray& ray) {
// 	vec3 oc = ray.origin() - center;
// 	double a = dot(ray.direction(), ray.direction());
// 	double b = 2 * dot(ray.direction(), oc); 
// 	double c = dot(oc, oc) - radius * radius;
// 	double discriminant = b*b - 4*a*c;
// 	return discriminant > 0;
// }

double hit_sphere(const vec3& center, double radius, const ray& ray) {
	vec3 oc = ray.origin() - center;
	double a = dot(ray.direction(), ray.direction());
	double b = 2 * dot(ray.direction(), oc); 
	double c = dot(oc, oc) - radius * radius;
	double discriminant = b*b - 4*a*c;
	if (discriminant < 0) {
		return -1.0;
	}
	else {
		// smallest t = closest hit point (if there are two)
		return  (-b - sqrt(discriminant)) / (2*a);
	}
}

vec3 color(const ray& r) {
	double t = hit_sphere(vec3(0,0,-1), 0.5, r);
	if (t > 0.0) {
		vec3 N = unit_vector(r.pt(t) - vec3(0,0,-1));
		return 0.5*(N + 1.0);
	}
	
	// dir vector is now between -1 and 1
	vec3 unit_direction = unit_vector(r.direction());
	// shift y so that: 0.0 < t < 1.0
	t = 0.5*(unit_direction.y() + 1.0);
	// (1-t)*white + (t)*blue <-- this is linear blending, or a "lerp"
	return (1.0-t)*vec3(1.0,1.0,1.0) + t*vec3(0.5,0.7,1.0);
}

int main()
{

	int nx = 200;
	int ny = 100;

	// open ppm file
	std::ofstream fs("blended.ppm");
	if (!fs) { std::cerr << "Cannot open the output file." << std::endl; return 1;}
	fs << "P3\n" << nx << " " << ny << "\n255\n";

	// simple pmm file
	// generate_simple_pmm(fs, nx, ny);


	vec3 lower_left_corner(-2.0, -1.0, -1.0);
	vec3 horizontal(4.0, 0.0, 0.0);
	vec3 vertical(0.0, 2.0, 0.0);
	vec3 origin(0.0, 0.0, 0.0);

	for (int j = ny-1; j >=0; j--) {
		for (int i = 0; i < nx; i++) {

			// u and v go roughly from 0 to 1, representing how far along each axis we are.
			double u = double(i) / double(nx);
			double v = double(j) / double(ny);

			//TODO: try this with and without the -origin
			ray r(origin, lower_left_corner + u*horizontal + v*vertical);
			// ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);

			vec3 col = color(r);

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
	std::cin.get();
	return 0;
}

