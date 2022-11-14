#include "light.hpp"
    
namespace rt3 {
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
		vis->light_distance = INFINITY;
		wi->assign(dir);
		return intensity;
	}
} // namespace rt3