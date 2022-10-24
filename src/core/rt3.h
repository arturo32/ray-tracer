#ifndef RT3_H
#define RT3_H 1

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>
using std::vector;
#include <stack>
#include <map>
#include <unordered_map>
#include <string>
using std::string;
#include <algorithm>
#include <utility>
#include <tuple>
using std::tuple;
#include <array>
using std::array;

#include "../ext/vec4.h"
#include "../ext/vec3.h"
#include "../ext/vec2.h"

//== Alias to the chosen data structure to implement a dictionary.
//#define Dictionary std::unordered_map
#define Dictionary std::map

//=== Aliases
namespace rt3 {
// Alias to a Point3f (simulation)
using Point3f = vec3<float>;
using Vector3f = vec3<float>;
using Vector4f = vec4<float>;
using ColorXYZ = vec3<float>;
using Spectrum = vec3<float>;
using Normal3f = vec3<float>;

// List of points
using ListPoint3f = std::vector<Point3f>;

using Vector3i = vec3<int>;
using Point3i = vec3<int>;
using Point2ui = vec2<int>;
using Point2f = vec2<float>;

template <typename T, size_t S>
std::ostream& operator<<(std::ostream& os, const std::array<T, S>& v) {
  os << "[ ";
  for (const auto& e : v) os << e << " ";
  os << "]";
  return os;
}

// Global Forward Declarations
class Film;
class Background;
class BackgroundColor;

//=== aliases
typedef float real_type;
typedef size_t size_type;
typedef std::tuple<bool, std::string> result_type;

/// This struct holds information provided via command line arguments
struct RunningOptions {
  RunningOptions() : filename{""}, outfile{""}, quick_render{false} {
    crop_window[0][0] = 0;  //!< x0
    crop_window[0][1] = 1;  //!< x1,
    crop_window[1][0] = 0;  //!< y0
    crop_window[1][1] = 1;  //!< y1
  }
  // [row=0] -> X; [row=1] -> Y
  // x0, x1, y0, y1
  real_type crop_window[2][2];  //!< Crop window to render. 1 = 100% of the full
                                //!< resolition.
  std::string filename;         //!< input scene file name.
  std::string outfile;          //!< output image file name.
  bool quick_render;  //!< when set, render image with 1/4 of the requested
                      //!< resolition.
};


//=== Global Inline Functions

/*! Linear interpolation.
 * \param t The parameter, in [0,1].
 * \param v1 The initial interpolation value.
 * \param v2 The final interpolation value.
 * \return The interpolated value.
 */
//
inline float Lerp(float t, float v1, float v2) {
  return (1.f - t) * v1 + t * v2;
}

/// Clamp T to [low,high].
template <typename T, typename U, typename V>
inline T Clamp(T val, U low, V high) {
  if (val < low)
    return low;
  else if (val > high)
    return high;
  else
    return val;
}

/// Degrees to radians.
inline float Radians(float deg) { return ((float)M_PI / 180.f) * deg; }

/// Radians to degreees.
inline float Degrees(float rad) { return (180.f / (float)M_PI) * rad; }
}  // namespace rt3

#endif  // RT3_H
