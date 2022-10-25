#ifndef LIGHT_H
#define LIGHT_H

#include "rt3.hpp"
#include "surfel.hpp"
#include "scene.hpp"

namespace rt3 {

enum class light_flag_e : int {
	point = 1,
	directional = 2,
	area = 4,
	ambient = 8,
	spot = 16
};

bool is_ambient( int flag ) {
	return flag & (int) light_flag_e::ambient;
}

class VisibilityTester {
	public:
		VisibilityTester()=default;
		VisibilityTester( const Surfel&, const Surfel& );
		bool unoccluded( const Scene& scene );
	public:
		Surfel p0, p1;
};

class Light {
  public:
    light_flag_e flags;
  public:
    Light(/* args */);
    virtual ~Light() = 0;
    virtual ColorXYZ sample_Li( const Surfel& hit     /*in*/,
								Vector3f *wi          /*out*/,
								VisibilityTester *vis /*out*/ ) = 0;
    virtual void preprocess( const Scene & ) {};

};

class PointLight : public Light {
    // TODO
}


}
#endif