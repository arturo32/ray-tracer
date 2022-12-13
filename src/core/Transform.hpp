#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "rt3-base.hpp"
#include "Matrix4x4.hpp"



namespace rt3 {

	class Transform {
	public:
		Transform() { 
			m = Matrix4x4();
			mInv = Inverse(m);
		}
		Transform(const real_type mat[4][4]) {
			m = Matrix4x4(mat[0][0], mat[0][1], mat[0][2], mat[0][3],
							mat[1][0], mat[1][1], mat[1][2], mat[1][3],
							mat[2][0], mat[2][1], mat[2][2], mat[2][3],
							mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
			mInv = Inverse(m);
		}
		Transform(const Matrix4x4 &m) : m(m), mInv(Inverse(m)) { }
		Transform(const Matrix4x4 &m, const Matrix4x4 &mInv) 
			: m(m), mInv(mInv) {
		}
		void Print(FILE *f) const;

		static Transform Translate(const Vector3f &delta) {
			Matrix4x4 m_t(1, 0, 0, delta.x(),
						0, 1, 0, delta.y(),
						0, 0, 1, delta.z(), 
						0, 0, 0,       1);
			Matrix4x4 minv_t(1, 0, 0, -delta.x(),
						0, 1, 0, -delta.y(),
						0, 0, 1, -delta.z(), 
						0, 0, 0,        1);
			return Transform(m_t, minv_t);
		}

		static Transform Scale(real_type x, real_type y, real_type z) {
			Matrix4x4 m_s(x, 0, 0, 0,
						0, y, 0, 0,
						0, 0, z, 0,
						0, 0, 0, 1);
			Matrix4x4 minv_s(1/x,   0,   0, 0,
							0,   1/y,   0, 0,
							0,     0, 1/z, 0,
							0,     0,   0, 1);
			return Transform(m_s, minv_s);
		}

		friend Transform Inverse(const Transform &t) {
			return Transform(t.mInv, t.m);
		}
		friend Transform Transpose(const Transform &t) {
			return Transform(Transpose(t.m), Transpose(t.mInv));
		}
		bool operator==(const Transform &t) const {
			return t.m == m && t.mInv == mInv;
		}
		bool operator!=(const Transform &t) const {
			return t.m != m || t.mInv != mInv;
		}
		bool operator<(const Transform &t2) const {
			for (int i = 0; i < 4; ++i)
				for (int j = 0; j < 4; ++j) {
					if (m.m[i][j] < t2.m.m[i][j]) return true;
					if (m.m[i][j] > t2.m.m[i][j]) return false;
				}
			return false;
		}
		bool IsIdentity() const {
			return (m.m[0][0] == 1.f && m.m[0][1] == 0.f &&
					m.m[0][2] == 0.f && m.m[0][3] == 0.f &&
					m.m[1][0] == 0.f && m.m[1][1] == 1.f &&
					m.m[1][2] == 0.f && m.m[1][3] == 0.f &&
					m.m[2][0] == 0.f && m.m[2][1] == 0.f &&
					m.m[2][2] == 1.f && m.m[2][3] == 0.f &&
					m.m[3][0] == 0.f && m.m[3][1] == 0.f &&
					m.m[3][2] == 0.f && m.m[3][3] == 1.f);
		}
		const Matrix4x4 &GetMatrix() const { return m; }
		const Matrix4x4 &GetInverseMatrix() const { return mInv; }
		bool HasScale() const {
			real_type la2 = (Vector3f(1, 0, 0)).squared_length();
			real_type lb2 = (Vector3f(0, 1, 0)).squared_length();
			real_type lc2 = (Vector3f(0, 0, 1)).squared_length();
		#define NOT_ONE(x) ((x) < .999f || (x) > 1.001f)
			return (NOT_ONE(la2) || NOT_ONE(lb2) || NOT_ONE(lc2));
		#undef NOT_ONE
		}
		// inline Point3i operator()(const Point3i &p) const;
		// inline Vector3i operator()(const Vector3i &v) const;
		// inline Normal3f operator()(const Normal3f &) const;
		// inline void operator()(const Normal3f &, Normal3f *nt) const;
		// inline Ray operator()(const Ray &r) const;
		// Bounds3f operator()(const Bounds3f &b) const;
		
		Transform operator*(const Transform &t2) const {
			return Matrix4x4::Mul(m, t2.GetMatrix());
		}

		bool SwapsHandedness() const;
		Surfel operator()(const Surfel &si) const;
		// inline Point3i operator()(const Point3i &pt, Vector3i *absError) const;
		// inline Point3i
		// operator()(const Point3i &p, const Vector3i &pError,
		// 			Vector3i *pTransError) const;
		// inline Vector3i
		// operator()(const Vector3i &v, Vector3i *vTransError) const;
		// inline Vector3i
		// operator()(const Vector3i &v, const Vector3i &vError,
		// 			Vector3i *vTransError) const;
		inline Ray operator()(const Ray &r, Vector3f *oError,
			Vector3f *dError) const;
		inline Ray operator()(const Ray &r, const Vector3f &oErrorIn,
								const Vector3f &dErrorIn, Vector3f *oErrorOut,
								Vector3f *dErrorOut) const;

	private:

		Matrix4x4 m, mInv;
	};

} // rt3 namespace


#endif