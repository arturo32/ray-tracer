#ifndef BLINNPHONGMATERIAL_H
#define BLINNPHONGMATERIAL_H

#include "material.hpp"

namespace rt3 {

class BlinnPhongMaterial : public Material {
	public:
		BlinnPhongMaterial(Vector3f ambient, Vector3f diffuse, Vector3f specular, real_type glossiness);
		~BlinnPhongMaterial() {};
		void scatter(Surfel* sf, Ray* r);
		Vector3f ambient;  // [0,1]
		Vector3f diffuse;  // [0,1]
		Vector3f specular; // [0,1]
		real_type glossiness; 
};

}
#endif