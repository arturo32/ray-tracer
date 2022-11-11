#include "background.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../../ext/stb_image.hpp"


namespace rt3{


SphericBackground::SphericBackground(const char* path) { 
  int* x = new int;
  
  int* y = new int;
  int* comp = new int;
  
  
  this->image = stbi_load(path, x, y, comp, 0);
  this->resolution = Point2i(*x, *y);

  std::cout << *x << " " << *y << " " << *comp << std::endl;
}


Spectrum SphericBackground::sampleXYZ(const Ray& ray){
  real_type x = ray.direction.x();
  real_type y = ray.direction.y();
  real_type z = ray.direction.z();
  real_type theta = atan(z/x);
  real_type phi = atan(sqrt(x*x + z*z)/y);

  real_type xCoordenate = (this->resolution.x() * theta) / 2 * (real_type) M_PI;
  real_type yCoordenate = (this->resolution.y() * phi) / (real_type) M_PI;


  uint pos = 3*8*((yCoordenate * this->resolution.x()) + xCoordenate);
  
  return Spectrum(this->image[pos], this->image[pos + 1], this->image[pos + 2]); 
}


} // namespace rt3