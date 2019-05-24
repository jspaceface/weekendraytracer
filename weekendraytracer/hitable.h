#ifndef HITABLEH
#define HITABLEH

#include "ray.h"
#include "unit_rand.h"

class material;

vec3 random_in_unit_sphere() {
	unit_rand ur;
	vec3 p;
	do {
		// generate a vector with: 0 <= x, y, and z < 1.0.
		p = 2.0*vec3(ur.gen(), ur.gen(), ur.gen()) - vec3(1, 1, 1);
		// try again if it is outside the unit sphere (check whether x^2 + y^2 + z^2 >= 1^2 = 1)
	} while (p.squared_length() >= 1);
	return p;
}

struct hit_record {
    double t;			// t at hitpoint
    vec3 p;				// hitpoint
    vec3 normal;		// normal vector from hitpoint
	material *mat_ptr;	// the material that was hit
};

class hitable {
    public:
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif