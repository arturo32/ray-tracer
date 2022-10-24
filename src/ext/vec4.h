//==================================================================================================
// Written in 2016 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is distributed
// without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication along
// with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==================================================================================================

#ifndef VEC4H
#define VEC4H

#include <math.h>
#include <stdlib.h>
#include <iostream>

template <typename T>
class vec4  {

    
public:
    vec4() {}
    vec4(T e0, T e1, T e2, T e3): e{e0, e1, e2, e3} {}
    explicit vec4(std::initializer_list<T> ilist) {
        assert(ilist.size() == 4);
        int i = 0;
        for (auto it = ilist.begin(); it != ilist.end(); ++it) {
            e[i] = *it;
            ++i;
        } 
    }
    inline T x() const { return e[0]; }
    inline T y() const { return e[1]; }
    inline T z() const { return e[2]; }
    inline T d() const { return e[3]; }

    inline T r() const { return e[0]; }
    inline T g() const { return e[1]; }
    inline T b() const { return e[2]; }
    inline T a() const { return e[3]; }
    
    inline const vec4& operator+() const { return *this; }
    inline vec4 operator-() const { return vec4(-e[0], -e[1], -e[2], -e[3]); }
    inline T operator[](int i) const { return e[i]; }
    inline T& operator[](int i) { return e[i]; };
    
    inline vec4& operator+=(const vec4 &v2);
    inline vec4& operator-=(const vec4 &v2);
    inline vec4& operator*=(const vec4 &v2);
    inline vec4& operator/=(const vec4 &v2);
    inline vec4& operator*=(const T t);
    inline vec4& operator/=(const T t);
    
    inline bool operator==(const vec4 &rhs) const { return tied() == rhs.tied(); }
    

    inline void clamp(T min, T max) {
        e[0] = std::clamp(e[0], min, max);
        e[1] = std::clamp(e[1], min, max);
        e[2] = std::clamp(e[2], min, max);
        e[3] = std::clamp(e[3], min, max);
    };
    inline T length() const { return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2] + e[3]*e[3]); }
    inline T squared_length() const { return e[0]*e[0] + e[1]*e[1] + e[2]*e[2] + e[3]*e[3]; }
    inline void make_unit_vector();
    
  private:
    T e[4];
    auto tied() const { return std::tie(e[0], e[1], e[2], e[3]); }
};


template <typename T>
inline std::istream& operator>>(std::istream &is, vec4<T> &t) {
    is >> t.x() >> t.y() >> t.z() >> t.d();
    return is;
}

template <typename T>
inline std::ostream& operator<<(std::ostream &os, const vec4<T> &t) {
    os << t.x() << " " << t.y() << " " << t.z() << " " << t.d();
    return os;
}

template <typename T>
inline void vec4<T>::make_unit_vector() {
    T k = 1.0 / length();
    e[0] *= k; e[1] *= k; e[2] *= k; e[3] *= k;
}

template <typename T>
inline vec4<T> operator+(const vec4<T> &v1, const vec4<T> &v2) {
    return vec4<T>(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2], v1.e[3] + v2.e[3]);
}

template <typename T>
inline vec4<T> operator-(const vec4<T> &v1, const vec4<T> &v2) {
    return vec4<T>(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2], v1.e[3] - v2.e[3]);
}

template <typename T>
inline vec4<T> operator*(const vec4<T> &v1, const vec4<T> &v2) {
    return vec4<T>(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2], v1.e[3] * v2.e[3]);
}

template <typename T>
inline vec4<T> operator/(const vec4<T> &v1, const vec4<T> &v2) {
    return vec4<T>(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2], v1.e[3] / v2.e[3]);
}

template <typename T>
inline vec4<T> operator*(T t, const vec4<T> &v) {
    return vec4<T>(t*v.e[0], t*v.e[1], t*v.e[2], t*v.e[3]);
}

template <typename T>
inline vec4<T> operator/(vec4<T> v, T t) {
    return vec4<T>(v.e[0]/t, v.e[1]/t, v.e[2]/t, v.e[3]/t);
}

template <typename T>
inline vec4<T> operator*(const vec4<T> &v, T t) {
    return vec4<T>(t*v.e[0], t*v.e[1], t*v.e[2], t*v.e[3]);
}

template <typename T>
inline T dot(const vec4<T> &v1, const vec4<T> &v2) {
    return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1]  + v1.e[2] * v2.e[2], v1.e[3] * v2.e[3];
}

// template <typename T>
// inline vec4<T> cross(const vec4<T> &v1, const vec4<T> &v2) {
//     return vec4<T>( (v1.e[1]*v2.e[2] - v1.e[2]*v2.e[1]),
//                 (-(v1.e[0]*v2.e[2] - v1.e[2]*v2.e[0])),
//                 (v1.e[0]*v2.e[1] - v1.e[1]*v2.e[0]));
// }

template <typename T>
inline vec4<T>& vec4<T>::operator+=(const vec4<T> &v){
    e[0]  += v.e[0];
    e[1]  += v.e[1];
    e[2]  += v.e[2];
    e[3]  += v.e[3];
    return *this;
}

template <typename T>
inline vec4<T>& vec4<T>::operator*=(const vec4<T> &v){
    e[0]  *= v.e[0];
    e[1]  *= v.e[1];
    e[2]  *= v.e[2];
    e[3]  *= v.e[3];
    return *this;
}

template <typename T>
inline vec4<T>& vec4<T>::operator/=(const vec4<T> &v){
    e[0]  /= v.e[0];
    e[1]  /= v.e[1];
    e[2]  /= v.e[2];
    e[3]  /= v.e[3];
    return *this;
}

template <typename T>
inline vec4<T>& vec4<T>::operator-=(const vec4<T>& v) {
    e[0]  -= v.e[0];
    e[1]  -= v.e[1];
    e[2]  -= v.e[2];
    e[3]  -= v.e[3];
    return *this;
}

template <typename T>
inline vec4<T>& vec4<T>::operator*=(const T t) {
    e[0]  *= t;
    e[1]  *= t;
    e[2]  *= t;
    e[3]  *= t;
    return *this;
}

template <typename T>
inline vec4<T>& vec4<T>::operator/=(const T t) {
    T k = 1.0/t;
    e[0]  *= k;
    e[1]  *= k;
    e[2]  *= k;
    e[3]  *= k;
    return *this;
}

template <typename T>
inline vec4<T> unit_vector(vec4<T> v) {
    return v / v.length();
}

#endif
