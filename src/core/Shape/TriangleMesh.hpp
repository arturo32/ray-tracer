#ifndef TRIANGLE_MESH_H
#define TRIANGLE_MESH_H

#include "../rt3-base.hpp"

namespace rt3 {

struct TriangleMesh {
	int n_triangles; //!< # of triangles in the mesh.
	// The size of the three lists below should be 3 * nTriangles. Every 3 values we have a triangle.
	vector< int > vertex_indices;  //!< The list of indices to the vertex list, for each individual triangle.
	vector< int > normal_indices;  //!< The list of indices to the normals list, for each individual triangle.
	vector< int > uvcoord_indices; //!< The list of indices to the UV coord list, for each individual triangle.

	// Here comes the data associated with each vertex. WARNING: The length of the vectors below may differ.
	vector< Point3f > vertices;  //!< The 3D geometric coordinates
	vector< Normal3f > normals;  //!< The 3D normals.
	vector< Point2f > uvcoords; //!< The 2D texture coordinates.

	// Regular constructor (almost all data is imported via move semantics).
	TriangleMesh(){/*empty*/};
	/// Copy constructor disabled
	TriangleMesh( const TriangleMesh& ) = delete;
	/// Assign operator disabled.
	TriangleMesh& operator=( const TriangleMesh & ) = delete;
	/// Move constructor.
	TriangleMesh( TriangleMesh && other ) = delete;
};


}
#endif  