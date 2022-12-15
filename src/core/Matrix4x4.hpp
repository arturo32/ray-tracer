#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include "rt3-base.hpp"


namespace rt3 {

	struct Matrix4x4 {
		Matrix4x4() {
			m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.f;
			m[0][1] = m[0][2] = m[0][3] = m[1][0] =
				m[1][2] = m[1][3] = m[2][0] = m[2][1] = m[2][3] =
				m[3][0] = m[3][1] = m[3][2] = 0.f;
		}
		Matrix4x4(real_type mat[4][4]);
		Matrix4x4(real_type t00, real_type t01, real_type t02, real_type t03,
					real_type t10, real_type t11, real_type t12, real_type t13,
					real_type t20, real_type t21, real_type t22, real_type t23,
					real_type t30, real_type t31, real_type t32, real_type t33);
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
		friend Matrix4x4 Transpose(const Matrix4x4 &);
		string Print() const {
			string s = "";
			s += "[ ";
			for (int i = 0; i < 4; ++i) {
				s += "  [ ";
				for (int j = 0; j < 4; ++j)  {
					s += m[i][j];
					if (j != 3){
						s += ", ";
					}
				}
				s += " ]\n";
			}
			s += " ] ";
			
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
		friend Matrix4x4 Inverse(const Matrix4x4 &);

		real_type m[4][4];
	};

} // rt3 namespace

#endif