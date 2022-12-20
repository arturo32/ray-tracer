#include "Triangle.hpp"

namespace rt3 {

	Triangle::Triangle( bool flip, const shared_ptr<Transform >& o2w, shared_ptr<TriangleMesh> mesh, int tri_id, bool bfc ) 
		: Shape(flip, o2w), mesh{mesh}, backface_cull{bfc} {
		
		// This is just a shortcut to access this triangle's data stored in the mesh database.
		v  = &mesh->vertex_indices[ 3 * tri_id ];
		n  = &mesh->normal_indices[ 3 * tri_id ];
		uv = &mesh->uvcoord_indices[ 3 * tri_id ];

		Matrix4x4 m = this->obj_to_world->GetMatrix();

		const Point3f& p0 = MultPoint(m, this->mesh->vertices[this->v[0]]); // Get the 3D coordinate of the 0-vertex of this triangle.
		const Point3f& p1 = MultPoint(m, this->mesh->vertices[this->v[1]]); // Same for the 1-vertex.
		const Point3f& p2 = MultPoint(m, this->mesh->vertices[this->v[2]]); // Same for the 2-vertex.
		
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
		// std::cout << "bounds tri = min:" << s << " | max:" << b << std::endl;
		bounds = MultBounds(o2w->GetMatrix(), Bounds3f{s, b});
	}

	bool Triangle::intersect( Ray& r, real_type& t_hit, Surfel *sf ) const {
		Ray newR = this->obj_to_world->GetInverseMatrix() * r;
		// Ray newR = r;
		Matrix4x4 m = this->obj_to_world->GetMatrix();

		const Point3f& p0 = MultPoint(m, this->mesh->vertices[this->v[0]]); // Get the 3D coordinate of the 0-vertex of this triangle.
		const Point3f& p1 = MultPoint(m, this->mesh->vertices[this->v[1]]); // Same for the 1-vertex.
		const Point3f& p2 = MultPoint(m, this->mesh->vertices[this->v[2]]); // Same for the 2-vertex.

		Vector3f edge1 = p1 - p0;
		Vector3f edge2 = p2 - p0;
		Vector3f pvec = cross(edge2, newR.direction);
		real_type det = dot(edge1, pvec);

		if(det > -EPSILON && det < EPSILON) {
			return false;
		}
		
		real_type inv_det = 1.0 / det;

		Vector3f tvec = newR.origin - p0;
		sf->uv[0] = dot(tvec, pvec) * inv_det;
		if(sf->uv[0] < 0.0  || sf->uv[0] > 1.0) {
			return false;
		}

		Vector3f qvec = cross(edge1, tvec);
		sf->uv[1] = dot(newR.direction, qvec) * inv_det;
		if(sf->uv[1] < 0.0  || (sf->uv[0] + sf->uv[1]) > 1.0) {
			return false;
		}

		t_hit = dot(edge2, qvec) * inv_det;
		if (t_hit < newR.t_max && t_hit > newR.t_min) {
			const Normal3f& n0 = MultVector(m, this->mesh->normals[this->n[0]]); // Retrieve the normal at vertex 0.
			const Normal3f& n1 = MultVector(m, this->mesh->normals[this->n[1]]); // Retrieve the normal at vertex 1.
			const Normal3f& n2 = MultVector(m, this->mesh->normals[this->n[2]]); // Retrieve the normal at vertex 2.
			sf->wo = -newR.direction;
			sf->p = newR(t_hit);
			sf->n = sf->uv[0] * n1 + sf->uv[1] * n2 + (1- sf->uv[0] - sf->uv[1])*n0; // cross(edge1, edge2);
			Matrix4x4 it = Transpose(this->obj_to_world->GetInverseMatrix());
			sf->n = MultVector(it, sf->n);
			sf->n.make_unit_vector();
			return true;
		}
		return false;
	}

	bool Triangle::intersect_p(const Ray& r) const {
		Ray newR = this->obj_to_world->GetInverseMatrix() * r;
		// Ray newR = r;
		Matrix4x4 m = this->obj_to_world->GetMatrix();

		const Point3f& p0 = MultPoint(m, this->mesh->vertices[this->v[0]]); // Get the 3D coordinate of the 0-vertex of this triangle.
		const Point3f& p1 = MultPoint(m, this->mesh->vertices[this->v[1]]); // Same for the 1-vertex.
		const Point3f& p2 = MultPoint(m, this->mesh->vertices[this->v[2]]); // Same for the 2-vertex.

		Vector3f edge1 = p1 - p0;
		Vector3f edge2 = p2 - p0;
		Vector3f pvec = cross(edge2, newR.direction);
		real_type det = dot(edge1, pvec);

		if(det > -EPSILON && det < EPSILON) {
			return false;
		}
		
		real_type inv_det = 1.0 / det;

		Vector3f tvec = newR.origin - p0;
		real_type u = dot(tvec, pvec) * inv_det;
		if(u < 0.0  || u > 1.0) {
			return false;
		}

		Vector3f qvec = cross(edge1, tvec);
		real_type v = dot(newR.direction, qvec) * inv_det;
		if(v < 0.0  || (u + v) > 1.0) {
			return false;
		}
		real_type t_hit = dot(edge2, qvec) * inv_det;
		if(t_hit < newR.t_max && t_hit > newR.t_min) {
			return true;
		}
		return false;
	}
}
