#include "background.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../../ext/stb_image.hpp"


namespace rt3{


BackgroundTexture::BackgroundTexture(string path, string mapping) { 
  int* x = new int;
  int* y = new int;
  int* comp = new int;
  this->image = stbi_load(path.begin().base(), x, y, comp, 0);
  this->resolution = Point2i(*x, *y);
  
  if(mapping == "screen") {
    this->mapping_type = Background::mapping_t::screen;
  }
  else {
    this->mapping_type = Background::mapping_t::spherical;
  }
}


Spectrum BackgroundTexture::sampleXYZ(const Ray& r) {
  real_type pi = (real_type)M_PI;
  real_type pi_2 = (real_type)M_PI_2;
  
  Vector3f ray{r.direction.x(), r.direction.y(), r.direction.z()};
  ray.make_unit_vector();

  real_type x = ray.x();
  real_type y = -ray.y(); // ?
  real_type z = ray.z();
  
  real_type theta = atan2(x, z);  
  real_type phi = atan2( y, sqrt(x*x + z*z));
  if(phi > pi_2) {
    phi -= (phi - pi_2)*2;
  }
  if(phi < - pi_2) {
    phi += (-pi_2 - phi)*2;
  }

  int xCoordenate = round(((theta+pi)/(2*pi)) * this->resolution.x());
  
  int yCoordenate = round(((phi+pi_2)/pi) * this->resolution.y());


  uint pos = 3*((yCoordenate * this->resolution.x()) + xCoordenate);
  real_type xx = this->image[pos];
  real_type yy = this->image[pos+1];
  real_type zz = this->image[pos+2];
  Spectrum result{xx, yy, zz};
  result /= 255.0;
  return result; 
}


} // namespace rt3