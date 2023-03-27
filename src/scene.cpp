#include "scene.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <iostream>

#include "hit.h"
#include "image.h"
#include "material.h"
#include "ray.h"

#define PI 3.14159265358979323846

using namespace std;

pair<ObjectPtr, Hit> Scene::castRay(Ray const &ray) const
{
    // Find hit object and distance
    Hit min_hit(numeric_limits<double>::infinity(), Vector());
    ObjectPtr obj = nullptr;
    for (unsigned idx = 0; idx != objects.size(); ++idx)
    {
        Hit hit(objects[idx]->intersect(ray));
        if (hit.t < min_hit.t)
        {
            min_hit = hit;
            obj = objects[idx];
        }
    }

    return pair<ObjectPtr, Hit>(obj, min_hit);
}

Color Scene::trace(Ray const &ray, unsigned depth)
{
    pair<ObjectPtr, Hit> mainhit = castRay(ray);
    ObjectPtr obj = mainhit.first;
    Hit min_hit = mainhit.second;

    // No hit? Return background color.
    if (!obj)
        return Color(0.0, 0.0, 0.0);

    Material const &material = obj->material;
    Point hit = ray.at(min_hit.t);
    Vector V = -ray.D;

    // Pre-condition: For closed objects, N points outwards.
    Vector N = min_hit.N;

    // The shading normal always points in the direction of the view,
    // as required by the Phong illumination model.
    Vector shadingN;
    if (N.dot(V) >= 0.0)
        shadingN = N;
    else
        shadingN = -N;

    Color matColor = material.color;

    // If the object has a texture, use the texture color instead.
    if (material.hasTexture)
    {
        Vector uv = obj->toUV(hit);
        matColor = material.texture.colorAt(uv.x, 1 - uv.y);
    }

    // Add ambient once, regardless of the number of lights.
    Color color = material.ka * matColor;

    // Add diffuse and specular components.
    for (auto const &light : lights)
    {
        Vector L = (light->position - hit).normalized();

        // check for shadows, shoot a ray to the light
        Ray ray(hit + epsilon * L, L);
        // if we're drawing shadows we only illuminate the pixel
        // (with diffuse and specular) if the pixel is not in an obj's shadow
        if (!renderShadows || !isInShadow(ray, light))
        {
            // Add diffuse.
            double dotNormal = shadingN.dot(L);
            double diffuse = std::max(dotNormal, 0.0);
            color += diffuse * material.kd * light->color * matColor;

            // Add specular.
            if (dotNormal > 0)
            {
                Vector reflectDir = reflect(-L, shadingN);
                double specAngle = std::max(reflectDir.dot(V), 0.0);
                double specular = std::pow(specAngle, material.n);

                color += specular * material.ks * light->color;
            }
        }
    }

    if (depth > 0 and material.isTransparent)
    {
        // The object is transparent, and thus refracts and reflects light.
        // Use Schlick's approximation to determine the ratio between the two.
        double ni = 1.0;
        double mu = ni / material.nt;

        // If the ray is inside the object,
        // swap the indices of refraction.
        if (N.dot(V) < 0)
        {
            mu = material.nt / ni;
        }

        Vector d = (-V).normalized();          // ray direction
        Vector n = shadingN.normalized();      // normal direction
        Vector r = reflect(d, n).normalized(); // reflection direction

        Vector t = mu * (d - d.dot(n) * n);
        double kr = 1;
        double kt = 0;

        // square of sin of the angle between the
        // transmited ray and the normal (sin^2 phi_t)
        double Q = 1 - mu * mu * (1 - n.dot(d) * n.dot(d));

        // not total internal reflection
        if (Q >= 0)
        {
            t -= n * sqrt(Q);

            // Schlick's approximation
            double R0 = pow((ni - material.nt) / (ni + material.nt), 2);
            double cosPhiI = -d.dot(n);

            kr = R0 + (1 - R0) * pow(1 - cosPhiI, 5);
            kt = 1 - kr;
        }

        // fix resulting ray for acne
        Ray reflection(hit + epsilon * r, r);
        Ray transmition(hit + epsilon * t, t);

        // recursively compute the color according to
        // transmission and reflection of the medium
        color += kr * trace(reflection, depth - 1);
        color += kt * trace(transmition, depth - 1);
    }
    else if (depth > 0 and material.ks > 0.0)
    {
        // The object is not transparent, but opaque.
        Vector reflectionDir = reflect(-V, shadingN);
        Ray ray(hit + epsilon * reflectionDir, reflectionDir);
        color += material.ks * trace(ray, depth - 1);
    }

    return color;
}

bool Scene::isInShadow(Ray const &ray, LightPtr const light)
{
    pair<ObjectPtr, Hit> shadowhit = castRay(ray);
    ObjectPtr shadow = shadowhit.first;
    Hit shadow_hit = shadowhit.second;
    // No hit? The pixel isn't in any object's shadow
    if (!shadow)
        return false;
    // Check if object is behind the light
    if (shadow_hit.t > (light->position - ray.O).length())
        return false;

    return true;
}

void Scene::render(Image &img)
{
    unsigned w = img.width();
    unsigned h = img.height();

    double eyeDistance = w / (2 * tan((FOV * M_PI / 180.) / 2));

    Point eyeRel(w / 2, h / 2, eyeDistance);

    for (unsigned y = 0; y < h; ++y)
        for (unsigned x = 0; x < w; ++x)
        {

            // placeholder for the final color
            Color col(0.0, 0.0, 0.0);

            // supersampling loop
            for (double i = 1; i <= supersamplingFactor; i++)
            {
                for (double j = 1; j <= supersamplingFactor; j++)
                {
                    Point pixel(
                        x + i / (supersamplingFactor + 1),
                        h - 1 - y + j / (supersamplingFactor + 1),
                        0);

                    Ray ray(eyeRel, (pixel - eyeRel).normalized());

                    ray.O = ray.O + eye;

                    ray.D = ray.D.rotateAroundAxis(eyeRotation.x, Triple(1, 0, 0));
                    ray.D = ray.D.rotateAroundAxis(eyeRotation.y, Triple(0, 1, 0));
                    ray.D = ray.D.rotateAroundAxis(eyeRotation.z, Triple(0, 0, 1));

                    Color subcol = trace(ray, recursionDepth);

                    subcol.clamp();
                    col += subcol;
                }
            }
            // average the color
            col /= (supersamplingFactor) * (supersamplingFactor);
            img(x, y) = col;
        }
}

// --- Misc functions ----------------------------------------------------------

// Defaults
Scene::Scene() : width(200), height(200), objects(), lights(), eye(), eyeRotation(Triple(0, 0, 0)), FOV(200.), renderShadows(false), recursionDepth(0), supersamplingFactor(1) {}

void Scene::addObject(ObjectPtr obj) { objects.push_back(obj); }

void Scene::addLight(Light const &light) { lights.push_back(LightPtr(new Light(light))); }

void Scene::setEye(Triple const &position) { eye = position; }

unsigned Scene::getNumObject() { return objects.size(); }

unsigned Scene::getNumLights() { return lights.size(); }

void Scene::setRenderShadows(bool shadows) { renderShadows = shadows; }

void Scene::setRecursionDepth(unsigned depth) { recursionDepth = depth; }

void Scene::setSuperSample(unsigned factor) { supersamplingFactor = factor; }

void Scene::setWidth(unsigned width) { this->width = width; }

void Scene::setHeight(unsigned height) { this->height = height; }

unsigned Scene::getWidth() { return width; }

unsigned Scene::getHeight() { return height; }

void Scene::setEyeRotation(Triple const &rotation) { this->eyeRotation = rotation; }

void Scene::setFOV(double fov) { this->FOV = fov; }