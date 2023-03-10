#ifndef API_H
#define API_H 1

#include <string>

#include "rt3-base.hpp"
#include "scene.hpp"
#include "Integrator/integrator.hpp"
#include "bvh.hpp"
#include "material.hpp"
#include "Shape/Triangle.hpp"


//=== API Macro definitions

/// Check whether the current state has been intialized.
#define VERIFY_INITIALIZED(func_name)                                        \
	if (curr_state == APIState::Uninitialized) {                               \
		RT3_ERROR(std::string{"API::init() must be called before "} +            \
							std::string{func_name} + std::string{"(). Ignoring."});        \
		return;                                                                  \
	} else /* Empty so that it receives the semicolon palace after this macro. \
					*/

/// Check whether the current state corresponds to setup section.
#define VERIFY_SETUP_BLOCK(func_name)                                        \
	VERIFY_INITIALIZED(func_name)                                              \
	if (curr_state == APIState::WorldBlock) {                                  \
		RT3_ERROR(std::string{"Rendering setup cannot happen inside "} +         \
							std::string{"World definition block; "} +                      \
							std::string{func_name} +                                       \
							std::string{"() not allowed. Ignoring"});                      \
		return;                                                                  \
	} else /* Empty so that it receives the semicolon palace after this macro. \
					*/

/// Check whether the current state corresponds to the world section.
#define VERIFY_WORLD_BLOCK(func_name)                                        \
	VERIFY_INITIALIZED(func_name)                                              \
	if (curr_state == APIState::SetupBlock) {                                  \
		RT3_ERROR(std::string{"Scene description must happen inside "} +         \
							std::string{"World Definition block; "} +                      \
							std::string{func_name} +                                       \
							std::string{"() not allowed. Ignoring"});                      \
		return;                                                                  \
	} else /* Empty so that it receives the semicolon palace after this macro. \
					*/

namespace rt3 {
/// Collection of objects and diretives that control rendering, such as camera,
/// lights, prims.
struct RenderOptions {
	// the Film
	std::string film_type{"image"};  // The only type available.
	ParamSet film_ps;
	/// the Camera
	string camera_type{"perspective"};
	ParamSet camera_ps;
	ParamSet lookat_ps;
	/// the Background
	string bkg_type{"solid"};  // "image", "interpolated"
	ParamSet bkg_ps;
	// std::shared_ptr<Material> curr_material;
	// std::map<std::string, std::shared_ptr<Material>> named_materials;
	Scene curr_scene;
	std::vector<std::shared_ptr<Primitive>> primitives;
	string accelerator{"primlist"};
	size_t bvh_leaf_size{2};
	string bvh_split_method{"middle"};
	std::shared_ptr<Integrator> integrator;
};

/// Collection of data related to a Graphics state, such as current material,
/// lib of material, etc.
struct GraphicsState {
	std::shared_ptr<Material> curr_material;  //!< Current material that globally affects all objects.
	bool flip_normals{false};              //!< When true, we flip the normals
	std::map<std::string, std::shared_ptr<Material>> named_materials;      //!< Library of materials.
	bool mats_lib_cloned{false};           //!< We only actually clone the library if a new material is added to it.
};

/// Static class that manages the render process
class API {
 public:
	/// Defines the current state the API may be at a given time
	enum class APIState {
		Uninitialized,  //!< Initial state, before parsing.
		SetupBlock,     //!< Parsing the render setup section.
		WorldBlock
	};  //!< Parsing the world's information section.

	/// Stores the running options collect in main().
	static RunningOptions curr_run_opt;

	/// Current API state
	static APIState curr_state;
	/*
	 * The unique pointer below is useful to support various calls to
	 * init()-run()-clean-up(), in case we want to process several input files in
	 * a single run of the system.
	 */
	/// Unique infrastructure to render a scene (camera, integrator, etc.).
	static std::unique_ptr<RenderOptions> render_opt;
	// /// The current GraphicsState
	static GraphicsState curr_GS;
	static Transform curr_TM;

	static std::stack<GraphicsState> saved_GS;
	static std::stack<Transform> saved_TM;
	
	static Dictionary<std::string, std::shared_ptr<Transform > > transformation_cache;
	static Dictionary< string,Transform > named_coord_system;


	// [NOT NECESSARY IN THIS PROJECT]
	// /// Pointer to the scene. We keep it as parte of the API because it may be
	// reused later [1] Create the integrator. static unique_ptr< Scene >
	// the_scene;

	//== Private methods
 private:
	// === Helper functions.
	///
	static Film* make_film(const string& name, const ParamSet& ps);
	static std::shared_ptr<Primitive> make_object(const std::string &type, const ParamSet &object_ps);

	/// @brief Read .obj file and puts its values in a TriangleMesh
	/// @param filename Name of the .obj file relative to the .xml file that imported it
	/// @param mesh Shared pointer to a TriagleMesh
	static std::shared_ptr<Primitive> read_obj_file(std::string filename, std::shared_ptr<TriangleMesh> mesh, std::shared_ptr<Material> material, bool rvo, bool cn, bool fn, std::unique_ptr<rt3::RenderOptions> &opt);
	static std::shared_ptr<GeometricPrimitive> create_sphere(const ParamSet &object_ps, std::unique_ptr<rt3::RenderOptions> &opt);
	static std::shared_ptr<Primitive> create_triangle_mesh(const ParamSet &object_ps, std::unique_ptr<rt3::RenderOptions> &opt);

 public:
	//=== API function begins here.
	static void init_engine(const RunningOptions&);
	static void run(void);
	static void clean_up(void);
	static void reset_engine(void);

	static void accelerator(const ParamSet &object_ps);
	static void integrator(const ParamSet &object_ps);
	static void film(const ParamSet& ps);
	static void camera(const ParamSet& ps);
	static void look_at(const ParamSet& ps);
	static void background(const ParamSet& ps);
	static void material(const ParamSet& ps);

	/// @brief Create and store named material if not present. Set it as the current material. 
	/// @param ps parameters
	static void make_named_material(const ParamSet& ps);

	/// @brief Set cited named_material as the current material
	/// @param ps parameters
	static void named_material(const ParamSet& ps);

	/// @brief Add translation to all objects of the current scene
	/// @param ps parameters
	static void translate(const ParamSet& ps);

	/// @brief Add rotation to all objects of the current scene
	/// @param ps parameters
	static void rotate(const ParamSet& ps);

	/// @brief Resets transformation matrix to the identity matrix
	/// @param ps parameters
	static void identity();

	/// @brief Scale all objects of the current scene
	/// @param ps parameters
	static void scale(const ParamSet& ps);

	
	
	static void light_source(const ParamSet& ps);
	static void world_begin(void);
	static void object(const ParamSet& ps);
	static void world_end(void);

	
};
}  // namespace rt3

#endif  // API_H
