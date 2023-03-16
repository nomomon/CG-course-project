#ifndef QUAD_H_
#define QUAD_H_

#include "../object.h"

class Quad : public Object {
 public:
  Quad(Point const &v0, Point const &v1, Point const &v2, Point const &v3);

  Hit intersect(Ray const &ray) override;
  Vector toUV(Point const &hit) override;

  Point const v0;
  Point const v1;
  Point const v2;
  Point const v3;

  Vector const N;
};

#endif
