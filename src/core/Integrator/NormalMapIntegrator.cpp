#include "integrator.hpp"

namespace rt3 {


ColorXYZ NormalMapIntegrator::Li( Ray& ray, Scene& scene, Point2f pixel, uint depth) 
{
    ColorXYZ L(0,0,0);
    Surfel isect = Surfel();
    scene.intersect(ray, &isect);
    if (!isect.hit) {
        L = scene.background->sampleXYZ(pixel);
    } else {
        Point3f normal = isect.n;
        normal.make_unit_vector();
        real_type r = Lerp((normal.r()+1)/2.0, 0.0, 255.0);
        real_type g = Lerp((normal.g()+1)/2.0, 0.0, 255.0);
        real_type b = Lerp(normal.b(), 128.0, 255.0);
        L = ColorXYZ(r, g, b);
        // std::cout << normal << std::endl;
    }
    return L;
}



} // rt3 namespace