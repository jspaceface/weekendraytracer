#ifndef SPHEREH
#define SPHEREH

#include "hitable.h"
#include "float.h"

class sphere: public hitable {
    public:
        sphere() {}
        sphere(vec3 cen, double r, material *m) : center(cen), radius(r), mat_ptr(m) {};
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const;

    private: 
        vec3 center;
        double radius;
		material *mat_ptr;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
	double a = dot(r.direction(), r.direction());
	double b = dot(oc, r.direction()); 
	double c = dot(oc, oc) - radius*radius;
	double discriminant = b*b - a*c;
	if (discriminant > 0) {
        // check all roots of quadratic (+/-)
        double temp = (-b - sqrt(discriminant))/a;
        if (temp > t_min && temp < t_max) {
            rec.t = temp;
            rec.p = r.pt(temp);
            rec.normal = (rec.p - center)/radius;
			rec.mat_ptr = mat_ptr;
            return true;
        }
        temp = (-b + sqrt(discriminant))/a;
        if (temp > t_min && temp < t_max) {
            rec.t = temp;
            rec.p = r.pt(temp);
            rec.normal = (rec.p - center)/radius;
			rec.mat_ptr = mat_ptr;
			return true;
        }
	}

    return false;
}

#endif