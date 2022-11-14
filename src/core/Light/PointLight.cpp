#include "light.hpp"
    
namespace rt3 {
    PointLight::PointLight(Point3f from, Vector3f intensity,
	 real_type kc, real_type kl, real_type kq, bool attenuate):
	 Light(light_flag_e::point, intensity, kc, kl, kq, attenuate), from{from} {
		//EMPTY
	}

	ColorXYZ PointLight::sample_Li( const Surfel& hit     /*in*/,
									Vector3f *wi          /*out*/,
									VisibilityTester *vis /*out*/ ) {
		Vector3f dir = (this->from - hit.p);
		real_type mod = this->a ? attenuate(dir.length()) : 1;
		vis->light_distance = dir.length();
		dir.make_unit_vector();
		// TODO vis
		wi->assign(dir);
		return mod * intensity;
	}
} // namespace rt3