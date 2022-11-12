#include "background.hpp"


namespace rt3 {


Spectrum Background::lerp(const Spectrum &A, const Spectrum &B, real_type t) const
{
  Spectrum result{Lerp(t, A.x(), B.x()), Lerp(t, A.y(), B.y()), Lerp(t, A.z(), B.z())};
  return result;
}

/*!
 * Samples a color base on spherical interpolation of an image ou colored
 * background.
 *
 * @param pixel_ndc Pixel position in NDC space,  in \f$[0:1]\f$.
 * \return The interpolated color.
 */
Spectrum Background::sampleXYZ(const Point2f &pixel_ndc) {
  return color;
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

std::unique_ptr<Background> create_texture_background(const ParamSet &ps) {
  string filename = retrieve(ps, "filename", string{"teste.png"});
  string mapping = retrieve(ps, "mapping", string{"screen"});
  return make_unique<BackgroundTexture>(filename, mapping);
}
}  // namespace rt3
