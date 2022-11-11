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

  std::cout << "RESOLUÇÃO IMAGEM: " << *x << " " << *y << " " << *comp << "------------------------------------------" << std::endl;
}


Spectrum SphericBackground::sampleXYZ(const Ray& r){
  real_type pi = (real_type)M_PI;
  
  Vector3f ray{r.direction.x(), r.direction.y(), r.direction.z()};
  ray.make_unit_vector();
  real_type x = ray.x();
  real_type y = ray.y();
  real_type z = ray.z();
  
  real_type theta = atan2(x, z);  
  real_type phi = atan2(sqrt(x*x + z*z), y);

  int xCoordenate =  floor(((theta+pi)/(2*pi)) * this->resolution.x());;
  // std::cout << theta << std::endl;
  // std::cout << "teste " << this->resolution.x() << std::endl;
  
  int yCoordenate = floor(((phi+pi)/(2*pi)) * this->resolution.y());;

  // std::cout << theta <<  ", " << phi <<std::endl;
  // std::cout << "coodernadas: " << xCoordenate <<  ", " << yCoordenate <<std::endl;


  uint pos = 3*((yCoordenate * this->resolution.x()) + xCoordenate);
  return Spectrum(this->image[pos], this->image[pos + 1], this->image[pos + 2]); 
}


} // namespace rt3