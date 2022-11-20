#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../rt3-base.hpp"
#include "shape.hpp"
#include "TriangleMesh.hpp"

namespace rt3 {

/// Represents a single triangle.
class Triangle : public Shape {
	private:
		int *v; //!< Pointer to the vertex index array stored in the triangle mesh database. 
		// After the proper triangle initialization, we may access vertices with v[0], v[1], and v[2]
		int *n; //!< Pointer to the normal index array stored in the triangle mesh database.
		// After the proper triangle initialization, we may access normals with n[0], n[1], and n[2]
		int *uv;//!< Pointer to the UV coord index array stored in the triangle mesh database.
		// After the proper triangle initialization, we may access UV coords with uv[0], uv[1], and uv[2]

		shared_ptr<TriangleMesh> mesh; //!< This is the **indexed triangle mesh database** this triangle is linked to.
		bool backface_cull;            //!< Set it `true` to activate backface culling.
	
	public:
        // The single constructor, that receives the mesh, this triangle id, and an indication for backface culling.
		Triangle( bool flip, shared_ptr<TriangleMesh> mesh, int tri_id, bool bfc=true )
			: Shape(flip), mesh{mesh}, backface_cull{bfc}
		{
			// This is just a shortcut to access this triangle's data stored in the mesh database.
			v  = &mesh->vertex_indices[ 3 * tri_id ];
			n  = &mesh->normal_indices[ 3 * tri_id ];
			uv = &mesh->uvcoord_indices[ 3 * tri_id ];
			// std::cout << "n[0]=" << n[0] << " " << mesh->normals[n[0]] <<  std::endl;
			// std::cout << "n[1]=" << n[1] << " " << mesh->normals[n[1]]<< std::endl;
			// std::cout << "n[2]=" << n[2] << " " << mesh->normals[n[2]]<< std::endl;

			// std::cout << "v[0]=" << v[0] << " " << mesh->vertices[v[0]] <<  std::endl;
			// std::cout << "v[1]=" << v[1] << " " << mesh->vertices[v[1]] << std::endl;
			// std::cout << "v[2]=" << v[2] << " " << mesh->vertices[v[2]] << std::endl;

			// std::cout << "uv[0]=" << uv[0] << " " << mesh->uvcoords[uv[0]] <<  std::endl;
			// std::cout << "uv[1]=" << uv[1] << " " << mesh->uvcoords[uv[1]] << std::endl;
			// std::cout << "uv[2]=" << uv[2] << " " << mesh->uvcoords[uv[2]] << std::endl;
		}
		/// Return the triangle's bounding box.
		// Bounds3f object_bound() const;
		/// The regular intersection methods, as defined in the Shape parent class.
		bool intersect(const Ray &ray, real_type &thit, Surfel *isect ) const;
		bool intersect_p( const Ray &ray ) const;

		/// This friend function helps us debug the triangles, if we want to.
		friend std::ostream& operator<<( std::ostream& os, const Triangle & t );
		Bounds3f world_bounds() const;
};

}
#endif  