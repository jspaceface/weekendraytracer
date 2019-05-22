#ifndef HITABLEH
#define HITABLEH

#include "ray.h"

struct hit_record {
    double t;       // t at hitpoint
    vec3 p;         // hitpoint
    vec3 normal;    // normal vector from hitpoint
};

class hitable {
    public:
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif