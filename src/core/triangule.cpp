#include "triangule.hpp"

#define EPSILON 0.000001

namespace rt3 {

    bool Triangule::intersect( const Ray& r, real_type& t_hit, Surfel *sf ) const {
        // ?std::cout << "testeeeeeeee" << std::endl;
        real_type inv_det;

        Vector3f edge1 = this->points[1] - this->points[0];
        Vector3f edge2 = this->points[2] - this->points[0];
        Vector3f pvec = cross(edge2, r.direction);
        real_type det = dot(edge1, pvec);

        if(det > -EPSILON && det < EPSILON) {
            return false;
        }
        
        inv_det = 1.0 / det;

        Vector3f tvec = r.origin - this->points[0];
        sf->uv[0] = dot(tvec, pvec) * inv_det;
        if(sf->uv[0] < 0.0  || sf->uv[0] > 1.0) {
            return false;
        }

        Vector3f qvec = cross(edge1, tvec);
        sf->uv[1] = dot(r.direction, qvec) * inv_det;
        if(sf->uv[1] < 0.0  || sf->uv[1] > 1.0) {
            return false;
        }

        t_hit = dot(edge2, qvec) * inv_det;
        sf->wo = -r.direction;
        sf->p = r(t_hit);
        sf->n = cross(edge1, edge2);
        sf->n.make_unit_vector();
        return true;
    }

    bool Triangule::intersect_p(const Ray& r) const {
        real_type inv_det;

        Vector3f edge1 = this->points[1] - this->points[0];
        Vector3f edge2 = this->points[2] - this->points[0];
        Vector3f pvec = cross(edge2, r.direction);
        real_type det = dot(edge1, pvec);

        if(det > -EPSILON && det < EPSILON) {
            return false;
        }
        
        inv_det = 1.0 / det;

        Vector3f tvec = r.origin - this->points[0];
        real_type u = dot(tvec, pvec) * inv_det;
        if(u < 0.0  || u > 1.0) {
            return false;
        }

        Vector3f qvec = cross(edge1, tvec);
        real_type v = dot(r.direction, qvec) * inv_det;
        if(v < 0.0  || v > 1.0) {
            return false;
        }
        return true;
    }

    Bounds3f Triangule::world_bounds() const {
        std::cout << "vish" << std::endl;
        return Bounds3f{Point3f{}, Point3f{}};
    }
}
