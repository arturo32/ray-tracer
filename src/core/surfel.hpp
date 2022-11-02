#ifndef SURFEL_H
#define SURFEL_H

#include "rt3.hpp"
#include "../ext/vec2.hpp"
#include "../ext/vec3.hpp"

namespace rt3 {

using Point2f = vec2<real_type>;
using Point3f = vec3<real_type>;
using Vector3f = vec3<real_type>;

class Primitive;

class Surfel{
	public:
		Surfel(){};
		// Surfel( const Point3f&p,const Vector3f&n, const Vector3f&wo, real_type time, 
		// 	const Point2f& uv, const Primitive *pri )
		// 	: p{p}, n{n}, wo{wo}, time{time}, uv{uv}, primitive{pri}
        //     {/* empty */};

		Surfel( const Point3f&p,const Vector3f&n, const Vector3f&wo, real_type time, 
			const Point2f& uv )
			: p{p}, n{n}, wo{wo}, time{time}, uv{uv}
            {/* empty */};

		Point3f p;        //!< Contact point.
		Vector3f n;       //!< The surface normal.
		Vector3f wo;      //!< Outgoing direction of light, which is -ray.
		real_type time = 0;     //!< Time of contact.
		Point2f uv;     //!< Parametric coordinate (u,v) of the hit surface.
		const Primitive* primitive = nullptr; //!< Pointer to the primitive.
		bool hit = false;
};
}

#endif  
