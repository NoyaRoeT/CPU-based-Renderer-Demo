#pragma once
#include "Vector.h"
#include "Matrix.h"

#define M_PI 3.14159265358979323846
struct Camera
{
public:
	Camera(float n, float f, float vFOV = 90.0f, float AspectRatio = 4.0f / 3.0f)
	{
		float t, r;
		t = tan((vFOV * 0.5f) * (M_PI / 180.0f));
		r = t * AspectRatio;

		mPerspProjection = Matrix4f(1 / r, 0.0f, 0.0f, 0.0f,
			0.0f, 1 / t , 0.0f, 0.0f,
			0.0f, 0.0f, (f)/(n-f), (n*f)/(n-f),
			0.0f, 0.0f, -1.0f, 0.0f);

	}

	void ChangeCameraParams(float vFOV, float AspectRatio)
	{
		float t, r;
		t = tan((vFOV * 0.5f) * (M_PI / 180.0f));
		r = t * AspectRatio;

		mPerspProjection = Matrix4f(1 / r, 0.0f, 0.0f, 0.0f,
			0.0f, 1 / t, 0.0f, 0.0f,
			0.0f, 0.0f, 1, 0.0f,
			0.0f, 0.0f, -1.0f, 0.0f);
	}

	Matrix4f mPerspProjection;
};