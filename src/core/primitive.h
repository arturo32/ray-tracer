#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <cassert>
#include "rt3-base.h"
#include "rt3.h"
#include "paramset.h"
#include "ray.h"
#include "../ext/vec3.h"
#include "surfel.h"
#include "shape.h"

namespace rt3 {

using Bounds3f = vec2<Point3f>;
using ColorXYZ = vec3<float>;

class Surfel;

class Material {
	public:
		virtual ~Material(){};
		virtual void scatter(Surfel* sf, Ray* r) = 0;
};

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

class Primitive {
	public:
		Primitive(){};
		~Primitive() = default;
		virtual Bounds3f world_bounds() = 0;
		virtual bool intersect( Ray& r, Surfel *sf ) const = 0;
		// Simpler & faster version of intersection that only return true/false.
		// It does not compute the hit point information.
		virtual bool intersect_p( const Ray& r ) const = 0;
		virtual const std::shared_ptr<Material> get_material(void) const = 0;
};

class AggregatePrimitive : public Primitive {
	public:
		AggregatePrimitive(){};
		virtual Bounds3f world_bounds() = 0;
		virtual bool intersect( Ray& r, Surfel *sf ) const = 0;
		virtual bool intersect_p( const Ray& r ) const = 0;
		const std::shared_ptr<Material> get_material(void) const { 
			std::cerr << "Não se deve chamar esse método nessa classe!" << std::endl;
			assert(false);
		} 
		virtual void add(std::shared_ptr< Primitive > p) = 0;
};

class PrimList : public AggregatePrimitive {
	public:
		std::vector<std::shared_ptr< Primitive >> primitives;
		PrimList() = default;
		PrimList(std::vector<std::shared_ptr< Primitive >> prim) : primitives{prim}{}
		Bounds3f world_bounds() {
			return Bounds3f(Point3f(0,0,0), Point3f(0,0,0));
		};
		bool intersect( Ray& r, Surfel *sf ) const {
            bool b = false;
            for (size_t i = 0; i < primitives.size(); ++i) {
                if(primitives.at(i)->intersect(r, sf)) {
                    b = true;
                }
            }
            return b;
		};
		bool intersect_p( const Ray& r ) const{
			for (size_t i = 0; i < primitives.size(); ++i) {
                if(primitives.at(i)->intersect_p(r)) {
                    return true;
                }
            }
            return false;
		};
		void add(std::shared_ptr< Primitive > p){
			primitives.push_back(p);
		}
};


class GeometricPrimitive : public Primitive {
	protected:
		std::shared_ptr<Material> material;
		std::shared_ptr<Shape> shape;
	public:
		GeometricPrimitive(std::shared_ptr<Material> material, std::shared_ptr<Shape> shape): 
		material{material}, shape{shape}{}
		
		~GeometricPrimitive() = default;
		
		Bounds3f world_bounds() { 
			return this->shape->world_bounds(); 
		}
		bool intersect( Ray& r, Surfel *sf ) const { 
			real_type t_hit;
			if(this->shape->intersect(r, t_hit, sf)) {
				sf->primitive = this;
				r.t_max = t_hit;
				return true;
			}
			return false;
		}
		bool intersect_p( const Ray& r ) const { 
			return this->shape->intersect_p(r); 
		}
		const std::shared_ptr<Material> get_material(void) const { 
			return this->material; 
		}
		void set_material(std::shared_ptr<Material> mat) {
			material = mat;
		}
};

}

#endif  
