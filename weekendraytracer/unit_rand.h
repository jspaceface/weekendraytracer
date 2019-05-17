#ifndef UNITRANDH
#define UNITRANDH

#include <random>

class unit_rand {
public:
    unit_rand() : rng(rd()), dis(std::uniform_int_distribution<double>(0.0, 1.0)){}

    double gen() { return dis(rng); }

private:
    std::random_device rd;
    std::mt19937 rng;
    std::uniform_int_distribution<double> dis;

};
#endif