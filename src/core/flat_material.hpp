#ifndef FLATMATERIAL_H
#define FLATMATERIAL_H

#include "material.hpp"

namespace rt3 {

class FlatMaterial : public Material {
	public:
		FlatMaterial(const ColorXYZ& color): Material(), color{color}{}
		~FlatMaterial() {};
		void scatter(Surfel* sf, Ray* r) {
			/* EMPTY */
		};
		ColorXYZ kd() { return color; };
		const ColorXYZ color;
};

}
#endif