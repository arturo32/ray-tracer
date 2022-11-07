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

	Light::Light(light_flag_e t, Vector3f intensity,
	 real_type kc, real_type kl, real_type kq, bool attenuate):
	 flags{t}, intensity{intensity}, kc{kc}, kl{kl}, kq{kq}, a{attenuate} {
		//EMPTY
	}

	real_type Light::attenuate(real_type d) {
		return 1.0 / (kc + kl*d + kq*d*d);
	}

	Light::~Light() = default;

	PointLight::PointLight(Point3f from, Vector3f intensity,
	 real_type kc, real_type kl, real_type kq, bool attenuate):
	 Light(light_flag_e::point, intensity, kc, kl, kq, attenuate), from{from} {
		// EMPTY
	}

	ColorXYZ PointLight::sample_Li( const Surfel& hit     /*in*/,
									Vector3f *wi          /*out*/,
									VisibilityTester *vis /*out*/ ) {
		Vector3f dir = (this->from - hit.p);
		real_type mod = a ? attenuate(dir.length()) : 1;
		dir.make_unit_vector();
		// TODO vis
		wi->assign(dir);
		return mod * intensity;
	}

	DirectionalLight::DirectionalLight(Point3f from, Point3f to, Vector3f intensity,
	 real_type kc, real_type kl, real_type kq, bool attenuate):
	 Light(light_flag_e::directional, intensity, kc, kl, kq, attenuate) {
		dir = (from-to);
		dir.make_unit_vector();
	}

	ColorXYZ DirectionalLight::sample_Li( const Surfel& hit     /*in*/,
									Vector3f *wi          /*out*/,
									VisibilityTester *vis /*out*/ ) {
		// pode ser atenuada?
		// TODO vis
		wi->assign(dir);
		return intensity;
	}

	AmbientLight::AmbientLight(Vector3f intensity,
	 real_type kc, real_type kl, real_type kq, bool attenuate):
	 Light(light_flag_e::ambient, intensity, kc, kl, kq, attenuate) {
		// EMPTY
	}

	ColorXYZ AmbientLight::sample_Li( const Surfel& hit     /*in*/,
									Vector3f *wi          /*out*/,
									VisibilityTester *vis /*out*/ ) {
		// pode ser atenuada?
		return intensity;
	}

	SpotLight::SpotLight(Point3f from, Point3f to, Vector3f intensity, real_type cutoff,
	 real_type falloff, real_type kc, real_type kl, real_type kq, bool attenuate):
	 Light(light_flag_e::ambient, intensity, kc, kl, kq, attenuate),
	  from{from}, to{to}, cutoff{cutoff}, falloff{falloff}{

	 }

	ColorXYZ SpotLight::sample_Li( const Surfel& hit     /*in*/,
									Vector3f *wi          /*out*/,
									VisibilityTester *vis /*out*/ ) {
		Vector3f dir = (this->from - hit.p);
		real_type mod = a ? attenuate(dir.length()) : 1;
		Vector3f spot_dir = (this->from - this->to);
		wi->assign(dir);
		dir.make_unit_vector();
		spot_dir.make_unit_vector();
		real_type theta = dot(dir, spot_dir);
		real_type epsilon = cutoff - falloff;
		real_type l = Clamp((theta - falloff) / epsilon, 0.0, 1.0);
		// TODO vis
		if(theta > cutoff) {
			return mod * intensity;
		} else if(theta > falloff) {
			return mod * intensity * l;
		}
		return ColorXYZ(0,0,0);
	}

}