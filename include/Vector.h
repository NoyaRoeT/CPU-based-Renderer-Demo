#pragma once
#include <cmath>
#include <iostream>

template<typename T>
struct Vector3 
{
	Vector3() = default;
	Vector3(T a, T b, T c) : x(a), y(b), z(c) {}

	Vector3 operator-() { return Vector3(-x, -y, -z); }
	Vector3& operator+=(T s) { x += s; y += s; z += s; return *this; }
	Vector3& operator += (const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
	Vector3& operator-=(T s) { x -= s; y -= s; z -= s; return *this; }
	Vector3& operator *= (T s) { x *= s; y *= s; z *= s; return *this; }
	Vector3& operator /= (T s) { s = 1.0f / s; x *= s; y *= s; z *= s; return *this; }

	T& operator[](int i) { return (&x)[i]; }
	const T& operator[](int i) const { return (&x)[i]; }
public:
	T x, y, z;
};

template <typename T>
std::ostream& operator << (std::ostream& os, const Vector3<T> v)
{
	os << v.x << ' ' << v.y << ' ' << v.z;
	return os;
}

template <typename T>
inline Vector3<T> operator*(const Vector3<T>& v, T s)
{
	return Vector3<T>(v.x * s, v.y * s, v.z * s);
}

template <typename T>
inline Vector3<T> operator*(T s, const Vector3<T>& v)
{
	return Vector3<T>(v.x * s, v.y * s, v.z * s);
}

template <typename T>
inline Vector3<T> operator/(T s, const Vector3<T>& v)
{
	s = 1 / s;
	return Vector3<T>(v.x * s, v.y * s, v.z * s);
}

template <typename T>
inline Vector3<T> operator/(const Vector3<T>& v, T s)
{
	s = 1 / s;
	return Vector3<T>(v.x * s, v.y * s, v.z * s);
}

template <typename T>
inline Vector3<T> operator + (const Vector3<T>&a, const Vector3<T>&b)
{
	return Vector3<T>(a.x + b.x, a.y + b.y, a.z + b.z);
}

template <typename T>
inline Vector3<T> operator - (const Vector3<T>& a, const Vector3<T>& b)
{
	return Vector3<T>(a.x - b.x, a.y - b.y, a.z - b.z);
}

template<typename T>
inline T Magnitude(const Vector3<T>& v)
{
	return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

template<typename T>
inline Vector3<T> Normalize(const Vector3<T>& v)
{
	return (v / Magnitude(v));
}

template <typename T>
inline T Dot(const Vector3<T>& a, const Vector3<T>& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

template <typename T>
inline Vector3<T> Cross(const Vector3<T>& a, const Vector3<T>& b)
{
	return Vector3<T>(a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x);
}

template <typename T>
inline Vector3<T> Reflect(const Vector3<T>& v, const Vector3<T>& n)
{
	return 2 * Dot(v, n) * n - v;
}

template <typename T>
inline Vector3<T> Reject(const Vector3<T>& a, const Vector3<T>& b)
{
	return a - Dot(a, b) * b;
}

typedef Vector3<float> Vector3f;
typedef Vector3<int> Vector3i;

typedef Vector3<float> ColorRGB;

template <typename T>
using Point3 = Vector3<T>;
using Point3f = Point3 <float>;
using Point3i = Point3<int>;

template<typename T>
struct Vector4
{
	Vector4() = default;
	Vector4(T a, T b, T c, T d) : x(a), y(b), z(c), w(d) {}

	Vector4 operator-() { return Vector4(-x, -y, -z, -w); }
	Vector4& operator+=(T s) { x += s; y += s; z += s; w += s; return *this; }
	Vector4& operator-=(T s) { x -= s; y -= s; z -= s; w -= s; return *this; }
	Vector4& operator *= (T s) { x *= s; y *= s; z *= s; w *= s; return *this; }
	Vector4& operator /= (T s) { s = 1 / s; x *= s; y *= s; z *= s; w *= s; return *this; }

	T& operator[](int i) { return (&x)[i]; }
	const T& operator[](int i) const { return (&x)[i]; }
public:
	T x, y, z, w;
};

template <typename T>
std::ostream& operator << (std::ostream& os, const Vector4<T> v)
{
	os << v.x << ' ' << v.y << ' ' << v.z << ' ' << v.w;
	return os;
}

template <typename T>
inline Vector4<T> operator*(const Vector4<T>& v, T s)
{
	return Vector4<T>(v.x * s, v.y * s, v.z * s, v.w * s);
}

template <typename T>
inline Vector4<T> operator*(T s, const Vector4<T>& v)
{
	return Vector3<T>(v.x * s, v.y * s, v.z * s, v.w * s);
}

template <typename T>
inline Vector4<T> operator/(T s, const Vector4<T>& v)
{
	s = 1.0f / s;
	return Vector4<T>(v.x * s, v.y * s, v.z * s, v.w * s);
}

template <typename T>
inline Vector4<T> operator/(const Vector4<T>& v, T s)
{
	s = 1.0f / s;
	return Vector4<T>(v.x * s, v.y * s, v.z * s, v.w * s);
}

template <typename T>
inline Vector4<T> operator + (const Vector4<T>& a, const Vector4<T>& b)
{
	return Vector4<T>(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

template <typename T>
inline Vector4<T> operator - (const Vector4<T>& a, const Vector4<T>& b)
{
	return Vector4<T>(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

template<typename T>
inline T Magnitude(const Vector4<T>& v)
{
	return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w));
}

template<typename T>
inline Vector4<T> Normalize(const Vector4<T>& v)
{
	if (v.w != 0) return v / v.w;
}

template <typename T>
inline T Dot(const Vector4<T>& a, const Vector4<T>& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

// Treats it as a 3D Vector
template <typename T>
inline Vector3<T> Cross(const Vector4<T>& a, const Vector4<T>& b)
{
	return Vector3<T>(a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x);
}

typedef Vector4<float> Vector4f;
typedef Vector4<int> Vector4i;

template <typename T>
using Point4 = Vector4<T>;
using Point4f = Point4 <float>;
using Point4i = Point4<int>;