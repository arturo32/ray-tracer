#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "rt3-base.h"
#include "rt3.h"

namespace rt3 {
// Create a virtual class Background and derive BackgroundColor,
// BackgroundSphereImage, BackgroundSkyBoxImage.
/*!
 * A background is basically a rectangle, have a color associated to each
 * corner. A background might be sampled based on a normalized coordinate in
 * \f$[0,1]^2.\f$. We generate samples with bilinear interpolation of the corner
 * colors. We consider the corners in a circular clockwise order, starting at
 * the lower-left corner.
 *
 * Alternatively, a background might be a regular image.
 */
class Background {
 public:
  /// Types of color mapping scheme
  enum class mapping_t {
    screen = 0,  //!< background is assigned to the image screen
    spherical    //!< background is assigne to a sphere surrounding the scene.
  };

 Spectrum color;

 public:
  mapping_t mapping_type;  //!< sampling type we use to pick a color.

  /// Return the linearly interpolated color in [A;B], based on the parameter \f$0\leq t \leq 1.\f$
  Spectrum lerp(const Spectrum &A, const Spectrum &B, float t) const;

  /// Ctro receiving a single color for the entire background.
  Background(Spectrum color = Spectrum(0,0,0), mapping_t mt = mapping_t::screen)
      : color{color}, mapping_type{mt} { /* empty */
  }

  //virtual ~Background(){/* empty */};
  virtual ~Background() = default;
  virtual Spectrum sampleXYZ(const Point2f &pixel_ndc);
};

class BackgroundColor : public Background {
 private:
  /// Each corner has a color associated with.
  Spectrum corners[4] = {Spectrum({0, 0, 0}), Spectrum({0, 0, 0}),
                         Spectrum({0, 0, 0}), Spectrum({0, 0, 0})};
  /// Corner indices.
  enum Corners_e {
    bl = 0,  //!< Bottom left corner.
    tl,      //!< Top left corner.
    tr,      //!< Top right corner.
    br       //!< Bottom right corner.
  };

 public:
  /// Ctro receives a list of four colors, for each corner.
  BackgroundColor(Spectrum values[]) {
    for (size_t i = 0; i < 4; ++i) {
      corners[i] = values[i];
    }
  }

  //virtual ~BackgroundColor(){};
  ~BackgroundColor() = default;
  Spectrum sampleXYZ(const Point2f &pixel_ndc) override;
};


// class SphericBackground : public Background {
//  public:
//   SphericBackground(String path) { }

//   //virtual ~BackgroundColor(){};
//   ~SphericBackground() = default;
//   Spectrum sampleXYZ(const Point2f &pixel_ndc) override;
// };


// factory pattern functions.
std::unique_ptr<Background> create_color_background(const ParamSet &ps);
}  // namespace rt3
#endif
