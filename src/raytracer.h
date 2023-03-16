#ifndef RAYTRACER_H_
#define RAYTRACER_H_

#include <string>

#include "scene.h"

// Forward declarations
class Light;
class Material;

#include "json/json_fwd.h"

class Raytracer {
  Scene scene;

 public:
  bool readScene(std::string const &ifname);
  void renderToFile(std::string const &ofname);

 private:
  bool parseObjectNode(nlohmann::json const &node);

  Light parseLightNode(nlohmann::json const &node) const;
  Material parseMaterialNode(nlohmann::json const &node) const;
};

#endif
