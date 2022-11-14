#include "light.hpp"

namespace rt3 {
	VisibilityTester::VisibilityTester( const Surfel& a, const Surfel& b, real_type value)
	:p0{a}, p1{b}, light_distance{value} {
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


}