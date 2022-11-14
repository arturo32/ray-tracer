#include "integrator.hpp"

namespace rt3 {



ColorXYZ FlatIntegrator::Li( Ray& ray,  Scene& scene, Point2f pixel, uint depth) 
{
    ColorXYZ L(0,0,0); // The radiance
    // Find closest ray intersection or return background radiance.
    Surfel isect; // Intersection information.  
    scene.intersect(ray, &isect);
    if (!isect.hit) {
        // This might be just:
        L = scene.background->sampleXYZ(pixel);
    }
    else {
        // Some form of determining the incoming radiance at the ray's origin.
        // For this integrator, it might just be:
        // Polymorphism in action.
        FlatMaterial *fm = dynamic_cast< FlatMaterial *>( isect.primitive->get_material().get() );
        // // Assign diffuse color to L.
        L = fm->kd(); // Call a method present only in FlatMaterial.
    }
    return L;
}


} // rt3 namespace