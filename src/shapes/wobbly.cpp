#include "wobbly.h"

#include <cmath>
#include <iostream>

#include "solvers.h"

using namespace std;

Hit Wobbly::intersect(Ray const &ray)
{
    // Wobbly formula: z = xy
    // Line formula:   x = ray.O + t * ray.D
    double x0 = ray.O.x - position.x;
    double y0 = ray.O.y - position.y;
    double z0 = ray.O.z - position.z;
    double x1 = ray.D.x;
    double y1 = ray.D.y;
    double z1 = ray.D.z;

    double a = x1 * y1;
    double b = x0 * y1 + x1 * y0 - z1;
    double c = x0 * y0 - z0;

    double t0;
    double t1;
    if (not Solvers::quadratic(a, b, c, t0, t1))
        return Hit::NO_HIT();

    // t0 is closest hit
    if (t0 < 0.0) // check if it is not behind the camera
    {
        t0 = t1;      // try t1
        if (t0 < 0.0) // both behind the camera
            return Hit::NO_HIT();
    }

    // check if the hit point is inside the sphere of wobbly
    if ((position - ray.at(t0)).length() > r)
    {
        return Hit::NO_HIT();
    }

    // calculate normal
    Point hit = ray.at(t0);
    Vector N = (hit - position).normalized();

    // Note that the direction of the normal is not changed here,
    // but in scene.cpp - if necessary.

    return Hit(t0, N);
}

Wobbly::Wobbly(Point const &pos, double radius, Vector const &axis, double angle)
    : // Feel free to modify this constructor.
      position(pos),
      r(radius),
      axis(axis),
      angle(angle)
{
}
