#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "../object.h"

class Triangle : public Object
{
public:
    Triangle(Point const &v0, Point const &v1, Point const &v2);

    Hit intersect(Ray const &ray);
    Vector toUV(Point const &hit) override;

    Point v0;
    Point v1;
    Point v2;
    Vector N;
};

#endif
