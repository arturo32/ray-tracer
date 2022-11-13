#ifndef TRIANGLE_MESH_H
#define TRIANGLE_MESH_H

#include "../rt3-base.hpp"

namespace rt3 {

struct TriangleMesh {
    std::vector<Point3f> points;
    std::vector<Normal3f> normals;
    std::vector<Point2f> uvs;
};

}
#endif  