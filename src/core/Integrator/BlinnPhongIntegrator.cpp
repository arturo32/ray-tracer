#include "integrator.hpp"



namespace rt3 {

ColorXYZ BlinnPhongIntegrator::Li( Ray& ray, Scene& scene, Point2f pixel, uint depth) {
	ColorXYZ BLACK(0,0,0);
	ColorXYZ L(0,0,0);
	Surfel isect = Surfel();
	scene.intersect(ray, &isect);
	if (!isect.hit) {
		if(scene.background->mapping_type == Background::mapping_t::screen) {
			return scene.background->sampleXYZ(pixel);
		} else {
			return scene.background->sampleXYZ(ray);
		}
	} else {
		// BlinnPhongMaterial *fm = dynamic_cast<BlinnPhongMaterial*>(isect.primitive->get_material().get());
		BlinnPhongMaterial *fm = (BlinnPhongMaterial*)(isect.primitive->get_material().get());
		if (fm == nullptr) {
			std::cerr << "Material não encontrado!" << std::endl;
			return BLACK;
		}
		Vector3f wi;
		VisibilityTester vis;
		for (auto &&l : scene.lights) {
			ColorXYZ intensity = l->sample_Li(isect, &wi, &vis);
			if(!(intensity == BLACK)) {
				// Ray from intersect point to the light source
				Ray shadow_ray = Ray(isect.p, wi);
				shadow_ray.t_min = EPSILON;
				shadow_ray.t_max = vis.light_distance;
				if(!scene.intersect_p(shadow_ray)) {
					Vector3f h = -ray.direction + wi;
					h.make_unit_vector();
					L += fm->diffuse * intensity * std::max(real_type(0.0), dot(isect.n, wi));
					if(fm->glossiness > 1) {
						L += fm->specular * intensity * pow( std::max(real_type(0.0), dot(isect.n, h)), fm->glossiness );
					}
				}
			}
		}
		if(scene.ambientLight != nullptr) {
			L += fm->ambient * scene.ambientLight->intensity;
		}
		if (depth < this->max_depth) {
			Vector3f dir = ray.direction - 2 * (dot(ray.direction, isect.n)) * isect.n;
			dir.make_unit_vector();
			Ray reflected_ray = Ray(isect.p+(dir*EPSILON), dir);
			L += fm->mirror * Li(reflected_ray, scene, pixel, depth+1);
		}
	}
	return L;
}
} // namespace rt3