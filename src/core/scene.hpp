#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "primitive.hpp"
#include "background.hpp"
#include "rt3-base.hpp"
#include "rt3.hpp"
#include "light.hpp"
#include "../ext/vec2.hpp"
#include "../ext/vec3.hpp"

namespace rt3 {

using Point2f = vec2<real_type>;
using Point3f = vec3<real_type>;
using Vector3f = vec3<real_type>;

class Scene {
    //=== Public data
    public:
        std::vector<shared_ptr<Light>> lights; // list of lights
        shared_ptr<AmbientLight> ambientLight; // ambient light
        std::shared_ptr<Background> background; // The background object.
        std::shared_ptr<AggregatePrimitive> agg; // The scene graph of objects, acceleration structure.

        Scene(){agg = make_shared<PrimList>();}

        /// Determines the intersection info; return true if there is an intersection.
        void intersect( Ray& r, Surfel *sf ) const {
            agg->intersect(r, sf);
        };
        /*! A faster version that only determines whether there is an intersection or not;
         * it doesn't calculate the intersection info.
         */
        bool intersect_p( const Ray& r ) const {
            return agg->intersect_p(r);
        };
        void add_object(std::shared_ptr<Primitive> p) {agg->add(p);};
        void add_light(std::shared_ptr<Light> l) {lights.push_back(l);};
};

}

#endif  
