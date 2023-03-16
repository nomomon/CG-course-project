#include "triangle.h"
#include <iostream>
using namespace std;

Hit Triangle::intersect(Ray const &ray)
{

    double t = (v0 - ray.O).dot(N) / N.dot(ray.D);
    if (t < 0) // behind camera
    {
        return Hit::NO_HIT();
    }

    Point hit = ray.at(t);

    // check if hit is inside triangle
    // calculate area of triangle
    double area = 0.5 * (v1 - v0).cross(v2 - v0).length();

    // calculate area of subtriangles
    double area1 = 0.5 * (v1 - hit).cross(v2 - hit).length();
    double area2 = 0.5 * (v0 - hit).cross(v2 - hit).length();
    double area3 = 0.5 * (v0 - hit).cross(v1 - hit).length();

    // check if sum of subtriangles is equal to triangle
    if (1e-3 > (area1 + area2 + area3) - area)
    {
        // cout << "hit "
        //      << "[ "
        //      << v0 << "\n      "
        //      << v1 << "\n      "
        //      << v2 << " ]" << endl;

        return Hit(t, N);
    }
    // cout << "no hit" << endl;
    return Hit::NO_HIT();
}

Vector Triangle::toUV(Point const &hit)
{
    return Vector();
}

Triangle::Triangle(Point const &v0, Point const &v1, Point const &v2) : v0(v0),
                                                                        v1(v1),
                                                                        v2(v2),
                                                                        N((v1 - v0).cross(v2 - v0).normalized()) {}
