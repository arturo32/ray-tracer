#include "camera.hpp"

namespace rt3 {

	Camera::Camera(Film* film, Point3f& look_from, Point3f& look_at, Vector3f& up, std::vector<real_type>& screen_space):
		film{film}, look_from{look_from}, look_at{look_at}, up{up}, 
		left{screen_space.at(0)}, right{screen_space.at(1)},
		bottom{screen_space.at(2)}, top{screen_space.at(3)} {
			// TODO: usar o crop_window do film
			Vector3f gaze = look_at - look_from;
			gaze.make_unit_vector();
			w = gaze;
			u = cross( up, w );
			u.make_unit_vector();
			v = cross ( w, u );
			v.make_unit_vector();
			e = look_from;
			std::cout << "w: " << w << std::endl
					  << "u: " << u << std::endl
					  << "v: " << v << std::endl
					  << "e: " << e << std::endl;
		}

	PerspectiveCamera::PerspectiveCamera(Point3f& look_from, Point3f& look_at, Vector3f& up, std::vector<real_type>& screen_space, Film* film):
	Camera(film, look_from, look_at, up, screen_space)
	{
		fd=1;
	}

	std::shared_ptr<Camera> create_perspective_camera(const ParamSet &camera_ps, const ParamSet &lookat_ps, Film* film){
		std::shared_ptr<Camera> cmr{nullptr};
		real_type fovy = retrieve(camera_ps, "fovy", real_type{30.0});
		Point3f look_from = retrieve(lookat_ps, "look_from", Point3f{0.0,0.0,0.0});
		Point3f look_at = retrieve(lookat_ps, "look_at", Point3f{0.0,0.0,0.0});
		Vector3f up = retrieve(lookat_ps, "up", Vector3f{0.0,1.0,0.0});
		
		real_type aspect_ratio = (real_type)film->get_resolution().x() / (real_type)film->get_resolution().y();
		std::cout << "calculo do aspect_ratio: " << (real_type)film->get_resolution().x() << " / " << (real_type)film->get_resolution().y() << std::endl;
		std::cout << "valor do aspect_ratio: " << aspect_ratio << std::endl;
		real_type h = tan(Radians(fovy/2.0));
		std::cout << "valor do fovy: " << fovy << std::endl;
		std::cout << "valor do h: " << h << std::endl;
		
		std::vector<real_type> values = std::vector<real_type>{-aspect_ratio*h, aspect_ratio*h, -h, h};
		
		// if (aspect_ratio > 1) {
		//     for (size_t i = 0; i < values.size(); ++i) {
		//         values[i] = values[i] / h;
		//     }
		// } else {
		//     for (size_t i = 0; i < values.size(); ++i) {
		//         values[i] = values[i] / aspect_ratio;
		//     }
		// }

		std::cout << "screen space: " << values[0] << " "
		<< values[1] << " "
		<< values[2] << " "
		<< values[3] << " " << std::endl;

		return make_shared<PerspectiveCamera>(look_from, look_at, up, values, film);
	}

	std::shared_ptr<Camera> create_orthographic_camera(const ParamSet &camera_ps, const ParamSet &lookat_ps, Film* film){
		std::shared_ptr<Camera> cmr{nullptr};
		std::vector<real_type> values = retrieve(camera_ps, "screen_window", std::vector<real_type>{0.0,0.0,0.0,0.0});
		Point3f look_from = retrieve(lookat_ps, "look_from", Point3f{0.0,0.0,0.0});
		Point3f look_at = retrieve(lookat_ps, "look_at", Point3f{0.0,0.0,0.0});
		Vector3f up = retrieve(lookat_ps, "up", Vector3f{0.0,1.0,0.0});
		return make_shared<OrthographicCamera>(look_from, look_at, up, values, film);
	}
}