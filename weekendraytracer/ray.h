#ifndef RAYH
#define RAYH
#include "vec3.h"

class ray {

    public:
        ray() {}
        ray(const vec3& a, const vec3& b) { this->A = a; this->B = b; }
        vec3 origin() const { return A; }
        vec3 direction() const { return B; }
        vec3 pt(double t) const { return A + t*B; }

    private:
        vec3 A;
        vec3 B;
};

#endif