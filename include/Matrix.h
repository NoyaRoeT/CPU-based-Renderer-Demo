#pragma once
#include "Vector.h"

// A simple 4x4 Matrix class that uses column-major order.
// Although, might actually be better to use row-major order for column vector notation.
template <typename T>
struct Matrix4
{
public:
	Matrix4() = default;
	Matrix4(float n00, float n01, float n02, float n03,
		float n10, float n11, float n12, float n13,
		float n20, float n21, float n22, float n23,
		float n30, float n31, float n32, float n33)
	{
		m[0][0] = n00; m[0][1] = n10; m[0][2] = n20; m[0][3] = n30;
		m[1][0] = n01; m[1][1] = n11; m[1][2] = n21; m[1][3] = n31;
		m[2][0] = n02; m[2][1] = n12; m[2][2] = n22; m[2][3] = n32;
		m[3][0] = n03; m[3][1] = n13; m[3][2] = n23; m[3][3] = n33;
	}
	Matrix4(const Vector4<T>& a, const Vector4<T>& b, const Vector4<T>& c, const Vector4<T>& d)
	{
		m[0][0] = a.x; m[0][1] = a.y; m[0][2] = a.z; m[0][3] = a.w;
		m[1][0] = b.x; m[1][1] = b.y; m[1][2] = b.z; m[1][3] = b.w;
		m[2][0] = c.x; m[2][1] = c.y; m[2][2] = c.z; m[2][3] = c.w;
		m[3][0] = d.x; m[3][1] = d.y; m[3][2] = d.z; m[3][3] = d.w;
	}
	Matrix4(const Vector3<T>& a, const Vector3<T>& b, const Vector3<T>& c)
	{
		m[0][0] = a.x; m[0][1] = a.y; m[0][2] = a.z; m[0][3] = 0;
		m[1][0] = b.x; m[1][1] = b.y; m[1][2] = b.z; m[1][3] = 0;
		m[2][0] = c.x; m[2][1] = c.y; m[2][2] = c.z; m[2][3] = 0;
		m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
	}

	void SetTranslation(T a, T b, T c) { m[3][0] = a; m[3][1] = b, m[3][2] = c; }
	void SetTranslation(const Vector3<T>& v) { m[3][0] = v.x; m[3][1] = v.y, m[3][2] = v.z; }

	T& operator()(int i, int j) { return m[j][i]; }
	const T& operator()(int i, int j) const { return m[j][i]; }

	Vector4<T>& operator[] (int i) { return (*reinterpret_cast<Vector4<T>*>(m[i])); }
	const Vector4<T>& operator[] (int i) const { return (*reinterpret_cast<const Vector4<T>*>(m[i])); }

public:
	T m[4][4];
};

template <typename T>
Matrix4<T> operator* (const Matrix4<T>& a, const Matrix4<T>& b)
{
	return Matrix4<T>(a[0][0] * b[0][0] + a[1][0] * b[0][1] + a[2][0] * b[0][2] + a[3][0] * b[0][3],
		a[0][0] * b[1][0] + a[1][0] * b[1][1] + a[2][0] * b[1][2] + a[3][0] * b[1][3],
		a[0][0] * b[2][0] + a[1][0] * b[2][1] + a[2][0] * b[2][2] + a[3][0] * b[2][3],
		a[0][0] * b[3][0] + a[1][0] * b[3][1] + a[2][0] * b[3][2] + a[3][0] * b[3][3],
		a[0][1] * b[0][0] + a[1][1] * b[0][1] + a[2][1] * b[0][2] + a[3][1] * b[0][3],
		a[0][1] * b[1][0] + a[1][1] * b[1][1] + a[2][1] * b[1][2] + a[3][1] * b[1][3],
		a[0][1] * b[2][0] + a[1][1] * b[2][1] + a[2][1] * b[2][2] + a[3][1] * b[2][3],
		a[0][1] * b[3][0] + a[1][1] * b[3][1] + a[2][1] * b[3][2] + a[3][1] * b[3][3],
		a[0][2] * b[0][0] + a[1][2] * b[0][1] + a[2][2] * b[0][2] + a[3][2] * b[0][3],
		a[0][2] * b[1][0] + a[1][2] * b[1][1] + a[2][2] * b[1][2] + a[3][2] * b[1][3],
		a[0][2] * b[2][0] + a[1][2] * b[2][1] + a[2][2] * b[2][2] + a[3][2] * b[2][3],
		a[0][2] * b[3][0] + a[1][2] * b[3][1] + a[2][2] * b[3][2] + a[3][2] * b[3][3],
		a[0][3] * b[0][0] + a[1][3] * b[0][1] + a[2][3] * b[0][2] + a[3][3] * b[0][3],
		a[0][3] * b[1][0] + a[1][3] * b[1][1] + a[2][3] * b[1][2] + a[3][3] * b[1][3],
		a[0][3] * b[2][0] + a[1][3] * b[2][1] + a[2][3] * b[2][2] + a[3][3] * b[2][3],
		a[0][3] * b[3][0] + a[1][3] * b[3][1] + a[2][3] * b[3][2] + a[3][3] * b[3][3]);
}

template <typename T>
Vector4<T> operator* (const Matrix4<T>& m, const Vector4<T>& v)
{
	return Vector4<T>(m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0] * v.w,
		m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1] * v.w,
		m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2] * v.w,
		m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3] * v.w);
}

template <typename T>
Vector4<T> operator* (const Vector4<T>& v, const Matrix4<T>& m)
{
	return Vector4<T>(v.x * m[0][0] + v.y * m[0][1] + v.z * m[0][2] + v.w * m[0][3],
		v.x * m[1][0] + v.y * m[1][1] + v.z * m[1][2] + v.w * m[1][3],
		v.x * m[2][0] + v.y * m[2][1] + v.z * m[2][2] + v.w * m[2][3],
		v.x * m[3][0] + v.y * m[3][1] + v.z * m[3][2] + v.w * m[3][3]);
}

template <typename T>
Vector3<T> MultiplyMatrixVector(const Matrix4<T>& m, const Vector3<T>& v)
{
	return Vector3<T>(m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z,
		m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z ,
		m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z);
}

template <typename T>
Vector3<T> MultiplyVectorMatrix(const Matrix4<T>& m, const Vector3<T>& v)
{
	return Vector3<T>(m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
		m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
		m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z);
}

template <typename T>
Vector3<T> MultiplyMatrixPoint(const Matrix4<T>& m, const Point3<T>& v)
{
	return Vector3<T>(m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0],
		m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1],
		m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2]);
}

template <typename T>
Vector3<T> MultiplyPointMatrix(const Point3<T>& v, const Matrix4<T>& m)
{
	return Vector3<T>(v.x * m[0][0] + v.y * m[0][1] + v.z * m[0][2] + m[0][3],
		v.x * m[1][0] + v.y * m[1][1] + v.z * m[1][2] + m[1][3],
		v.x * m[2][0] + v.y * m[2][1] + v.z * m[2][2] + m[2][3]);
}

template <typename T>
Matrix4<T> Transpose(const Matrix4<T>& m)
{
	Matrix4<T> Transposed;
	for (int i = 4; i--;) {
		for (int j = 4; j--;)
		{
			Transposed[i][j] = m[j][i];
		}
	}

	return Transposed;
}

template <typename T>
Matrix4<T> Inverse(const Matrix4<T>& M)
{
	const Vector3<T>& a = reinterpret_cast<const Vector3<T>&>(M[0]);
	const Vector3<T>& b = reinterpret_cast<const Vector3<T>&>(M[1]);
	const Vector3<T>& c = reinterpret_cast<const Vector3<T>&>(M[2]);
	const Vector3<T>& d = reinterpret_cast<const Vector3<T>&>(M[3]);

	const float& x = M(3, 0);
	const float& y = M(3, 1);
	const float& z = M(3, 2);
	const float& w = M(3, 3);

	Vector3<T> s = Cross(a, b);
	Vector3<T> t = Cross(c, d);
	Vector3<T> u = a * y - b * x;
	Vector3<T> v = c * w - d * z;

	float invDet = 1.0F / (Dot(s, v) + Dot(t, u));
	s *= invDet;
	t *= invDet;
	u *= invDet;
	v *= invDet;

	Vector3<T> r0 = Cross(b, v) + t * y;
	Vector3<T> r1 = Cross(v, a) - t * x;
	Vector3<T> r2 = Cross(d, u) + s * w;
	Vector3<T> r3 = Cross(u, c) - s * z;

	return (Matrix4<T>(r0.x, r0.y, r0.z, -Dot(b, t),
		r1.x, r1.y, r1.z, Dot(a, t),
		r2.x, r2.y, r2.z, -Dot(d, s),
		r3.x, r3.y, r3.z, Dot(c, s)));
}

typedef Matrix4<float> Matrix4f;

// Transforms:
inline Matrix4f MakeRotationX(float t)
{
	float c = cos(t);
	float s = sin(t);
	return Matrix4f(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, c, -s, 0.0f,
		0.0f, s, c, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

inline Matrix4f MakeRotationY(float t)
{
	float c = cos(t);
	float s = sin(t);
	return Matrix4f(c, 0.0f, s, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-s, 0.0f, c, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

inline Matrix4f MakeRotationZ(float t)
{
	float c = cos(t);
	float s = sin(t);
	return Matrix4f(c, -s, 0.0f, 0.0f,
		s, c, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

// Rotation Matrix that applies rotation in the orderXYZ
inline Matrix4f MakeRotationZYX(float alpha, float beta, float gamma) {
	Matrix4f rotMat;
	float cosA = std::cos(alpha);
	float sinA = std::sin(alpha);

	float cosB = std::cos(beta);
	float sinB = std::sin(beta);

	float cosG = std::cos(gamma);
	float sinG = std::sin(gamma);

	//First row
	rotMat(0, 0) = cosB * cosG;
	rotMat(0, 1) = -cosA * sinG + sinA * sinB * cosG;
	rotMat(0, 2) = sinA * sinG + cosA * sinB * cosG;

	//Second row
	rotMat(1, 0) = cosB * sinG;
	rotMat(1, 1) = cosA * cosG + sinA * sinB * sinG;
	rotMat(1, 2) = -sinA * cosG + cosA * sinB * sinG;

	//Third row
	rotMat(2, 0) = -sinB;
	rotMat(2, 1) = sinA * cosB;
	rotMat(2, 2) = cosA * cosB;

	//Fourth row
	rotMat(3, 0) = 0;
	rotMat(3, 1) = 0;
	rotMat(3, 2) = 0;
	rotMat(3, 3) = 1;

	return rotMat;
}

inline Matrix4f MakeScale(float sx, float sy, float sz)
{
	return Matrix4f(sx, 0.0f, 0.0f, 0.0f,
		0.0f, sy, 0.0f, 0.0f,
		0.0f, 0.0f, sz, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

template <typename T>
Matrix4f MakeScale(float s, const Vector3<T> a)
{
	s -= 1.0F;
	float x = a.x * s;
	float y = a.y * s;
	float z = a.z * s;
	float axay = x * a.y;
	float axaz = x * a.z;
	float ayaz = y * a.z;

	return (Matrix4f(x * a.x + 1.0f, axay, axaz, 0.0f,
		axay, y * a.y + 1.0f, ayaz, 0.0f,
		axaz, ayaz, z * a.z + 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f));
}

// This assumes an OpenGL Projection Matrix
inline Point4f MultiplyProjectionPoint(const Matrix4f& m, const Point3f& v)
{
	return Point4f(m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0],
		m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1],
		m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2],
		m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3]);
}