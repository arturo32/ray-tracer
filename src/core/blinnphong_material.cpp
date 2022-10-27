#include "blinnphong_material.hpp"

namespace rt3 {

BlinnPhongMaterial::BlinnPhongMaterial(Vector3f ambient, Vector3f diffuse,
									   Vector3f specular, real_type glossiness):
	ambient{ambient}, diffuse{diffuse}, specular{specular}, glossiness{glossiness} {/*EMPTY*/}

void BlinnPhongMaterial::scatter(Surfel* sf, Ray* r) {
	// TODO
}

}