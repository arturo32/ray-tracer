#ifndef LIGHT_H
#define LIGHT_H

#include "rt3.hpp"
#include "surfel.hpp"

namespace rt3 {

class Scene;

class VisibilityTester {
	public:
		VisibilityTester()=default;
		~VisibilityTester()=default;
		VisibilityTester( const Surfel& a, const Surfel& b);
		bool unoccluded( const Scene& scene );
	public:
		Surfel p0, p1;
};

enum class light_flag_e : int {
	point = 1,
	directional = 2,
	area = 4,
	ambient = 8,
	spot = 16
};

// bool is_ambient( int flag ) {
// 	return flag & (int) light_flag_e::ambient;
// }

class Light {
  public:
    light_flag_e flags;
	Vector3f intensity;

  public:
    Light(light_flag_e t, Vector3f intensity);
    virtual ~Light() = 0;
	// Retorna a intensidade da luz, direção e o teste oclusão.
    virtual ColorXYZ sample_Li( const Surfel& hit     /*in*/,
								Vector3f *wi          /*out*/,
								VisibilityTester *vis /*out*/ ) = 0;
    virtual void preprocess( const Scene & ) {};

};

class PointLight : public Light {
  public:
	Point3f from;
	PointLight(Point3f from, Vector3f intensity);
	~PointLight() {};
    ColorXYZ sample_Li( const Surfel& hit     /*in*/,
						Vector3f *wi          /*out*/,
						VisibilityTester *vis /*out*/ );

};

class DirectionalLight : public Light {
  public:
	Point3f from;
	Point3f to;
	DirectionalLight(Point3f from, Point3f to, Vector3f intensity);
	~DirectionalLight() {};
    ColorXYZ sample_Li( const Surfel& hit     /*in*/,
						Vector3f *wi          /*out*/,
						VisibilityTester *vis /*out*/ );
};

class AmbientLight : public Light {
  public:
  	AmbientLight(Vector3f intensity);
	~AmbientLight() {};
    ColorXYZ sample_Li( const Surfel& hit     /*in*/,
						Vector3f *wi          /*out*/,
						VisibilityTester *vis /*out*/ );
};

class SpotLight : public Light {
  public:
  	Point3f from;
	Point3f to;
	real_type cutoff;
	real_type falloff;
	SpotLight(Point3f from, Point3f to, Vector3f intensity, real_type cutoff, real_type falloff);
	~SpotLight() {};
    ColorXYZ sample_Li( const Surfel& hit     /*in*/,
						Vector3f *wi          /*out*/,
						VisibilityTester *vis /*out*/ );
};


}
#endif