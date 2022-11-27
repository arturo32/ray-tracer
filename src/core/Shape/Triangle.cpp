#include "Triangle.hpp"

namespace rt3 {

	Triangle::Triangle( bool flip, shared_ptr<TriangleMesh> mesh, int tri_id, bool bfc ) 
		: Shape(flip), mesh{mesh}, backface_cull{bfc} {
		
		// This is just a shortcut to access this triangle's data stored in the mesh database.
		v  = &mesh->vertex_indices[ 3 * tri_id ];
		n  = &mesh->normal_indices[ 3 * tri_id ];
		uv = &mesh->uvcoord_indices[ 3 * tri_id ];

		const Point3f& p0 = this->mesh->vertices[this->v[0]]; // Get the 3D coordinate of the 0-vertex of this triangle.
		const Point3f& p1 = this->mesh->vertices[this->v[1]]; // Same for the 1-vertex.
		const Point3f& p2 = this->mesh->vertices[this->v[2]]; // Same for the 2-vertex.
		
		Point3f s = p0;
		Point3f b = p0;
		for (size_t i = 0; i < 3; ++i) {
			// small
			if (p1[i] < s[i]) {
				s[i] = p1[i];
			}
			if (p2[i] < s[i]) {
				s[i] = p2[i];
			}
			// big
			if (p1[i] > b[i]) {
				b[i] = p1[i];
			}
			if (p2[i] > b[i]) {
				b[i] = p2[i];
			}
		}
		std::cout << "bounds tri = min:" << s << " | max:" << b << std::endl;
		bounds = Bounds3f{s, b};
	}

	bool Triangle::intersect( Ray& r, real_type& t_hit, Surfel *sf ) const {
		const Point3f& p0 = this->mesh->vertices[this->v[0]]; // Get the 3D coordinate of the 0-vertex of this triangle.
		const Point3f& p1 = this->mesh->vertices[this->v[1]]; // Same for the 1-vertex.
		const Point3f& p2 = this->mesh->vertices[this->v[2]]; // Same for the 2-vertex.

		const Normal3f& n0 = this->mesh->normals[this->n[0]]; // Retrieve the normal at vertex 0.
		const Normal3f& n1 = this->mesh->normals[this->n[1]]; // Retrieve the normal at vertex 1.
		const Normal3f& n2 = this->mesh->normals[this->n[2]]; // Retrieve the normal at vertex 2.

		//const Point2f& uv0 = this->mesh->uvcoords[this->uv[0]]; // Retrieve the uv coord at vertex 0.
		//const Point2f& uv1 = this->mesh->uvcoords[this->uv[1]]; // Retrieve the uv coord at vertex 1.
		//const Point2f& uv2 = this->mesh->uvcoords[this->uv[2]]; // Retrieve the uv coord at vertex 2.
		
		Vector3f edge1 = p1 - p0;
		Vector3f edge2 = p2 - p0;
		Vector3f pvec = cross(edge2, r.direction);
		real_type det = dot(edge1, pvec);

		if(det > -EPSILON && det < EPSILON) {
			return false;
		}
		
		real_type inv_det = 1.0 / det;

		Vector3f tvec = r.origin - p0;
		sf->uv[0] = dot(tvec, pvec) * inv_det;
		if(sf->uv[0] < 0.0  || sf->uv[0] > 1.0) {
			return false;
		}

		Vector3f qvec = cross(edge1, tvec);
		sf->uv[1] = dot(r.direction, qvec) * inv_det;
		if(sf->uv[1] < 0.0  || (sf->uv[0] + sf->uv[1]) > 1.0) {
			return false;
		}

		t_hit = dot(edge2, qvec) * inv_det;
		if (t_hit < r.t_max && t_hit > r.t_min) {
			sf->wo = -r.direction;
			sf->p = r(t_hit);
			sf->n = sf->uv[0] * n1 + sf->uv[1] * n2 + (1- sf->uv[0] - sf->uv[1])*n0; // cross(edge1, edge2);
			sf->n.make_unit_vector();
			return true;
		}
		return false;
	}

	bool Triangle::intersect_p( Ray& r) const {
		const Point3f& p0 = this->mesh->vertices[this->v[0]]; // Get the 3D coordinate of the 0-vertex of this triangle.
		const Point3f& p1 = this->mesh->vertices[this->v[1]]; // Same for the 1-vertex.
		const Point3f& p2 = this->mesh->vertices[this->v[2]]; // Same for the 2-vertex.

		Vector3f edge1 = p1 - p0;
		Vector3f edge2 = p2 - p0;
		Vector3f pvec = cross(edge2, r.direction);
		real_type det = dot(edge1, pvec);

		if(det > -EPSILON && det < EPSILON) {
			return false;
		}
		
		real_type inv_det = 1.0 / det;

		Vector3f tvec = r.origin - p0;
		real_type u = dot(tvec, pvec) * inv_det;
		if(u < 0.0  || u > 1.0) {
			return false;
		}

		Vector3f qvec = cross(edge1, tvec);
		real_type v = dot(r.direction, qvec) * inv_det;
		if(v < 0.0  || (u + v) > 1.0) {
			return false;
		}
		real_type t_hit = dot(edge2, qvec) * inv_det;
		if(t_hit < r.t_max && t_hit > r.t_min) {
			return true;
		}
		return false;
	}
}
