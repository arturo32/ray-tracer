#include "light.hpp"

namespace rt3 {
	VisibilityTester::VisibilityTester( const Surfel& a, const Surfel& b):p0{a}, p1{b} {
		// EMPTY
	}

	bool VisibilityTester::unoccluded( const Scene& scene ) {
		// TODO
	}

	Light::Light(light_flag_e t, Vector3f intensity):
	 flags{t}, intensity{intensity} {
		//EMPTY
	}

	Light::~Light() = default;

	PointLight::PointLight(Point3f from, Vector3f intensity):
	 Light(light_flag_e::point, intensity), from{from} {
		// EMPTY
	}

	ColorXYZ PointLight::sample_Li( const Surfel& hit     /*in*/,
									Vector3f *wi          /*out*/,
									VisibilityTester *vis /*out*/ ) {

		// TODO
	}

	DirectionalLight::DirectionalLight(Point3f from, Point3f to, Vector3f intensity):
	 Light(light_flag_e::directional, intensity), from{from}, to{to} {
		// EMPTY
	}

	ColorXYZ DirectionalLight::sample_Li( const Surfel& hit     /*in*/,
									Vector3f *wi          /*out*/,
									VisibilityTester *vis /*out*/ ) {

		// TODO
	}

	AmbientLight::AmbientLight(Vector3f intensity):
	 Light(light_flag_e::ambient, intensity) {
		// EMPTY
	}

	ColorXYZ AmbientLight::sample_Li( const Surfel& hit     /*in*/,
									Vector3f *wi          /*out*/,
									VisibilityTester *vis /*out*/ ) {

		// TODO
	}

	SpotLight::SpotLight(Point3f from, Point3f to, Vector3f intensity, real_type cutoff, real_type falloff):
	 Light(light_flag_e::ambient, intensity), from{from}, to{to}, cutoff{cutoff}, falloff{falloff}{

	 }

	ColorXYZ SpotLight::sample_Li( const Surfel& hit     /*in*/,
									Vector3f *wi          /*out*/,
									VisibilityTester *vis /*out*/ ) {

		// TODO
	}

}