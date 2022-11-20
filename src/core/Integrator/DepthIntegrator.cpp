#include "integrator.hpp"

namespace rt3 {

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

} // namespace rt3