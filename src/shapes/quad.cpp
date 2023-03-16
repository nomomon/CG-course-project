#include "quad.h"

#include <cmath>
#include <limits>

/*  Method:
 *  First find the intersection with the plane the quad is in,
 *  then determine whether the point of intersection is within the quad.
 */
Hit Quad::intersect(Ray const &ray) {
  // Catch the case where the ray is parallel to the plane, i.e. no intersection.
  double DdotN = (-ray.D).dot(N);
  if (std::abs(DdotN) < std::numeric_limits<double>::epsilon()) return Hit::NO_HIT();

  // Find the point of intersection with the plane.
  double t = -N.dot(ray.O - v0) / N.dot(ray.D);

  if (t < 0.0) return Hit::NO_HIT();

  Point hit = ray.at(t);

  // Determine if the hit is inside of the quad.
  double u = (hit - v0).dot(v1 - v0);
  double v = (hit - v0).dot(v3 - v0);
  if (0.0 <= u and u <= (v1 - v0).length_2() and 0.0 <= v and v <= (v3 - v0).length_2()) return Hit(t, N);

  return Hit::NO_HIT();
}

Vector Quad::toUV(Point const &hit) {
  double u = (hit - v0).dot(v1 - v0) / (v1 - v0).length_2();
  double v = (hit - v0).dot(v3 - v0) / (v3 - v0).length_2();

  return Vector(u, v, 0.0);
}

Quad::Quad(Point const &v0, Point const &v1, Point const &v2, Point const &v3)
    : v0(v0), v1(v1), v2(v2), v3(v3), N((v1 - v0).cross(v3 - v0).normalized()) {}
