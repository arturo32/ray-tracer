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
				// Determine the ray for the current camera type.
				Ray ray = camera->generate_ray( Point2f{real_type(i), real_type(j)} );
				
				// Determine the incoming light.
				Point2f pixel = Point2f{real_type(i) / real_type(w), real_type(j) / real_type(h)};
				Spectrum L = Li( ray, scene, pixel, 1);
				
				if (camera->film->gamma_corretion) {
					L[0] = pow(L[0], camera->film->igamma);
					L[1] = pow(L[1], camera->film->igamma);
					L[2] = pow(L[2], camera->film->igamma);
				}
				
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

} // rt3 namespace