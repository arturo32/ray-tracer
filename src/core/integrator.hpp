#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "scene.hpp"
#include "camera.hpp"

namespace rt3 {

class Integrator {
	public:
		virtual ~Integrator(){};
		virtual void render( Scene& scene ) = 0;
		std::shared_ptr<Camera> camera;
};

class SamplerIntegrator : public Integrator {
	//=== Public interface
	public:
		virtual ~SamplerIntegrator(){};
		virtual ColorXYZ Li( Ray& ray,  Scene& scene, Spectrum bkg_color ) = 0;
		virtual void render( Scene& scene );
		virtual void preprocess(  Scene& scene ) {};
};

class FlatIntegrator : public SamplerIntegrator {
	//=== Public interface
	public:
		~FlatIntegrator(){};
		FlatIntegrator(){};
		ColorXYZ Li( Ray& ray, Scene& scene, Spectrum bkg_color );
		virtual void preprocess(  Scene& scene ) override {};
};

class DepthIntegrator : public Integrator {
	//=== Public interface
	public:
		~DepthIntegrator(){};
		DepthIntegrator(real_type z_min, real_type z_max,
		 ColorXYZ near_color = ColorXYZ(0,0,0), ColorXYZ far_color = ColorXYZ(255,255,255))
		: z_min{z_min}, z_max{z_max}, near_color{near_color}, far_color{far_color} {};
		// ColorXYZ Li( Ray& ray, Scene& scene, Spectrum bkg_color );
		void render( Scene& scene );
		void preprocess(  Scene& scene );
		real_type z_min;
		real_type z_max;
		uint cz_min;
		uint cz_max;
		ColorXYZ near_color;
		ColorXYZ far_color;
		real_type** z_buffer;
};

class NormalMapIntegrator : public SamplerIntegrator {
	//=== Public interface
	public:
		~NormalMapIntegrator(){};
		NormalMapIntegrator(){};
		ColorXYZ Li( Ray& ray, Scene& scene, Spectrum bkg_color );
};

}

#endif