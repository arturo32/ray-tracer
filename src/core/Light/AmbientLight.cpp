#include "light.hpp"
    
namespace rt3 {
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
} // namespace rt3    

    
    
    
    