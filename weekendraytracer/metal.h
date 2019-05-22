#ifndef METALH
#define METALH

#include "material.h"

class metal : public material {
public:
	metal(vec3& a) : albedo(a) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = ray(rec.p, reflected);
		attenuation = albedo;
		return dot(scattered.direction(), rec.normal) > 0;
	}

private:
	vec3 albedo;

	vec3 reflect(const vec3& v, const vec3& n) {
		return v - 2*dot(v,n)*n;
	}
};

#endif // !METALH
