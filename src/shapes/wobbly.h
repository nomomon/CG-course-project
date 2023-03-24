#ifndef WOBBLY_H_
#define WOBBLY_H_

#include "../object.h"
#include "../quaternion.h"

class Wobbly : public Object
{

public:
    Wobbly(Point const &pos, double radius, Vector const &axis = Vector(0.0, 1.0, 0.0), double angle = 0.0);

    Hit intersect(Ray const &ray) override;

    Point const position;
    double const r;
    Vector const axis;
    double const angle;
};

#endif
