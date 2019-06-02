#ifndef MATERIALH
#define MATERIALH

#include "hitable.h"

class material {
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;

protected:
	vec3 reflect(const vec3& v, const vec3& n) const {
		return v - 2*dot(v,n)*n;
	}

	bool refract(const vec3& v, const vec3& n, double ni_over_nt, vec3& refracted) const {
		vec3 uv = unit_vector(v);
		double dt = dot(uv,n);
		double discriminant = 1.0 - ni_over_nt*ni_over_nt*(1-dt*dt);
		if (discriminant > 0) {
			refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
			return true;
		}
		return false;
	}
};

#endif