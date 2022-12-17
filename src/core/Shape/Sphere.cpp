#include "Sphere.hpp"

namespace rt3 {

	bool Sphere::intersect( Ray& r, real_type& t_hit, Surfel *sf ) const {
		//Ray newR = r;
		Ray newR = this->obj_to_world->GetInverseMatrix() * r;
		Vector3f oc = (newR.origin - this->center);
		real_type A = dot(newR.direction, newR.direction);
		real_type B = dot(oc, newR.direction);
		real_type C = dot(oc, oc) - (this->radius * this->radius);
		real_type delta = B*B - A*C;
		if (delta >= 0) {
			real_type sqrtD = sqrt(delta);
			real_type t1 = (-B + sqrtD) / A;
			real_type t2 = (-B - sqrtD) / A;
			t_hit = (t1 < t2) ? t1 : t2;
			if (t_hit < newR.t_max && t_hit > newR.t_min) {
				// TODO: calcular as outras informações do surfel
				sf->wo = -newR.direction;
				sf->p = newR(t_hit);
				sf->n = sf->p - this->center;
				sf->n.make_unit_vector();
				return true;    
			}
			return false;
		} else {
			return false;
		}
	}

	bool Sphere::intersect_p(const Ray& r) const {
		//Ray newR = r;
		Ray newR = this->obj_to_world->GetInverseMatrix() * r;
		Vector3f oc = (newR.origin - this->center);
		real_type A = dot(newR.direction, newR.direction);
		real_type B = dot(oc, newR.direction);
		real_type C = dot(oc, oc) - (this->radius * this->radius);
		real_type delta = B*B - A*C;
		if (delta >= 0) {
			real_type sqrtD = sqrt(delta);
			real_type t1 = (-B + sqrtD) / A;
			real_type t2 = (-B - sqrtD) / A;
			real_type t_hit = (t1 < t2) ? t1 : t2;
			return (t_hit < newR.t_max && t_hit > newR.t_min);
		}
		return false;
	}

	// Bounds3f Sphere::world_bounds() const {
	// 	// versão inicial
	// 	real_type npX = this->center.x() - this->radius;
	// 	real_type npY = this->center.y() - this->radius;
	// 	real_type npZ = this->center.z() + this->radius;
	// 	Point3f nearest_point = Point3f{npX, npY, npZ};

	// 	real_type fpX = this->center.x() + this->radius;
	// 	real_type fpY = this->center.y() + this->radius;
	// 	real_type fpZ = this->center.z() - this->radius;
	// 	Point3f furthest_point = Point3f{fpX, fpY, fpZ};
	// 	return Bounds3f{nearest_point, furthest_point};

	// 	// versão do next week
	// 	Point3f r3 = vec3(radius, radius, radius);
	// 	std::cout << "bounds sphere = min:" << center - r3 << " | max:" << center + r3 << std::endl;
	// 	return Bounds3f(center - r3, center + r3);
	// }

} // rt3 namespace
