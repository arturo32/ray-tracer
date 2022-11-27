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
		Bounds3f bounds;
	public:
        // The single constructor, that receives the mesh, this triangle id, and an indication for backface culling.
		Triangle( bool flip, shared_ptr<TriangleMesh> mesh, int tri_id, bool bfc=true );
		/// The regular intersection methods, as defined in the Shape parent class.
		bool intersect( Ray &ray, real_type &thit, Surfel *isect ) const;
		bool intersect_p( Ray &ray ) const;

		/// This friend function helps us debug the triangles, if we want to.
		friend std::ostream& operator<<( std::ostream& os, const Triangle & t );
		Bounds3f world_bounds() const {
			return bounds;
		}
};

}
#endif  