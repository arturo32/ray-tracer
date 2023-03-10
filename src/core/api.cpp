#include "api.hpp"
#include "Background/background.hpp"
#include "camera.hpp"
#include "Shape/Sphere.hpp"
#include "scene.hpp"
#include "Transform.hpp"
#include <fstream>

#include <chrono>
#include <memory>

#define TINYOBJLOADER_IMPLEMENTATION
#include "../ext/tiny_obj_loader.h"


namespace rt3 {

void normalize_spectrum(Vector3f& a){
	if(a.x() > 1.0 || a.y() > 1.0 || a.z() > 1.0) {
		a /= 255.0;
		a.clamp(0.0, 1.0);
	}
} 

//=== API's static members declaration and initialization.
API::APIState API::curr_state = APIState::Uninitialized;
RunningOptions API::curr_run_opt;
std::unique_ptr<RenderOptions> API::render_opt;
GraphicsState API::curr_GS;
Transform API::curr_TM;
std::stack<GraphicsState> API::saved_GS;
std::stack<Transform> API::saved_TM;
Dictionary<std::string, std::shared_ptr<Transform > > API::transformation_cache;
Dictionary< string,Transform > API::named_coord_system;

/* --------------------------------------------------------------------------------
* The matrix lookup is unique map (hash table) of transformation matrices.
* Every new transformation that is created in `API::transform()`
* should be stored in this map.
* So, whenever we generate a transformation matrix (either defined
* directly in the scene file or as a result of composition of other
* matrices), we do the following: we look it up in the dictionary;
* if it's there, we return the shared pointer stored in the map;
* if it's NOT there yet, we store it in the dictionary and return
* the shared pointer stored in the map.
* -------------------------------------------------------------------------------- */
/// The Dictionary of instantiated transformation matrix.
// static Dictionary< string, shared_ptr<const  Transform > > transformation_cache;


// static std::stack< GraphicsState > saved_GS;
// static std::stack< GraphicsState > saved_GS;


// GraphicsState API::curr_GS;

// THESE FUNCTIONS ARE NEEDED ONLY IN THIS SOURCE FILE (NO HEADER NECESSARY)
// ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ

Film *API::make_film(const std::string &type, const ParamSet &ps) {
	std::cout << ">>> Inside API::make_film()\n";
	Film *film{nullptr};
	film = create_film(ps);

	// Return the newly created film.
	return film;
}

std::shared_ptr<Primitive> prepare_obj(std::vector<std::shared_ptr<Primitive>> primitives,
									   std::string accelerator,
									   std::string bvh_split_method,
									   size_t bvh_leaf_size) {

	std::cout << "inside prepare_obj" << std::endl;
	std::cout << "primitives size = " << primitives.size() << std::endl;
	if (accelerator == "bvh") {
		return make_shared<BVH>(primitives, 0, primitives.size(), 0, bvh_split_method, bvh_leaf_size);  
	} else if (accelerator == "primlist") {
		return make_shared<PrimList>(primitives);  
	} else {
		// Default accelerator
		return make_shared<PrimList>(primitives);  
	}
}

std::unique_ptr<Background> make_background(const std::string &type, const ParamSet &ps) {
	std::cout << ">>> Inside API::background()\n";
	std::unique_ptr<Background> bkg{nullptr};
	if(type == "colors") {
		bkg = create_color_background(ps);
	} else if(type == "texture") {  
		bkg = create_texture_background(ps);
	} else {
		bkg = create_color_background(ps);
	}
	// Return the newly created background.
	return bkg;
}


std::shared_ptr<Camera> make_camera(const std::string &type, const ParamSet &camera_ps, const ParamSet &lookat_ps, Film* film) {
	std::cout << ">>> Inside API::camera()\n";
	std::shared_ptr<Camera> cmr{nullptr};

	if(type == "orthographic") {
		cmr = create_orthographic_camera(camera_ps, lookat_ps, film);
	} else if (type == "perspective") {
		cmr = create_perspective_camera(camera_ps, lookat_ps, film);
	} else {
		RT3_ERROR("API::clean_up() called before engine initialization.");
	}

	// Return the newly created background.
	return cmr;
}

std::shared_ptr<Primitive> API::make_object(const std::string &type, const ParamSet &object_ps) {
	std::shared_ptr<Primitive> object{nullptr};
	
	
	string new_matrix_string = curr_TM.GetMatrix().Print();
	transformation_cache.insert(
			std::pair<string, shared_ptr<Transform >>(new_matrix_string, std::make_shared<Transform>(curr_TM)));
	
	if(type == "sphere") {		
		object = create_sphere(object_ps, render_opt);
	}
	else if(type == "trianglemesh") {
		object = create_triangle_mesh(object_ps, render_opt);
	}
	else {
		RT3_ERROR("Type of object not valid.");
	}
	return object;
}

std::shared_ptr<GeometricPrimitive> API::create_sphere(const ParamSet &object_ps, std::unique_ptr<RenderOptions> &opt) {
	real_type radius = retrieve(object_ps, "radius", real_type{0});
	Point3f center = retrieve(object_ps, "center", Point3f{0.0,0.0,0.0});
	
	const shared_ptr<Transform > o2w = transformation_cache.at(curr_TM.GetMatrix().Print());

	cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
	cout << o2w->Print() << endl;
	cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
	
	std::shared_ptr<Sphere> sphere = make_shared<Sphere>(false, o2w, center, radius);

	std::string name = retrieve(object_ps, "material", std::string{""});

	std::shared_ptr<Material> materialSphere;
	if (name == "") {
		materialSphere = curr_GS.curr_material;
	} else {
		if (curr_GS.named_materials.find(name) == curr_GS.named_materials.end()){
			RT3_ERROR("Material of name '" + name + "' not found!");
		}
		materialSphere = curr_GS.named_materials[name];
	}
	return make_shared<GeometricPrimitive>(materialSphere, sphere);
}

std::shared_ptr<Primitive> API::read_obj_file(std::string filename, 
											 std::shared_ptr<TriangleMesh> mesh,
											 std::shared_ptr<Material> material,
											 bool rvo, bool cn, bool fn,
											 std::unique_ptr<rt3::RenderOptions> &opt) {
	tinyobj::ObjReaderConfig reader_config;
	tinyobj::ObjReader reader;
	if (!reader.ParseFromFile(filename, reader_config)) {
		if (!reader.Error().empty()) {
			std::cerr << "TinyObjReader: " << reader.Error();
		}
		exit(1);
	}

	if (!reader.Warning().empty()) {
		std::cout << "TinyObjReader: " << reader.Warning();
	}

	auto& attrib = reader.GetAttrib();
	auto& shapes = reader.GetShapes();

	std::cout << "-- SUMMARY of the OBJ file --\n";
	std::cout << "# of vertices  : " << (attrib.vertices.size()  / 3) << std::endl;
	std::cout << "# of normals   : " << (attrib.normals.size()   / 3) << std::endl;
	std::cout << "# of texcoords : " << (attrib.texcoords.size() / 2) << std::endl;
	std::cout << "# of shapes    : " << shapes.size()                 << std::endl;
	std::cout << "-----------------------------\n";

	// Retrieve the complete list of vertices.
	auto n_vertices{ attrib.vertices.size() / 3 };
	for ( auto idx_v{0u} ; idx_v < n_vertices; idx_v++)	{
		mesh->vertices.push_back(
			Point3f{
				attrib.vertices[ 3 * idx_v + 0 ],
				attrib.vertices[ 3 * idx_v + 1 ],
				attrib.vertices[ 3 * idx_v + 2 ]
			}
		);
	}
	// Read the normals
	// TODO: flip normals if requested.
	real_type flip = ( fn ) ? -1 : 1 ;
	auto n_normals{ attrib.normals.size()/3 };

	// Do we need to compute the normals? Yes only if the user requeste or there are no normals in the file.
	if ( cn == true or n_normals == 0)
	{
	   // TODO: COmpute normals here.
	   // compute_normals();
	}
	else {// Read normals from file. This corresponds to the entire 'for' below.
		// Traverse the normals read from the OBJ file.
		for ( auto idx_n{0u} ; idx_n < n_normals; idx_n++) {
			// Store the normal.
			mesh->normals.push_back(Normal3f{ 
											attrib.normals[ 3 * idx_n + 0 ] * flip,
											attrib.normals[ 3 * idx_n + 1 ] * flip,
											attrib.normals[ 3 * idx_n + 2 ] * flip
											}
									);
		}
	}
	// Read the complete list of texture coordinates.
	auto n_texcoords{ attrib.texcoords.size()/2 };
	for ( auto idx_tc{0u} ; idx_tc < n_texcoords; idx_tc++)
	{
		// Store the texture coords.
		mesh->uvcoords.push_back( 
			Point2f{
				attrib.texcoords[ 2 * idx_tc + 0 ],
				attrib.texcoords[ 2 * idx_tc + 1 ]
			}
		);
	}

	// Read mesh connectivity and store it as lists of indices to the real data.
	auto n_shapes{ shapes.size() };
	mesh->n_triangles = 0; // We must reset this here.
	// In case the OBJ file has the triangles organized in several shapes or groups, we
	// ignore this and store all triangles as a single mesh dataset.
	// This is why we need to reset the triangle count here.
	for ( auto idx_s{0u} ; idx_s < n_shapes; idx_s++)
	{
		// For each face print out the indices lists.
		size_t index_offset = 0;
		// # of triangles for this "shape" (group).
		// NOTE that we are accumulate the number of triangles coming from the shapes present in the OBJ file.
		mesh->n_triangles += shapes[idx_s].mesh.num_face_vertices.size();
		for ( auto idx_f{0} ; idx_f < mesh->n_triangles; idx_f++) {
			// Number of vertices per face (always 3, in our case)
			size_t fnum = shapes[idx_s].mesh.num_face_vertices[idx_f];
			if ( rvo == true ) {
				std::cout << "Reverse order\n";
				// For each vertex in the face print the corresponding indices
				for (int v = fnum-1; v >= 0 ; v--)
				{
					tinyobj::index_t idx = shapes[idx_s].mesh.indices[index_offset + v];
					// cout << "    face[" << idx_f << "].v[" << v << "].indices = "
					//     << idx.vertex_index << "/" << idx.normal_index << "/" << idx.texcoord_index << '\n';
					// Add the indices to the global list of indices we need to pass on to the mesh object.
					mesh->vertex_indices.push_back( idx.vertex_index );
					mesh->normal_indices.push_back( idx.normal_index );
					mesh->uvcoord_indices.push_back( idx.texcoord_index );
				}
			}
			else { // Keep the original vertex order
				// For each vertex in the face get the corresponding indices
				for (size_t v = 0; v < fnum; v++)
				{
					tinyobj::index_t idx = shapes[idx_s].mesh.indices[index_offset + v];
					mesh->vertex_indices.push_back( idx.vertex_index );
					mesh->normal_indices.push_back( idx.normal_index );
					mesh->uvcoord_indices.push_back( idx.texcoord_index );
				}
			}
			// Advance over to the next triangle.
			index_offset += fnum;
		}
	}
	
	vector<shared_ptr<Primitive>> primitives;
	for ( int i = 0 ; i < mesh->n_triangles ; ++i ) {
		// TODO: esse fn tá errado!!!

		const shared_ptr<Transform > o2w = transformation_cache.at(curr_TM.GetMatrix().Print());
		shared_ptr<Triangle> triangle = std::make_shared<Triangle>(false, o2w, mesh, i, fn );
		primitives.push_back(std::make_shared<GeometricPrimitive>(material, triangle));
	}  
	if (opt->accelerator == "bvh") {
		return make_shared<BVH>(primitives, 0, primitives.size(), 0, opt->bvh_split_method, opt->bvh_leaf_size);  
	} else if (opt->accelerator == "primlist") {
		return make_shared<PrimList>(primitives);  
	} else {
		// Default accelerator
		return make_shared<PrimList>(primitives);  
	}
}

std::shared_ptr<Primitive> API::create_triangle_mesh(const ParamSet &object_ps, std::unique_ptr<rt3::RenderOptions> &opt) {
	std::string name = retrieve(object_ps, "material", std::string{""});
	std::shared_ptr<Material> materialMesh;
	if (name == "") {
		materialMesh = curr_GS.curr_material;
		std::cout << curr_GS.curr_material << "=====================================================================" <<std::endl;
	} else {
		if (curr_GS.named_materials.find(name) == curr_GS.named_materials.end()){
			RT3_ERROR("Material of name '" + name + "' not found!");
		}
		materialMesh = curr_GS.named_materials[name];
	}
	
	std::shared_ptr<TriangleMesh> mesh = make_shared<TriangleMesh>();
	std::string filename = retrieve(object_ps, "filename", std::string{""});

	std::string backface_cull = retrieve(object_ps, "backface_cull", std::string{"false"});
	std::string reverse_vertex_order = retrieve(object_ps, "reverse_vertex_order", std::string{"false"});
	std::string compute_normals = retrieve(object_ps, "compute_normals", std::string{"false"});
	
	if(filename != "") {
		std::string curr_filename = curr_run_opt.filename.c_str();
		
		// Search included file relative to the directory of the current file
		std::size_t last_index_before_file = curr_filename.find_last_of("/");
		if(last_index_before_file != string::npos) {
			filename = curr_filename.substr(0, last_index_before_file) + "/" + filename;
		}

		return read_obj_file(filename, mesh, materialMesh, reverse_vertex_order == "true",
							 compute_normals == "true", backface_cull == "true", opt);
	}
	else {
		mesh->n_triangles = retrieve(object_ps, "ntriangles", int{});
		mesh->vertices = retrieve(object_ps, "vertices", vector<Point3f>{});
		mesh->normals = retrieve(object_ps, "normals", std::vector<Normal3f>{});
		mesh->uvcoords = retrieve(object_ps, "uv", std::vector<Point2f>{});
		
		std::vector<int> indices = retrieve(object_ps, "indices", std::vector<int>{});
		
		mesh->vertex_indices = indices;
		mesh->normal_indices = indices;
		mesh->uvcoord_indices = indices;

		vector<shared_ptr<Primitive>> primitives;
		for ( int i = 0 ; i < mesh->n_triangles ; ++i ) {
			const shared_ptr<Transform > o2w = transformation_cache.at(curr_TM.GetMatrix().Print());
			
			shared_ptr<Shape> triangle = std::make_shared<Triangle>(false, o2w, mesh, i, backface_cull == "true" );
			primitives.push_back(std::make_shared<GeometricPrimitive>(materialMesh, triangle));
		}  
		if (opt->accelerator == "bvh") {
			return make_shared<BVH>(primitives, 0, primitives.size(), 0, opt->bvh_split_method, opt->bvh_leaf_size);  
		} else if (opt->accelerator == "primlist") {
			return make_shared<PrimList>(primitives);  
		} else {
			// Default accelerator
			return make_shared<PrimList>(primitives);  
		}
	}
}

// ˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆ
// END OF THE AUXILIARY FUNCTIONS
// =========================================================================

//=== API's public methods implementation
void API::init_engine(const RunningOptions &opt) {
	// Save running option sent from the main().
	curr_run_opt = opt;
	// Check current machine state.
	if (curr_state != APIState::Uninitialized)
		RT3_ERROR("API::init_engine() has already been called! ");
	// Set proper machine state
	curr_state = APIState::SetupBlock;
	// Preprare render infrastructure for a new scene.
	render_opt = std::make_unique<RenderOptions>();

	curr_TM = Transform();
	Transform new_TM = Transform(curr_TM.GetMatrix(), curr_TM.GetInverseMatrix());
	string new_matrix_string = new_TM.GetMatrix().Print();
	transformation_cache.insert(
		std::pair<string, std::shared_ptr<Transform >>(new_matrix_string, std::make_shared<Transform>(new_TM)));

	


	// Create a new initial GS
	// curr_GS = GraphicsState();
	RT3_MESSAGE("[1] Rendering engine initiated.\n");
}

void API::clean_up(void) {
	// Check for correct machine state
	if (curr_state == APIState::Uninitialized)
		RT3_ERROR("API::clean_up() called before engine initialization.");
	else if (curr_state == APIState::WorldBlock)
		RT3_ERROR("API::clean_up() called inside world definition section.");
	curr_state = APIState::Uninitialized;

	RT3_MESSAGE("[4] Rendering engine clean up concluded. Shutting down...\n");
}

void API::run(void) {
	// Try to load and parse the scene from a file.
	RT3_MESSAGE("[2] Beginning scene file parsing...\n");
	// Recall that the file name comes from the running option struct.
	parse(curr_run_opt.filename.c_str());
}

void API::world_begin(void) {
	VERIFY_SETUP_BLOCK("API::world_begin");  // check for correct machine state.
	curr_state = APIState::WorldBlock;       // correct machine state.
	render_opt->curr_scene = Scene();
}

void API::world_end(void) {
	VERIFY_WORLD_BLOCK("API::world_end");
	// The scene has been properly set up and the scene has
	// already been parsed. It's time to render the scene.

	render_opt->curr_scene.background = make_background(render_opt->bkg_type, render_opt->bkg_ps);
	render_opt->curr_scene.agg = prepare_obj(render_opt->primitives,
											 render_opt->accelerator,
											 render_opt->bvh_split_method,
											 render_opt->bvh_leaf_size
	);
	std::cout << "bounds da cena completa" << std::endl;
	std::cout << render_opt->curr_scene.agg->world_bounds() << std::endl;
	// Same with the film, that later on will belong to a camera object.
	Film* the_film{make_film(render_opt->film_type, render_opt->film_ps)};

	render_opt->integrator->camera = make_camera(render_opt->camera_type,
	render_opt->camera_ps, render_opt->lookat_ps, the_film);

	// Run only if we got film and background.
	if (render_opt->integrator and render_opt->curr_scene.background) {
		RT3_MESSAGE("    Parsing scene successfuly done!\n");
		RT3_MESSAGE("[2] Starting ray tracing progress.\n");

		// Structure biding, c++17.
		auto res = the_film->get_resolution();
		size_t w = res[0];
		size_t h = res[1];
		RT3_MESSAGE("    Image dimensions in pixels (W x H): " + std::to_string(w) +
								" x " + std::to_string(h) + ".\n");
		RT3_MESSAGE(
				"    Ray tracing is usually a slow process, please be patient: \n");

		//================================================================================
		auto start = std::chrono::steady_clock::now();
		render_opt->integrator->render(render_opt->curr_scene);  // This is the ray tracer's  main loop.
		auto end = std::chrono::steady_clock::now();
		//================================================================================
		auto diff = end - start;  // Store the time difference between start and end
		// Seconds
		auto diff_sec = std::chrono::duration_cast<std::chrono::seconds>(diff);
		RT3_MESSAGE("    Time elapsed: " + std::to_string(diff_sec.count()) +
								" seconds (" +
								std::to_string(
										std::chrono::duration<double, std::milli>(diff).count()) +
								" ms) \n");
	}
	// [4] Basic clean up
	curr_state = APIState::SetupBlock;  // correct machine state.
	reset_engine();
}

/// This api function is called when we need to re-render the *same* scene (i.e.
/// objects, lights, materials, etc) , maybe with different integrator, and
/// camera setup. Hard reset on the engine. User needs to setup all entities,
/// such as camera, integrator, accelerator, etc.
void API::reset_engine(void) {
	// curr_GS = GraphicsState();
	// This will delete all information on integrator, cameras, filters,
	// acceleration structures, etc., that has been set previously.
	render_opt.reset(new RenderOptions);
}

void API::object(const ParamSet &ps) {
	std::cout << ">>> Inside API::object()\n";
	VERIFY_WORLD_BLOCK("API::object");
	std::string type = retrieve(ps, "type", string{"unknown"});
	render_opt->primitives.push_back(make_object(type, ps));
}

void API::translate(const ParamSet& ps) {
	std::cout << ">>> Inside API::translate()\n";
	VERIFY_WORLD_BLOCK("API::translate");
	Vector3f translation = retrieve(ps, "value", Vector3f{0, 0, 0});
	
	std::cout << "BEFORE: " << std::endl; 
	std::cout << curr_TM.Print() << std::endl;

	Transform translate = Transform::Translate(translation);
	std::cout << "TRANSLATE: " << std::endl; 
	std::cout << translate.Print() << std::endl;
	
	curr_TM = translate * curr_TM;
	
	std::cout << "AFTER: " << std::endl;
	std::cout << curr_TM.Print() << std::endl;
	// cout << "INVERSA:" << endl;
	// cout << curr_TM.GetInverseMatrix().Print() << std::endl;
	// cout << "SUPOSTA IDENTIDADE:" << endl;
	// Transform inv = Transform(curr_TM.GetInverseMatrix());
	// Transform inden = curr_TM*inv;
	// cout << inden.Print() << endl;


	// Ray testRay = Ray(Point3f(0, 0, 0), Vector3f(2, 4, -5));
	// Transform testT = Transform();
	// Ray resultTestRay = testT.GetMatrix() * testRay;
	// cout << resultTestRay.direction << "-----------------------------------------------------------------------------------------------------" <<  endl;
	
	// cout << "TESTE MULTIPLICAO/TRANSLAÇÃO: " << endl;
	// Transform testeT2  = Transform::Translate(Vector3f(1, 1, 1));
	// Ray resultTestRay2 = testeT2.GetMatrix() * testRay;
	// cout << resultTestRay2.direction << endl;

}

void API::rotate(const ParamSet& ps) {
	std::cout << ">>> Inside API::translate()\n";
	VERIFY_WORLD_BLOCK("API::translate");
	Vector3f axis = retrieve(ps, "axis", Vector3f{1, 1, 1});
	real_type angle = retrieve(ps, "angle", real_type{0});

	std::cout << "BEFORE: " << std::endl; 
	std::cout << curr_TM.Print() << std::endl;

	Transform rotation = Transform::Rotate(axis, angle);
	std::cout << "ROTATION: " << std::endl; 
	std::cout << rotation.Print() << std::endl;
	
	curr_TM = rotation * curr_TM;
	
	std::cout << "AFTER: " << std::endl;
	std::cout << curr_TM.Print() << std::endl;
}

void API::scale(const ParamSet& ps) {
	std::cout << ">>> Inside API::scale()\n";
	VERIFY_WORLD_BLOCK("API::scale");
	Vector3f scaling_factors = retrieve(ps, "value", Vector3f{1, 1, 1});
	
	std::cout << "BEFORE: " << std::endl; 
	std::cout << curr_TM.Print() << std::endl;

	Transform scale = Transform::Scale(scaling_factors.x(), scaling_factors.y(), scaling_factors.z());
	std::cout << "SCALE: " << std::endl; 
	std::cout << scale.Print() << std::endl;
	
	curr_TM = scale * curr_TM;
	
	std::cout << "AFTER: " << std::endl;
	std::cout << curr_TM.Print() << std::endl;
}

void API::identity() {
	std::cout << ">>> Inside API::identity()\n";
	VERIFY_WORLD_BLOCK("API::identity");
	curr_TM = Transform();
}


void API::background(const ParamSet &ps) {
	std::cout << ">>> Inside API::background()\n";
	VERIFY_WORLD_BLOCK("API::background");

	// retrieve type from ps.
	std::string type = retrieve(ps, "type", string{"unknown"});
	render_opt->bkg_type = type;
	// Store current background object.
	render_opt->bkg_ps = ps;
}

void API::material(const ParamSet &ps) {
	std::cout << ">>> Inside API::material()\n";
	VERIFY_WORLD_BLOCK("API::material");
	// retrieve type from ps.
	std::string type = retrieve(ps, "type", string{"unknown"});

	if (type == "flat") {
			// retrieve color from ps.
			ColorXYZ c = retrieve(ps, "color", ColorXYZ{0,0,0});
			// check interval of values and convert if needed
			if(c.x() <= 1.0 && c.y() <= 1.0 && c.z() <= 1.0) {
				c *= 255.0;
				c.clamp(0.0, 255.0);
			}
			curr_GS.curr_material = std::make_shared<FlatMaterial>(c);
	} else if (type == "blinn") {
			Vector3f a = retrieve(ps, "ambient", Vector3f{0,0,0});
			normalize_spectrum(a);
			Vector3f d = retrieve(ps, "diffuse", Vector3f{0,0,0});
			normalize_spectrum(d);
			Vector3f s = retrieve(ps, "specular", Vector3f{0,0,0});
			normalize_spectrum(s);
			Vector3f m = retrieve(ps, "mirror", Vector3f{0,0,0});
			normalize_spectrum(m);
			real_type g = retrieve(ps, "glossiness", real_type{0});
			curr_GS.curr_material = std::make_shared<BlinnPhongMaterial>(a,d,s,m,g);
	}

}



void API::make_named_material(const ParamSet &ps) {
	std::cout << ">>> Inside API::make_named_material()\n";
	VERIFY_WORLD_BLOCK("API::make_named_material");
	// retrieve type from ps.
	std::string type = retrieve(ps, "type", string{"unknown"});
	std::string name = retrieve(ps, "name", string{"unknown"});
	
	if (type == "flat") {
		// retrieve color from ps.
		ColorXYZ c = retrieve(ps, "color", ColorXYZ{0,0,0});
		// check interval of values and convert if needed
		if(c.x() <= 1.0 && c.y() <= 1.0 && c.z() <= 1.0) {
			c *= 255.0;
			c.clamp(0.0, 255.0);
		}
		std::cout << "color: " << c << std::endl;
		curr_GS.named_materials[name] = std::make_shared<FlatMaterial>(c);
	} else if (type == "blinn") {
		// check interval of values and convert if needed?
		Vector3f a = retrieve(ps, "ambient", Vector3f{0,0,0});
		normalize_spectrum(a);
		Vector3f d = retrieve(ps, "diffuse", Vector3f{0,0,0});
		normalize_spectrum(d);
		Vector3f s = retrieve(ps, "specular", Vector3f{0,0,0});
		normalize_spectrum(s);
		Vector3f m = retrieve(ps, "mirror", Vector3f{0,0,0});
		normalize_spectrum(m);
		real_type g = retrieve(ps, "glossiness", real_type{0});
		curr_GS.named_materials[name] = std::make_shared<BlinnPhongMaterial>(a,d,s,m,g);
	}
	
}

void API::named_material(const ParamSet &ps) {
	std::cout << ">>> Inside API::named_material()\n";
	VERIFY_WORLD_BLOCK("API::named_material");

	std::string name = retrieve(ps, "name", string{"unknown"});
	if(curr_GS.named_materials.find(name) == curr_GS.named_materials.end()) { 
		RT3_ERROR("Material of name '" + name + "' not found!");
	}

	curr_GS.curr_material = curr_GS.named_materials[name];
}

void API::light_source(const ParamSet &ps) {
	std::cout << ">>> Inside API::light_source()\n";
	VERIFY_WORLD_BLOCK("API::light_source");
	// retrieve type from ps.
	std::string type = retrieve(ps, "type", string{"unknown"});
	Vector3f intensity = retrieve(ps, "L", Vector3f{1,1,1});
	Vector3f scale = retrieve(ps, "scale", Vector3f{1,1,1});

	string attenuate_s = retrieve(ps, "attenuate", string{"false"});
	bool attenuate = (attenuate_s == "true");
	
	real_type kc = retrieve(ps, "kc", real_type{1});
	real_type kl = retrieve(ps, "kl", real_type{1});
	real_type kq = retrieve(ps, "kq", real_type{1});

	std::shared_ptr<Light> light;
	if (type == "ambient") {
		render_opt->curr_scene.ambientLight = std::make_shared<AmbientLight>(scale*intensity, kc, kl, kq, attenuate);
		return;
	} else if (type == "directional") {
		Point3f from = retrieve(ps, "from", Point3f{0,0,0});
		Point3f to = retrieve(ps, "to", Point3f{0,0,0});
		light = std::make_shared<DirectionalLight>(from, to, scale*intensity, kc, kl, kq, attenuate);
	} else if (type == "point") {
		Point3f from = retrieve(ps, "from", Point3f{0,0,0});
		light = std::make_shared<PointLight>(from, scale*intensity, kc, kl, kq, attenuate);
	} else if (type == "spot") {
		Point3f from = retrieve(ps, "from", Point3f{0,0,0});
		Point3f to = retrieve(ps, "to", Point3f{0,0,0});
		real_type cutoff = retrieve(ps, "cutoff", real_type{0});
		real_type falloff = retrieve(ps, "falloff", real_type{0});
		light = std::make_shared<SpotLight>(from, to, scale*intensity, Radians(cutoff), Radians(falloff), kc, kl, kq, attenuate);
	}
	render_opt->curr_scene.add_light(light);
}

void API::film(const ParamSet &ps) {
	std::cout << ">>> Inside API::film()\n";
	VERIFY_SETUP_BLOCK("API::film");

	// retrieve type from ps.
	std::string type = retrieve(ps, "type", string{"unknown"});
	render_opt->film_type = type;
	render_opt->film_ps = ps;
}

void API::camera(const ParamSet &ps) {
	std::cout << ">>> Inside API::camera()\n";
	VERIFY_SETUP_BLOCK("API::camera");

	// retrieve type from ps.
	std::string type = retrieve(ps, "type", string{"unknown"});
	render_opt->camera_type = type;
	// Store current camera object.

	render_opt->camera_ps = ps;
}

void API::look_at(const ParamSet &ps) {
	std::cout << ">>> Inside API::look_at()\n";
	VERIFY_SETUP_BLOCK("API::look_at");
	// retrieve type from ps.
	std::string type = retrieve(ps, "type", string{"unknown"});
	render_opt->lookat_ps = ps;

}

void API::integrator(const ParamSet &object_ps) {
	std::shared_ptr<Integrator> object{nullptr};
	std::string type = retrieve(object_ps, "type", string{"unknown"});
	if(type == "flat") {
		object = make_shared<FlatIntegrator>();
	} else if(type == "depth_map") {
		real_type zmin = retrieve(object_ps, "zmin", real_type{0.1});
		real_type zmax = retrieve(object_ps, "zmax", real_type{0.9});
		Spectrum near_color = retrieve(object_ps, "near_color", Spectrum{50,50,50});
		Spectrum far_color = retrieve(object_ps, "far_color", Spectrum{220,220,220});
		object = make_shared<DepthIntegrator>(zmin, zmax, near_color, far_color);
	} else if (type == "normal_map") {
		object = make_shared<NormalMapIntegrator>(); 
	} else if (type == "blinn_phong") {
		uint max_depth = retrieve(object_ps, "depth", uint{1});
		object = make_shared<BlinnPhongIntegrator>(max_depth);
	} else {
		RT3_ERROR("Type of object not valid.");
	}
	render_opt->integrator = object;
}

void API::accelerator(const ParamSet &object_ps) {
	render_opt->accelerator = retrieve(object_ps, "type", string{"primlist"});
	render_opt->bvh_split_method = retrieve(object_ps, "split_method", string{"middle"});
	render_opt->bvh_leaf_size = retrieve(object_ps, "max_prims_per_node", int{2});
}

}