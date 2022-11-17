#ifndef TRIANGLE_MESH_H
#define TRIANGLE_MESH_H

#include "../rt3-base.hpp"

namespace rt3 {

struct TriangleMesh {
    std::vector<Point3f> points;
    std::vector<Normal3f> normals;
    std::vector<Point2f> uvs;
    long n_triangles = 0;
    vector<Point3i> vertex_indices;
    vector<Point3i> normal_indices;
    vector<Point3i> uvcoord_indices;
};

/// This is the function that converts from the tinyobjloader's internal storage to our mesh data organization.
void extract_obj_data( const tinyobj::attrib_t& attrib,
                       const std::vector<tinyobj::shape_t>& shapes,
                       bool rvo, bool cn, bool fn, /* OUT */ shared_ptr<TriangleMesh> md )
{
    std::cout << "-- SUMMARY of the OBJ file --\n";
    std::cout << "# of vertices  : " << (attrib.vertices.size()  / 3) << std::endl;
    std::cout << "# of normals   : " << (attrib.normals.size()   / 3) << std::endl;
    std::cout << "# of texcoords : " << (attrib.texcoords.size() / 2) << std::endl;
    std::cout << "# of shapes    : " << shapes.size()                 << std::endl;
    std::cout << "-----------------------------\n";

    // Retrieve the complete list of vertices.
    auto n_vertices{ attrib.vertices.size() / 3 };
    for ( auto idx_v{0u} ; idx_v < n_vertices; idx_v++)
    {
        cout << "   v[" << static_cast<long>(idx_v) << "] = ( "
             << static_cast<double>(attrib.vertices[3 * idx_v + 0]) << ", "
             << static_cast<double>(attrib.vertices[3 * idx_v + 1]) << ", "
             << static_cast<double>(attrib.vertices[3 * idx_v + 2]) << " )\n";

        // Store the vertex in the mesh data structure.
        md->points.push_back( Point3f{ attrib.vertices[ 3 * idx_v + 0 ],
                                        attrib.vertices[ 3 * idx_v + 1 ],
                                        attrib.vertices[ 3 * idx_v + 2 ] } );
    }


    // Read the normals
    // TODO: flip normals if requested.
    real_type flip = ( fn ) ? -1 : 1 ;
    auto n_normals{ attrib.normals.size()/3 };

    // Do we need to compute the normals? Yes only if the user requeste or there are no normals in the file.
    // if ( cn == true or n_normals == 0)
    // {
    //    // TODO: COmpute normals here.
    //    // compute_normals();
    // }
    //else {// Read normals from file. This corresponds to the entire 'for' below.

    // Traverse the normals read from the OBJ file.
    for ( auto idx_n{0u} ; idx_n < n_normals; idx_n++)
    {
        cout << "   n[" << static_cast<long>(idx_n) << "] = ( "
             << static_cast<double>(attrib.normals[3 * idx_n + 0]) << ", "
             << static_cast<double>(attrib.normals[3 * idx_n + 1]) << ", "
             << static_cast<double>(attrib.normals[3 * idx_n + 2]) << " )\n";

        // Store the normal.
        md->normals.push_back( Normal3f{ attrib.normals[ 3 * idx_n + 0 ] * flip,
                                        attrib.normals[ 3 * idx_n + 1 ] * flip,
                                        attrib.normals[ 3 * idx_n + 2 ] * flip } );
    }

    // Read the complete list of texture coordinates.
    auto n_texcoords{ attrib.texcoords.size()/2 };
    for ( auto idx_tc{0u} ; idx_tc < n_texcoords; idx_tc++)
    {
        cout << "   t[" << static_cast<long>(idx_tc) << "] = ( "
             << static_cast<double>(attrib.texcoords[2 * idx_tc + 0]) << ", "
             << static_cast<double>(attrib.texcoords[2 * idx_tc + 1]) << " )\n";

        // Store the texture coords.
        md->uvs.push_back( Point2f{ attrib.texcoords[ 2 * idx_tc + 0 ],
                                         attrib.texcoords[ 2 * idx_tc + 1 ] } );
    }

    // Read mesh connectivity and store it as lists of indices to the real data.
    auto n_shapes{ shapes.size() };
    md->n_triangles = 0; // We must reset this here.
    // In case the OBJ file has the triangles organized in several shapes or groups, we
    // ignore this and store all triangles as a single mesh dataset.
    // This is why we need to reset the triangle count here.
    for ( auto idx_s{0u} ; idx_s < n_shapes; idx_s++)
    {
        cout << "The shape[" << idx_s << "].name = " << shapes[idx_s].name << '\n';
        cout << "Size of shape["<< idx_s << "].mesh.indices: "
             << static_cast<unsigned long>(shapes[idx_s].mesh.indices.size()) << '\n';
        cout << "shape["<< idx_s << "].num_faces: "
            <<  static_cast<unsigned long>(shapes[idx_s].mesh.num_face_vertices.size()) << '\n';

        // For each face print out the indices lists.
        size_t index_offset = 0;
        // # of triangles for this "shape" (group).
        // NOTE that we are accumulate the number of triangles coming from the shapes present in the OBJ file.
        md->n_triangles += shapes[idx_s].mesh.num_face_vertices.size();
        for ( auto idx_f{0} ; idx_f < md->n_triangles; idx_f++)
        {
            // Number of vertices per face (always 3, in our case)
            size_t fnum = shapes[idx_s].mesh.num_face_vertices[idx_f];

            cout << "  face[" << idx_f << "].fnum = "  << static_cast<unsigned long>(fnum) << '\n';

            // TODO: Invert order of vertices if flag is on. (DONE!)
            if ( rvo == true ) {
                std::cout << "Reverse order\n";
                // For each vertex in the face print the corresponding indices
                for (int v = fnum-1; v >= 0 ; v -= 3)
                {
                    tinyobj::index_t idx3 = shapes[idx_s].mesh.indices[index_offset + v];
                    tinyobj::index_t idx2 = shapes[idx_s].mesh.indices[index_offset + (v-1)];
                    tinyobj::index_t idx1 = shapes[idx_s].mesh.indices[index_offset + (v-2)];
                    // cout << "    face[" << idx_f << "].v[" << v << "].indices = "
                    //     << idx.vertex_index << "/" << idx.normal_index << "/" << idx.texcoord_index << '\n';
                    // Add the indices to the global list of indices we need to pass on to the mesh object.
                    md->vertex_indices.push_back( Point3i{idx1.vertex_index, idx2.vertex_index, idx3.vertex_index} );
                    md->normal_indices.push_back( Point3i{idx1.normal_index, idx2.normal_index, idx3.normal_index} );
                    md->uvcoord_indices.push_back( Point3i{idx1.texcoord_index, idx2.texcoord_index, idx3.texcoord_index} );
                }
            }
            else { // Keep the original vertex order
                // For each vertex in the face get the corresponding indices
                for (size_t v = 0; v < fnum; v += 3)
                {
                    tinyobj::index_t idx1 = shapes[idx_s].mesh.indices[index_offset + v];
                    tinyobj::index_t idx2 = shapes[idx_s].mesh.indices[index_offset + v + 1];
                    tinyobj::index_t idx3 = shapes[idx_s].mesh.indices[index_offset + v + 2];
                    // cout << "    face[" << idx_f << "].v[" << v << "].indices = "
                        // << idx.vertex_index << "/" << idx.normal_index << "/" << idx.texcoord_index << '\n';
                    // Add the indices to the global list of indices we need to pass on to the mesh object.
                    // This goes to the mesh data structure.
                    md->vertex_indices.push_back( Point3i{idx1.vertex_index, idx2.vertex_index, idx3.vertex_index} );
                    md->normal_indices.push_back( Point3i{idx1.normal_index, idx2.normal_index, idx3.normal_index} );
                    md->uvcoord_indices.push_back( Point3i{idx1.texcoord_index, idx2.texcoord_index, idx3.texcoord_index} );
                }
            }

            // Advance over to the next triangle.
            index_offset += fnum;
        }
    }

    cout << "This is the list of indices: \n";

    cout << "   + Vertices [ ";
    std::copy( md->vertex_indices.begin(), md->vertex_indices.end(), std::ostream_iterator< int > ( std::cout, " " ) );
    cout << "]\n";

    cout << "   + Normals [ ";
    std::copy( md->normal_indices.begin(), md->normal_indices.end(), std::ostream_iterator< int > ( std::cout, " " ) );
    cout << "]\n";

    cout << "   + UV coords [ ";
    std::copy( md->uvcoord_indices.begin(), md->uvcoord_indices.end(), std::ostream_iterator< int > ( std::cout, " " ) );
    cout << "]\n";
}


}
#endif  