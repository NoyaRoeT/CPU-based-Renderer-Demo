#include "Scene.h"

void Scene::Update(float DeltaT)
{
	for (auto& obj : mObjects)
	{
		obj.mRotationXYZ.y += 0.0005 * DeltaT;
	}
}