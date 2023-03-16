#ifndef SPHERE_H_
#define SPHERE_H_

#include "../object.h"
#include "../quaternion.h"

class Sphere : public Object
{
    double const PI = 3.14159265358979323846;

public:
    Sphere(Point const &pos, double radius, Vector const &axis = Vector(0.0, 1.0, 0.0), double angle = 0.0);

    Hit intersect(Ray const &ray) override;
    Vector toUV(Point const &hit) override;

    Point const position;
    double const r;
    Vector const axis;
    double const angle;
};

#endif
