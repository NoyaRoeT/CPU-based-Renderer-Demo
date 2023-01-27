#include "Scene.h"

void Scene::AddModel(const Model& model) {
	mObjects.push_back(model);
}

void Scene::ChangeCamera(Camera camera) {
	mCamera = camera;
}

void Scene::Update(float DeltaT)
{
	for (auto& obj : mObjects)
	{
		obj.mRotationXYZ.y += 0.0005 * DeltaT;
	}
}