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

#ifndef VEC2H
#define VEC2H

#include <math.h>
#include <stdlib.h>
#include <iostream>

template <typename T>
class vec2  {
    
public:
    vec2() {}
    vec2(T e0, T e1): e{e0, e1} {}
    explicit vec2(std::initializer_list<T> ilist) {
        assert(ilist.size() == 2);
        int i = 0;
        for (auto it = ilist.begin(); it != ilist.end(); ++it) {
            e[i] = *it;
            ++i;
        } 
    }
    inline T x() const { return e[0]; }
    inline T y() const { return e[1]; }
    inline T r() const { return e[0]; }
    
    inline const vec2& operator+() const { return *this; }
    inline vec2 operator-() const { return vec2(-e[0], -e[1]); }
    inline T operator[](int i) const { return e[i]; }
    inline T& operator[](int i) { return e[i]; };
    
    inline vec2& operator+=(const vec2 &v2);
    inline vec2& operator-=(const vec2 &v2);
    inline vec2& operator*=(const vec2 &v2);
    inline vec2& operator/=(const vec2 &v2);
    inline vec2& operator*=(const T t);
    inline vec2& operator/=(const T t);
    
    inline T length() const { return sqrt(e[0]*e[0] + e[1]*e[1]); }
    inline T squared_length() const { return e[0]*e[0] + e[1]*e[1]; }
    inline void make_unit_vector();
    
    T e[3];
};


template <typename T>
inline std::istream& operator>>(std::istream &is, vec2<T> &t) {
    is >> t.e[0] >> t.e[1];
    return is;
}

template <typename T>
inline std::ostream& operator<<(std::ostream &os, const vec2<T> &t) {
    os << t.e[0] << " " << t.e[1];
    return os;
}

template <typename T>
inline void vec2<T>::make_unit_vector() {
    T k = 1.0 / length();
    e[0] *= k; e[1] *= k;
}

template <typename T>
inline vec2<T> operator+(const vec2<T> &v1, const vec2<T> &v2) {
    return vec2(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1]);
}

template <typename T>
inline vec2<T> operator-(const vec2<T> &v1, const vec2<T> &v2) {
    return vec2<T>(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1]);
}

template <typename T>
inline vec2<T> operator*(const vec2<T> &v1, const vec2<T> &v2) {
    return vec2<T>(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1]);
}

template <typename T>
inline vec2<T> operator/(const vec2<T> &v1, const vec2<T> &v2) {
    return vec2<T>(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1]);
}

template <typename T>
inline vec2<T> operator*(T t, const vec2<T> &v) {
    return vec2<T>(t*v.e[0], t*v.e[1]);
}

template <typename T>
inline vec2<T> operator/(vec2<T> v, T t) {
    return vec2<T>(v.e[0]/t, v.e[1]/t);
}

template <typename T>
inline vec2<T> operator*(const vec2<T> &v, T t) {
    return vec2<T>(t*v.e[0], t*v.e[1]);
}

template <typename T>
inline T dot(const vec2<T> &v1, const vec2<T> &v2) {
    return v1.e[0] *v2.e[0] + v1.e[1] *v2.e[1];
}

template <typename T>
inline vec2<T>& vec2<T>::operator+=(const vec2<T> &v){
    e[0]  += v.e[0];
    e[1]  += v.e[1];
    return *this;
}

template <typename T>
inline vec2<T>& vec2<T>::operator*=(const vec2<T> &v){
    e[0]  *= v.e[0];
    e[1]  *= v.e[1];
    return *this;
}

template <typename T>
inline vec2<T>& vec2<T>::operator/=(const vec2<T> &v){
    e[0]  /= v.e[0];
    e[1]  /= v.e[1];
    return *this;
}

template <typename T>
inline vec2<T>& vec2<T>::operator-=(const vec2<T>& v) {
    e[0]  -= v.e[0];
    e[1]  -= v.e[1];
    return *this;
}

template <typename T>
inline vec2<T>& vec2<T>::operator*=(const T t) {
    e[0]  *= t;
    e[1]  *= t;
    return *this;
}

template <typename T>
inline vec2<T>& vec2<T>::operator/=(const T t) {
    T k = 1.0/t;
    
    e[0]  *= k;
    e[1]  *= k;
    return *this;
}

template <typename T>
inline vec2<T> unit_vector(vec2<T> v) {
    return v / v.length();
}

#endif
