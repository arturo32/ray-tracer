#ifndef MATERIAL_H
#define MATERIAL_H

#include "rt3.hpp"

namespace rt3 {

class Surfel;
class Ray;
using Vector3f = vec3<real_type>;

class Material {
	public:
		virtual ~Material(){};
		virtual void scatter(Surfel* sf, Ray* r) = 0;
};

class FlatMaterial : public Material {
	public:
		FlatMaterial(const ColorXYZ& color): color{color}{}
		~FlatMaterial() {};
		void scatter(Surfel* sf, Ray* r) {
			/* EMPTY */
		};
		ColorXYZ kd() { return color; };
		const ColorXYZ color;
};

class BlinnPhongMaterial : public Material {
	public:
		BlinnPhongMaterial(
			Vector3f ambient, 
			Vector3f diffuse,
		 	Vector3f specular, 
			Vector3f mirror, 
			real_type glossiness
		);
		~BlinnPhongMaterial() {};
		void scatter(Surfel* sf, Ray* r);
		Vector3f ambient;  // [0,1]
		Vector3f diffuse;  // [0,1]
		Vector3f specular; // [0,1]
		Vector3f mirror; // [0,1]
		real_type glossiness = 0; 
};

}
#endif