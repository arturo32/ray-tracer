#include "light.hpp"

namespace rt3 {
	VisibilityTester::VisibilityTester( const Surfel& a, const Surfel& b, real_type value)
	:p0{a}, p1{b}, value{value} {
		// EMPTY
	}

	bool VisibilityTester::unoccluded( const Scene& scene ) {
		// TODO
		return false;
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
		Vector3f dir = (this->from - hit.p);
		dir.make_unit_vector();
		// TODO vis
		wi->assign(dir);
		return ColorXYZ(0,0,0);
	}

	DirectionalLight::DirectionalLight(Point3f from, Point3f to, Vector3f intensity):
	 Light(light_flag_e::directional, intensity) {
		dir = (from-to);
		dir.make_unit_vector();
	}

	ColorXYZ DirectionalLight::sample_Li( const Surfel& hit     /*in*/,
									Vector3f *wi          /*out*/,
									VisibilityTester *vis /*out*/ ) {
		// TODO vis
		wi->assign(dir);
		return ColorXYZ(0,0,0);
	}

	AmbientLight::AmbientLight(Vector3f intensity):
	 Light(light_flag_e::ambient, intensity) {
		// EMPTY
	}

	ColorXYZ AmbientLight::sample_Li( const Surfel& hit     /*in*/,
									Vector3f *wi          /*out*/,
									VisibilityTester *vis /*out*/ ) {
		// EMPTY
		return ColorXYZ(0,0,0);
	}

	SpotLight::SpotLight(Point3f from, Point3f to, Vector3f intensity, real_type cutoff, real_type falloff):
	 Light(light_flag_e::ambient, intensity), from{from}, to{to}, cutoff{cutoff}, falloff{falloff}{

	 }

	ColorXYZ SpotLight::sample_Li( const Surfel& hit     /*in*/,
									Vector3f *wi          /*out*/,
									VisibilityTester *vis /*out*/ ) {
		// TODO cutoff and faloff 
		Vector3f dir = (this->from - hit.p);
		dir.make_unit_vector();
		// TODO vis
		wi->assign(dir);
		return ColorXYZ(0,0,0);
	}

}