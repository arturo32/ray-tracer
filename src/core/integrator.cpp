#include "integrator.hpp"
#include <thread>
#include <omp.h>

namespace rt3 {

void SamplerIntegrator::render( Scene& scene) {
    // Always call the preprocess() before doing any rendering.
    // This might be just an empty method, or not, depending on the integrator's needs.
    preprocess(scene);
    
    // This might just be a tile (part) of an image, rendered in parallel.
    // Retrieve the image dimensions in pixels.
    int w = camera->film->get_resolution().x(); 
    int h = camera->film->get_resolution().y();

    // TODO: usar o crop_window do film

    // rxmin = clamp (ceil ( xresolution*xmin ), 0, xresolution-1);
    // rxmax = clamp (ceil ( xresolution*xmax -1 ), 0, xresolution-1);
    // rymin = clamp (ceil ( yresolution*ymin ),  0, yresolution-1);
    // rymax = clamp (ceil ( yresolution*ymax -1 ), 0, yresolution-1);

    const uint processor_count = std::thread::hardware_concurrency();
    std::cout << "procs: " << processor_count << std::endl;
    #pragma omp parallel num_threads(processor_count) default(none) shared(h, w, scene, camera)
    {
        #pragma omp for schedule(guided)
        for( int j = 0 ; j < h ; ++j ) {
            for( int i = 0 ; i < w ; ++i ) {
                Point2f pixel = Point2f{real_type(i) / real_type(w), real_type(j) / real_type(h)};
                
                // Get the background color in case we hit nothing.
                Spectrum background = scene.background->sampleXYZ( pixel );
                
                // Determine the ray for the current camera type.
                Ray ray = camera->generate_ray( Point2f{real_type(i), real_type(j)} );
                
                // Determine the incoming light.
                Spectrum L = Li( ray, scene, background, 1);
                L *= 255.0;
                L.clamp(0.0, 255.0);
                
                // Add color (radiance) to the image.
                camera->film->add_sample( Point2i( i, h-(j+1) ), L ); // Set color of pixel (x,y) to L.
            }
        }
    }
    // Send image color buffer to the output file.
    camera->film->write_image();
}

// This method must be overridden by all classes derived from SamplerIntegrator.
/// Determines a color for the incoming ray.
ColorXYZ FlatIntegrator::Li( Ray& ray,  Scene& scene, Spectrum bkg_color, uint depth) 
{
    ColorXYZ L(0,0,0); // The radiance
    // Find closest ray intersection or return background radiance.
    Surfel isect; // Intersection information.  
    scene.intersect(ray, &isect);
    if (!isect.hit) {
        // This might be just:
        L = bkg_color;
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

void DepthIntegrator::preprocess(  Scene& scene ) {
    int w = camera->film->get_resolution().x();
    int h = camera->film->get_resolution().y();

    z_buffer = (real_type **)malloc(h * sizeof(real_type*));
    for(int i = 0; i < h; ++i){
        z_buffer[i] = (real_type *) malloc(w * sizeof(real_type));
    }

    cz_min = -1;
    cz_max = 0;

    // TODO: usar o crop_window do film

    for( int j = 0 ; j < h ; ++j ) {
        for( int i = 0 ; i < w ; ++i ) {
            z_buffer[j][i] = INFINITY;
            Ray ray = camera->generate_ray( Point2f{real_type(i), real_type(j)} );
            Surfel isect = Surfel();
            scene.intersect(ray, &isect);
            if (isect.hit) {
                if(ray.t_max < cz_min) {
                    cz_min = ray.t_max; 
                }
                if (ray.t_max > cz_max) {
                    cz_max = ray.t_max;
                }
                z_buffer[j][i] = ray.t_max;
            }
        }
    }
}

void DepthIntegrator::render( Scene& scene) {
    // Always call the preprocess() before doing any rendering.
    // This might be just an empty method, or not, depending on the integrator's needs.
    preprocess(scene);
    
    // This might just be a tile (part) of an image, rendered in parallel.
    // Retrieve the image dimensions in pixels.
    int w = camera->film->get_resolution().x(); 
    int h = camera->film->get_resolution().y();

    // TODO: usar o crop_window do film

    for( int j = 0 ; j < h ; ++j ) {
        for( int i = 0 ; i < w ; ++i ) {
            real_type z = z_buffer[j][i];
            ColorXYZ L(0,0,0);
            if (z == INFINITY) {
                Point2f pixel = Point2f{real_type(i) / real_type(w), real_type(j) / real_type(h)};
                L = scene.background->sampleXYZ( pixel );
            } else {
                real_type t = (z - cz_min) /(cz_max - cz_min);
                // real_type t = z_min + (z_max - z_min)*t;
                t = Lerp(t, z_min, z_max);
                L = scene.background->lerp(near_color, far_color, t);
            }
            // Add color (radiance) to the image.
            camera->film->add_sample( Point2i( i, h-(j+1) ), L ); // Set color of pixel (x,y) to L.
        }
    }
    // Send image color buffer to the output file.
    camera->film->write_image();
    
    for(int i = 0; i < h; i++){
        free(z_buffer[i]);
    }
    free(z_buffer);
}

ColorXYZ NormalMapIntegrator::Li( Ray& ray, Scene& scene, Spectrum bkg_color, uint depth) 
{
    ColorXYZ L(0,0,0);
    Surfel isect = Surfel();
    scene.intersect(ray, &isect);
    if (!isect.hit) {
        L = bkg_color;
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

ColorXYZ BlinnPhongIntegrator::Li( Ray& ray, Scene& scene, Spectrum bkg_color, uint depth) 
{
    ColorXYZ L(0,0,0);
    Surfel isect = Surfel();
    scene.intersect(ray, &isect);
    if (!isect.hit) {
        return bkg_color;
    } else {
        BlinnPhongMaterial *fm = dynamic_cast<BlinnPhongMaterial*>(isect.primitive->get_material().get());
        Vector3f wi;
        VisibilityTester vis;
        for (auto &&l : scene.lights) {
            l->sample_Li(isect, &wi, &vis);
            // Ray from intersect point to the light source
            Ray sray = Ray(isect.p, wi);
            Surfel sisect = Surfel();
		    scene.intersect(sray, &sisect);
            if(!sisect.hit) {
                Vector3f vecFromPointToCamera = -ray.direction;
                vecFromPointToCamera.make_unit_vector();
                Vector3f h = (vecFromPointToCamera + wi) / (vecFromPointToCamera + wi).length();  
                L += fm->diffuse * l->intensity * std::max(real_type(0.0), dot(isect.n, wi));
                L += fm->specular * l->intensity * pow( std::max(real_type(0.0), dot(isect.n, h)), fm->glossiness );
            }
        }
        if(scene.ambientLight != nullptr) {
            L += fm->ambient * scene.ambientLight->intensity;
        }
        if (depth < max_depth) {
            Ray reflected_ray = Ray(isect.p, ray.direction - 2*(dot(ray.direction,isect.n))*isect.n);
            L += + fm->mirror * Li(reflected_ray, scene, bkg_color, depth+1);
        }
    }
    return L;
}

}