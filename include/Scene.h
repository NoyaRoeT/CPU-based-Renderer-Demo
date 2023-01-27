#pragma once
#include "Model.h"
#include "Camera.h"

struct Scene
{
	Scene() : mCamera(1.0f, 1000.0f, 45.0f, 16.0f / 9.0f) {}

	void Update(float DeltaT);

	void AddModel(const Model& model);

	void ChangeCamera(Camera camera);

	std::vector<Model> mObjects;
	Camera mCamera;
};