#include "light.hpp"
    
namespace rt3 {
    SpotLight::SpotLight(Point3f from, Point3f to, Vector3f intensity, real_type cutoff,
    real_type falloff, real_type kc, real_type kl, real_type kq, bool attenuate):
    Light(light_flag_e::spot, intensity, kc, kl, kq, attenuate),
    from{from}, to{to}, cutoff{cutoff}, falloff{falloff}{

    }

	ColorXYZ SpotLight::sample_Li( const Surfel& hit     /*in*/,
									Vector3f *wi          /*out*/,
									VisibilityTester *vis /*out*/ ) {
		Vector3f dir = (this->from - hit.p);
		real_type mod = a ? attenuate(dir.length()) : 1;
		Vector3f spot_dir = (this->from - this->to);
		vis->light_distance = dir.length();
		dir.make_unit_vector();
		wi->assign(dir);
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
	
} // namespace rt3    

    
    
    
    

