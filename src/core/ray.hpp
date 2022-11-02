#ifndef RAY_H
#define RAY_H

#include "rt3.hpp"

#include "../ext/vec3.hpp"

namespace rt3 {
using Point3f = vec3<real_type>;
using Vector3f = vec3<real_type>;
    class Ray {
        public:
            Ray (const Point3f& o, const Vector3f& d ) : origin{o}, direction{d} {/*empty*/}
            const Point3f origin; //!< origin
            const Vector3f direction; //!< direction
            real_type t_max = INFINITY;
            real_type t_min = 0;
            // O que precisa ser normalizado exatamente?
            // Point3f normalize() const { return ???; }
            Point3f operator()(real_type t) const { return origin + direction * t; }
            
    };
}
#endif // RAY_H
