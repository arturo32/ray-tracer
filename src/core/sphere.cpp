#include "sphere.h"

namespace rt3 {

    bool Sphere::intersect( const Ray& r, real_type& t_hit, Surfel *sf ) const {
        Vector3f oc = (r.origin - this->center);
        real_type A = dot(r.direction, r.direction);
        real_type B = dot(oc, r.direction);
        real_type C = dot(oc, oc) - (this->radius * this->radius);
        real_type delta = B*B - A*C;
        if (delta >= 0) {
            real_type sqrtD = sqrt(delta);
            real_type t1 = (-B + sqrtD) / A;
            real_type t2 = (-B - sqrtD) / A;
            t_hit = (t1 < t2) ? t1 : t2;
            if (t_hit < r.t_max && t_hit > r.t_min) {
                // TODO: calcular as outras informações do surfel
                sf->wo = -r.direction;
                sf->p = r(t_hit);
                sf->n = sf->p - this->center;
                return true;    
            }
            return false;
        } else {
            return false;
        }
    }

    // bool Sphere::intersect( const Ray& r, real_type& t_hit, Surfel *sf ) const {
    //     Vector3f oc = (r.origin - this->center);
    //     real_type ocd = dot(oc, r.direction);
    //     real_type dir2 = dot(r.direction, r.direction);
    //     real_type delta = (ocd * ocd) - (dir2 * dot(oc, oc) - (this->radius * this->radius));
    //     if (delta >= 0) {
    //         real_type sqrtD = sqrt(delta);
    //         real_type t1 =  (-ocd + sqrtD) / dir2;
    //         real_type t2 =  (-ocd - sqrtD) / dir2;
    //         t_hit = (t1 < t2) ? t1 : t2;
    //         // std::cout << "t1 " << t1 << std::endl;
    //         // std::cout << "t2 " << t2 << std::endl;
    //         if (t_hit < r.t_max && t_hit > r.t_min) {
    //             // std::cout << "t_hit: " << t_hit << std::endl;
    //             // TODO: calcular as outras informações do surfel
    //             sf->wo = -r.direction;
    //             sf->p = r(t_hit);
    //             sf->n = sf->p - this->center;
    //             return true;    
    //         }
    //         return false;
    //     } else {
    //         return false;
    //     }
    // }

    bool Sphere::intersect_p( const Ray& r ) const {
        Point3f oc = (r.origin - this->center);
        real_type ocd2 = dot(oc, r.direction);
        real_type delta = ocd2 * ocd2
            - (dot(r.direction, r.direction) * dot(oc, oc) - this->radius * this->radius);
        return delta >= 0;
    }

    Bounds3f Sphere::world_bounds() const {
        real_type npX = this->center.x() - this->radius;
        real_type npY = this->center.y() - this->radius;
        real_type npZ = this->center.z() + this->radius;
        Point3f nearest_point = Point3f{npX, npY, npZ};

        real_type fpX = this->center.x() + this->radius;
        real_type fpY = this->center.y() + this->radius;
        real_type fpZ = this->center.z() - this->radius;
        Point3f furthest_point = Point3f{fpX, fpY, fpZ};

        return Bounds3f{nearest_point, furthest_point};
    }

    std::shared_ptr<GeometricPrimitive> create_sphere(const ParamSet &object_ps, std::shared_ptr<Material> material) {
        real_type radius = retrieve(object_ps, "radius", real_type{0});
        Point3f center = retrieve(object_ps, "center", Point3f{0.0,0.0,0.0});
        std::shared_ptr<Sphere> sphere = make_shared<Sphere>(false, center, radius);
        return make_shared<GeometricPrimitive>(material, sphere);
    }
}
