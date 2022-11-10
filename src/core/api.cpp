#include "api.hpp"
#include "Background/background.hpp"
#include "camera.hpp"
#include "sphere.hpp"
#include "triangule.hpp"
#include "scene.hpp"

#include <chrono>
#include <memory>

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

std::unique_ptr<Background> make_background(const std::string &type, const ParamSet &ps) {
  std::cout << ">>> Inside API::background()\n";
  std::unique_ptr<Background> bkg{nullptr};
  bkg = create_color_background(ps);

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
  if(type == "sphere") {
    object = create_sphere(object_ps, render_opt);
  }
  else if(type == "triangule") {
    Point3f a{-4, 0, 4};
    Point3f b{4, 0, 4};
    Point3f c{4, 0, -4};
    vector<Point3f> p{a, b, c};

    std::shared_ptr<Triangule> tri = make_shared<Triangule>(false, p);
    object = make_shared<GeometricPrimitive>(render_opt->curr_material, tri) ;
  }
   else {
    RT3_ERROR("Type of object not valid.");
  }
  return object;
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
  render_opt->curr_scene.add_object(make_object(type, ps));
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
      render_opt->curr_material = std::make_shared<FlatMaterial>(c);
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
      render_opt->curr_material = std::make_shared<BlinnPhongMaterial>(a,d,s,m,g);
  }

}



void API::named_material(const ParamSet &ps) {
  std::cout << ">>> Inside API::named_material()\n";
  VERIFY_WORLD_BLOCK("API::named_material");
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
    render_opt->named_materials[name] = std::make_shared<FlatMaterial>(c);
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
    render_opt->named_materials[name] = std::make_shared<BlinnPhongMaterial>(a,d,s,m,g);
  }
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

std::shared_ptr<GeometricPrimitive> API::create_sphere(const ParamSet &object_ps, std::unique_ptr<RenderOptions> &opt) {
    real_type radius = retrieve(object_ps, "radius", real_type{0});
    Point3f center = retrieve(object_ps, "center", Point3f{0.0,0.0,0.0});
    std::shared_ptr<Sphere> sphere = make_shared<Sphere>(false, center, radius);

    std::string name = retrieve(object_ps, "material", std::string{""});

    if (name == "") {
        return make_shared<GeometricPrimitive>(opt->curr_material, sphere);
    } else {
        if (opt->named_materials.find(name) == opt->named_materials.end()){
            RT3_ERROR("Material of name '" + name + "' not found!");
        }
        return make_shared<GeometricPrimitive>(opt->named_materials[name], sphere);
    }
}


}