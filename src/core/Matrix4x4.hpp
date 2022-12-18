#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include "rt3.hpp"
#include <sstream>

namespace rt3 {

	struct Matrix4x4 {
		Matrix4x4() {
			m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.f;
			m[0][1] = m[0][2] = m[0][3] = m[1][0] =
			m[1][2] = m[1][3] = m[2][0] = m[2][1] = 
			m[2][3] = m[3][0] = m[3][1] = m[3][2] = 0.f;
		}
		Matrix4x4(real_type mat[4][4])
		{
			for (size_t i = 0; i < 4; ++i)
				for (size_t j = 0; j < 4; ++j)
					m[i][j] = mat[i][j];
		}
		Matrix4x4(real_type t00, real_type t01, real_type t02, real_type t03,
				  real_type t10, real_type t11, real_type t12, real_type t13,
				  real_type t20, real_type t21, real_type t22, real_type t23,
				  real_type t30, real_type t31, real_type t32, real_type t33)
		{
			m[0][0] = t00;m[1][0] = t10;m[2][0] = t20;m[3][0] = t30;
			m[0][1] = t01;m[1][1] = t11;m[2][1] = t21;m[3][1] = t31;
			m[0][2] = t02;m[1][2] = t12;m[2][2] = t22;m[3][2] = t32;
			m[0][3] = t03;m[1][3] = t13;m[2][3] = t23;m[3][3] = t33;
		}
		bool operator==(const Matrix4x4 &m2) const {
			for (int i = 0; i < 4; ++i)
				for (int j = 0; j < 4; ++j)
					if (m[i][j] != m2.m[i][j]) return false;
			return true;
		}
		bool operator!=(const Matrix4x4 &m2) const {
			for (int i = 0; i < 4; ++i)
				for (int j = 0; j < 4; ++j)
					if (m[i][j] != m2.m[i][j]) return true;
			return false;
		}
		friend Matrix4x4 Transpose(const Matrix4x4 &m2)
		{
			Matrix4x4 r;
			for (size_t i = 0; i < 4; ++i)
				for (size_t j = 0; j < 4; ++j)
					r.m[j][i] = m2.m[i][j];
			return r;
		}
		string Print() const {
			string s = "";
			//s += "[ ";
			for (int i = 0; i < 4; ++i) {
				s += "  [ ";
				for (int j = 0; j < 4; ++j)  {
					std::ostringstream ss;
					ss << m[i][j];
					s += ss.str();
					if (j != 3){
						s += ", ";
					}
				}
				s += " ]\n";
			}
			//s += " ] ";
			
			return s;
		}
		static Matrix4x4 Mul(const Matrix4x4 &m1, const Matrix4x4 &m2) {
			Matrix4x4 r;
			for (int i = 0; i < 4; ++i)
				for (int j = 0; j < 4; ++j)
					r.m[i][j] = m1.m[i][0] * m2.m[0][j] + 
								m1.m[i][1] * m2.m[1][j] + 
								m1.m[i][2] * m2.m[2][j] + 
								m1.m[i][3] * m2.m[3][j];
			return r;
		}
		friend Matrix4x4 Inverse(const Matrix4x4 &m)
		{
			real_type A2323 = m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2] ;
			real_type A1323 = m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1] ;
			real_type A1223 = m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1] ;
			real_type A0323 = m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0] ;
			real_type A0223 = m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0] ;
			real_type A0123 = m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0] ;
			real_type A2313 = m.m[1][2] * m.m[3][3] - m.m[1][3] * m.m[3][2] ;
			real_type A1313 = m.m[1][1] * m.m[3][3] - m.m[1][3] * m.m[3][1] ;
			real_type A1213 = m.m[1][1] * m.m[3][2] - m.m[1][2] * m.m[3][1] ;
			real_type A2312 = m.m[1][2] * m.m[2][3] - m.m[1][3] * m.m[2][2] ;
			real_type A1312 = m.m[1][1] * m.m[2][3] - m.m[1][3] * m.m[2][1] ;
			real_type A1212 = m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1] ;
			real_type A0313 = m.m[1][0] * m.m[3][3] - m.m[1][3] * m.m[3][0] ;
			real_type A0213 = m.m[1][0] * m.m[3][2] - m.m[1][2] * m.m[3][0] ;
			real_type A0312 = m.m[1][0] * m.m[2][3] - m.m[1][3] * m.m[2][0] ;
			real_type A0212 = m.m[1][0] * m.m[2][2] - m.m[1][2] * m.m[2][0] ;
			real_type A0113 = m.m[1][0] * m.m[3][1] - m.m[1][1] * m.m[3][0] ;
			real_type A0112 = m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0] ;

			real_type det = m.m[0][0] * ( m.m[1][1] * A2323 - m.m[1][2] * A1323 + m.m[1][3] * A1223 ) 
						  - m.m[0][1] * ( m.m[1][0] * A2323 - m.m[1][2] * A0323 + m.m[1][3] * A0223 ) 
						  + m.m[0][2] * ( m.m[1][0] * A1323 - m.m[1][1] * A0323 + m.m[1][3] * A0123 ) 
						  - m.m[0][3] * ( m.m[1][0] * A1223 - m.m[1][1] * A0223 + m.m[1][2] * A0123 ) ;
			det = 1 / det;
			return Matrix4x4(
				det *   ( m.m[1][1] * A2323 - m.m[1][2] * A1323 + m.m[1][3] * A1223 ),
				det * - ( m.m[0][1] * A2323 - m.m[0][2] * A1323 + m.m[0][3] * A1223 ),
				det *   ( m.m[0][1] * A2313 - m.m[0][2] * A1313 + m.m[0][3] * A1213 ),
				det * - ( m.m[0][1] * A2312 - m.m[0][2] * A1312 + m.m[0][3] * A1212 ),
				det * - ( m.m[1][0] * A2323 - m.m[1][2] * A0323 + m.m[1][3] * A0223 ),
				det *   ( m.m[0][0] * A2323 - m.m[0][2] * A0323 + m.m[0][3] * A0223 ),
				det * - ( m.m[0][0] * A2313 - m.m[0][2] * A0313 + m.m[0][3] * A0213 ),
				det *   ( m.m[0][0] * A2312 - m.m[0][2] * A0312 + m.m[0][3] * A0212 ),
				det *   ( m.m[1][0] * A1323 - m.m[1][1] * A0323 + m.m[1][3] * A0123 ),
				det * - ( m.m[0][0] * A1323 - m.m[0][1] * A0323 + m.m[0][3] * A0123 ),
				det *   ( m.m[0][0] * A1313 - m.m[0][1] * A0313 + m.m[0][3] * A0113 ),
				det * - ( m.m[0][0] * A1312 - m.m[0][1] * A0312 + m.m[0][3] * A0112 ),
				det * - ( m.m[1][0] * A1223 - m.m[1][1] * A0223 + m.m[1][2] * A0123 ),
				det *   ( m.m[0][0] * A1223 - m.m[0][1] * A0223 + m.m[0][2] * A0123 ),
				det * - ( m.m[0][0] * A1213 - m.m[0][1] * A0213 + m.m[0][2] * A0113 ),
				det *   ( m.m[0][0] * A1212 - m.m[0][1] * A0212 + m.m[0][2] * A0112 )
			);
		}

		real_type m[4][4];
	};

} // rt3 namespace

#endif