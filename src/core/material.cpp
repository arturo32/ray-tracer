#include "material.hpp"

namespace rt3 {

BlinnPhongMaterial::BlinnPhongMaterial(
	Vector3f ambient, 
	Vector3f diffuse,
	Vector3f specular,
	Vector3f mirror, 
	real_type glossiness):
	ambient{ambient}, diffuse{diffuse},
	specular{specular}, mirror{mirror},
	glossiness{glossiness} {/*EMPTY*/}

void BlinnPhongMaterial::scatter(Surfel* sf, Ray* r) {
	// TODO
}

}