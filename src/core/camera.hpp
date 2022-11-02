#ifndef CAMERA_H
#define CAMERA_H

#include "rt3-base.hpp"
#include "rt3.hpp"
#include "ray.hpp"
#include <memory>
#include <vector>

namespace rt3 {
    class Camera {
        public:
            Camera(Film* film, Point3f& look_from, Point3f& look_at, Vector3f& up, std::vector<real_type>& screen_space);
            virtual ~Camera() {};
            Point2f to_screen_coord(const Point2f& pixel_coord) const {
                real_type u = left + (real_type)((right-left)*(pixel_coord.x() + 0.5))/(real_type) film->m_full_resolution.x();
                real_type v = bottom + (real_type)((top-bottom)*(pixel_coord.y() + 0.5))/(real_type)film->m_full_resolution.y();
                return Point2f{u, v};
            }
            Ray generate_ray(const Point2f& pixel_coord) const {
                Point2f screen_coord = to_screen_coord(pixel_coord);
                return Ray(calculate_ray_origin(screen_coord), calculate_ray_direction(screen_coord));
            };

            Point3f virtual calculate_ray_origin(const Point2f& screen_coord) const = 0;
            Vector3f virtual calculate_ray_direction(const Point2f& screen_coord) const = 0;

            std::unique_ptr<Film> film;
        protected:
            Point3f look_from;
            Point3f look_at;
            Vector3f up;
            //
            real_type left;
            real_type right;
            real_type bottom;
            real_type top;
            //
            Vector3f w;
            Vector3f u;
            Vector3f v;
            Vector3f e;
    };

    class PerspectiveCamera : public Camera {

        public:
            PerspectiveCamera(Point3f& look_from, Point3f& look_at, Vector3f& up, std::vector<real_type>& screen_space, Film* film);
            ~PerspectiveCamera(){};
            Point3f calculate_ray_origin(const Point2f& screen_coord) const {
                return e;
            };

            Vector3f calculate_ray_direction(const Point2f& screen_coord) const {
                return w + screen_coord.x() * u + screen_coord.y() * v;
            };

            
        private:
            real_type fd;
    };

    class OrthographicCamera : public Camera {

        public:
            OrthographicCamera(Point3f& look_from, Point3f& look_at, Vector3f& up, std::vector<real_type>& screen_space, Film* film):
            Camera(film, look_from, look_at, up, screen_space)
            {
                fd=1;
            }
            ~OrthographicCamera(){};
            Point3f calculate_ray_origin(const Point2f& screen_coord) const {
                return e + screen_coord.x() * u + screen_coord.y() * v;
            };
            
            Vector3f calculate_ray_direction(const Point2f& screen_coord) const {
                return w;
            };

        private:
            real_type fd;
    };

std::shared_ptr<Camera> create_perspective_camera(const ParamSet &camera_ps, const ParamSet &lookat_ps, Film* film);
std::shared_ptr<Camera> create_orthographic_camera(const ParamSet &camera_ps, const ParamSet &lookat_ps, Film* film);

}
#endif // CAMERA_H
