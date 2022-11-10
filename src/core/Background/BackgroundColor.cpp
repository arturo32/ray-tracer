#include "background.hpp"

namespace rt3 {

BackgroundColor::BackgroundColor(Spectrum values[]) {
  for (size_t i = 0; i < 4; ++i) {
    corners[i] = values[i];
  }
}

Spectrum BackgroundColor::sampleXYZ(const Point2f &pixel_ndc) {
  Spectrum Xb = this->lerp(corners[bl], corners[br], pixel_ndc.x());
  Spectrum Xt = this->lerp(corners[tl], corners[tr], pixel_ndc.x());
  Spectrum result = this->lerp(Xb, Xt, pixel_ndc.y());
  result /= 255.0;
  result.clamp(0.0, 1.0);
  return result;
}

} // namespace rt3