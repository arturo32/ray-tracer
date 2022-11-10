#include "background.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../../ext/stb_image.hpp"


namespace rt3{


SphericBackground::SphericBackground(const char* path) { 
  int* x = nullptr;
  int* y = nullptr;
  int* comp = nullptr;
  this->image = stbi_load(path, x, y, comp, 0);
  this->resolution = Point2i(*x, *y);

  std::cout << *x << " " << *y << " " << *comp << std::endl;
}


Spectrum SphericBackground::sampleXYZ(const Point2f &pixel_ndc){
  
  
  // int x = pixel_ndc.x() * this->resolution.x();
  // int y = pixel_ndc.y() * this->resolution.y();

  // real_type theta = tan(
  // real_type phi = 

  // uint pos = 3*((pixel_ndc.y() * this->resolution.x()) + pixel_ndc.x());

  // int x = pixel_ndc.x() * ;
  // this->image[]
  return Spectrum(); 
}


} // namespace rt3