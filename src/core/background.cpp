#include "background.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../ext/stb_image.hpp"

namespace rt3 {
/*!
 * Samples a color base on spherical interpolation of an image ou colored
 * background.
 *
 * @param pixel_ndc Pixel position in NDC space,  in \f$[0:1]\f$.
 * \return The interpolated color.
 */

Spectrum Background::lerp(const Spectrum &A, const Spectrum &B, real_type t) const
{
  Spectrum result{Lerp(t, A.x(), B.x()), Lerp(t, A.y(), B.y()), Lerp(t, A.z(), B.z())};
  return result;
}

Spectrum Background::sampleXYZ(const Point2f &pixel_ndc) {
  return color;
}

Spectrum BackgroundColor::sampleXYZ(const Point2f &pixel_ndc) {
  Spectrum Xb = this->lerp(corners[bl], corners[br], pixel_ndc.x());
  Spectrum Xt = this->lerp(corners[tl], corners[tr], pixel_ndc.x());
  Spectrum result = this->lerp(Xt, Xb, pixel_ndc.y());
  result /= 255.0;
  result.clamp(0.0, 1.0);
  return result;
}



std::unique_ptr<Background> create_color_background(const ParamSet &ps) {
  std::string type = retrieve(ps, "type", std::string("colors"));
  std::string mapping = retrieve(ps, "mapping", std::string("colors"));
  Spectrum color = retrieve(ps, "color", Spectrum(-1,-1,-1));

  if (color == Spectrum(-1,-1,-1)) {
    Spectrum bl = retrieve(ps, "bl", Spectrum(0,0,0));
    if(bl.x() <= 1.0 && bl.y() <= 1.0 && bl.z() <= 1) {
        bl *= 255.0;
        bl.clamp(0.0, 255.0);
    }
    Spectrum tl = retrieve(ps, "tl", Spectrum(255,255,255));
    if(tl.x() <= 1.0 && tl.y() <= 1.0 && tl.z() <= 1) {
        tl *= 255.0;
        tl.clamp(0.0, 255.0);
    }
    Spectrum tr = retrieve(ps, "tr", Spectrum(255,255,255));
    if(tr.x() <= 1.0 && tr.y() <= 1.0 && tr.z() <= 1) {
        tr *= 255.0;
        tr.clamp(0.0, 255.0);
    }
    Spectrum br = retrieve(ps, "br", Spectrum(0,0,0));
    if(br.x() <= 1.0 && br.y() <= 1.0 && br.z() <= 1) {
        br *= 255.0;
        br.clamp(0.0, 255.0);
    }
    Spectrum corners[4] = {bl, tl, tr, br};
    return make_unique<BackgroundColor>(corners);
  } else {
    if(color.x() <= 1.0 && color.y() <= 1.0 && color.z() <= 1) {
        color *= 255.0;
        color.clamp(0.0, 255.0);
    }
    return make_unique<Background>(color);
  }
}

SphericBackground::SphericBackground(const char* path) { 
  int* x = nullptr;
  int* y = nullptr;
  int* comp = new int;
  this->image = stbi_load(path, x, y, comp, 0);
  this->resolution = Point2i(*x, *y);

  std::cout << *x << " " << *y << " " << *comp << std::endl;
}


Spectrum SphericBackground::sampleXYZ(const Point2f &pixel_ndc){
  return Spectrum(); 
}



}  // namespace rt3
