#include "mesh.h"

#include <cmath>
#include <iostream>
#include <limits>

#include "../objloader.h"
#include "../vertex.h"
#include "triangle.h"

using namespace std;

Hit Mesh::intersect(Ray const &ray)
{
    // Replace the return of a NO_HIT by determining the intersection based
    // on the ray and this class's data members.
    Hit min_hit(numeric_limits<double>::infinity(), Vector());
    ObjectPtr obj = nullptr;
    for (unsigned idx = 0; idx != d_tris.size(); ++idx)
    {
        Hit hit(d_tris[idx]->intersect(ray));
        if (hit.t < min_hit.t)
        {
            min_hit = hit;
            obj = d_tris[idx];
        }
    }

    // No hit?
    if (!obj)
        return Hit::NO_HIT();

    return min_hit;
}

Vector Mesh::toUV(Point const &hit)
{
    return Vector();
}

Mesh::Mesh(string const &filename, Point const &position, Vector const &rotation, Vector const &scale)
{
    OBJLoader model(filename);
    d_tris.reserve(model.numTriangles());
    vector<Vertex> vertices = model.vertex_data();
    for (size_t tri = 0; tri != model.numTriangles(); ++tri)
    {
        Vertex one = vertices[tri * 3];
        Point v0(one.x, one.y, one.z);

        Vertex two = vertices[tri * 3 + 1];
        Point v1(two.x, two.y, two.z);

        Vertex three = vertices[tri * 3 + 2];
        Point v2(three.x, three.y, three.z);

        // Apply non-uniform scaling, rotation and translation to the three points
        // of the triangle (v0, v1, and v2) here.

        // Non-uniform scaling
        v0 = v0.scale(scale);
        v1 = v1.scale(scale);
        v2 = v2.scale(scale);

        // Rotation
        v0 = v0.rotate(rotation);
        v1 = v1.rotate(rotation);
        v2 = v2.rotate(rotation);

        // Translation
        v0 = v0.translate(position);
        v1 = v1.translate(position);
        v2 = v2.translate(position);

        d_tris.push_back(ObjectPtr(new Triangle(v0, v1, v2)));
    }

    cout << "Loaded model: " << filename << " with " << model.numTriangles() << " triangles.\n";
}