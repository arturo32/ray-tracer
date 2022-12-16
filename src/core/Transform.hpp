#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Matrix4x4.hpp"


namespace rt3 {

	class Transform {
	public:
		Transform();
		Transform(const real_type mat[4][4]);
		Transform(const Matrix4x4 &m) : m(m), mInv(Inverse(m)) { }
		Transform(const Matrix4x4 &m, const Matrix4x4 &mInv) 
			: m(m), mInv(mInv) {
		}

		string Print() const;

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
		bool operator==(const Transform &t) const;
		bool operator!=(const Transform &t) const;
		bool operator<(const Transform &t2) const;
		bool IsIdentity() const;
		const Matrix4x4 &GetMatrix() const;
		const Matrix4x4 &GetInverseMatrix() const;
		bool HasScale() const;

		// inline Point3i operator()(const Point3i &p) const;
		// inline Vector3i operator()(const Vector3i &v) const;
		// inline Normal3f operator()(const Normal3f &) const;
		// inline void operator()(const Normal3f &, Normal3f *nt) const;
		// inline Ray operator()(const Ray &r) const;
		// Bounds3f operator()(const Bounds3f &b) const;
		
		Transform operator*(const Transform &t2) const;

		// bool SwapsHandedness() const;
		// Surfel operator()(const Surfel &si) const;
		// inline Point3i operator()(const Point3i &pt, Vector3i *absError) const;
		// inline Point3i
		// operator()(const Point3i &p, const Vector3i &pError,
		// 			Vector3i *pTransError) const;
		// inline Vector3i
		// operator()(const Vector3i &v, Vector3i *vTransError) const;
		// inline Vector3i
		// operator()(const Vector3i &v, const Vector3i &vError,
		// 			Vector3i *vTransError) const;
		
		
		
		// inline Ray operator()(const Ray &r, Vector3f *oError,
		// 	Vector3f *dError) const;
		// inline Ray operator()(const Ray &r, const Vector3f &oErrorIn,
		// 						const Vector3f &dErrorIn, Vector3f *oErrorOut,
		// 						Vector3f *dErrorOut) const;

	private:

		Matrix4x4 m, mInv;
	};

} // rt3 namespace


#endif