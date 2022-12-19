#include "Transform.hpp"

namespace rt3 {
	Transform::Transform() { 
		m = Matrix4x4();
		mInv = Inverse(m);
	}
	Transform::Transform(const real_type mat[4][4]) {
		m = Matrix4x4(mat[0][0], mat[0][1], mat[0][2], mat[0][3],
					  mat[1][0], mat[1][1], mat[1][2], mat[1][3],
					  mat[2][0], mat[2][1], mat[2][2], mat[2][3],
					  mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
		mInv = Inverse(m);
	}
	string Transform::Print() const {
		return m.Print();
	}

	bool Transform::operator==(Transform &t) const {
		return t.m == m && t.mInv == mInv;
	}
	bool Transform::operator!=(Transform &t) const {
		return t.m != m || t.mInv != mInv;
	}
	bool Transform::operator<(Transform &t2) const {
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j) {
				if (m.m[i][j] < t2.m.m[i][j]) return true;
				if (m.m[i][j] > t2.m.m[i][j]) return false;
			}
		return false;
	}
	bool Transform::IsIdentity() const {
		return (m.m[0][0] == 1.f && m.m[0][1] == 0.f &&
				m.m[0][2] == 0.f && m.m[0][3] == 0.f &&
				m.m[1][0] == 0.f && m.m[1][1] == 1.f &&
				m.m[1][2] == 0.f && m.m[1][3] == 0.f &&
				m.m[2][0] == 0.f && m.m[2][1] == 0.f &&
				m.m[2][2] == 1.f && m.m[2][3] == 0.f &&
				m.m[3][0] == 0.f && m.m[3][1] == 0.f &&
				m.m[3][2] == 0.f && m.m[3][3] == 1.f);
	}
	const Matrix4x4& Transform::GetMatrix() const { return m; }
	const Matrix4x4& Transform::GetInverseMatrix() const { return mInv; }
	bool Transform::HasScale() const {
		real_type la2 = (Vector3f(1, 0, 0)).squared_length();
		real_type lb2 = (Vector3f(0, 1, 0)).squared_length();
		real_type lc2 = (Vector3f(0, 0, 1)).squared_length();
		#define NOT_ONE(x) ((x) < .999f || (x) > 1.001f)
			return (NOT_ONE(la2) || NOT_ONE(lb2) || NOT_ONE(lc2));
		#undef NOT_ONE
	}

	
	Transform Transform::operator*(Transform &t2) const {
		return Matrix4x4::Mul(m, t2.GetMatrix());
	}

} // rt3 namespace