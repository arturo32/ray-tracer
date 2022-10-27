#ifndef MATERIAL_H
#define MATERIAL_H

#include "surfel.hpp"

namespace rt3 {

class Material {
	public:
		virtual ~Material(){};
		virtual void scatter(Surfel* sf, Ray* r) = 0;
};

}
#endif