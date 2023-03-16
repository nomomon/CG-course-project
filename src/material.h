#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "image.h"
#include "triple.h"

class Material {
 public:
  Color color;  // base color
  double ka;    // ambient coefficient
  double kd;    // diffuse coefficient
  double ks;    // specular coefficient
  double n;     // exponent for specular highlight size

  bool hasTexture = false;
  Image texture;

  bool isTransparent = false;
  double nt = 1.0;

  Material() = default;

  Material(Color const &color, double ka, double kd, double ks, double n)
      : color(color), ka(ka), kd(kd), ks(ks), n(n), hasTexture(false), texture() {}

  Material(Image const &texture, double ka, double kd, double ks, double n)
      : color(), ka(ka), kd(kd), ks(ks), n(n), hasTexture(true), texture(texture) {}

  Material(Color const &color, double ka, double kd, double ks, double n, double nt)
      : color(color), ka(ka), kd(kd), ks(ks), n(n), isTransparent(true), nt(nt) {}
};

#endif
