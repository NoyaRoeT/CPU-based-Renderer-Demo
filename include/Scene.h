#pragma once
#include "Model.h"
#include "Camera.h"

struct Scene
{
	Scene() : mCamera(1.0f, 1000.0f, 45.0f, 16.0f / 9.0f)
	{
		Model temp;
		temp.LoadMesh("obj/CatStatue/Cats_obj.obj");
		temp.LoadAlbedoTexture("obj/CatStatue/Maps/Cats_dif.jpg", TextureType::RGB);
		temp.LoadNormalMap("obj/CatStatue/Maps/Cats_norm.jpg", TextureType::XYZ);
		temp.LoadSpecularMap("obj/CatStatue/Maps/Cats_gloss.jpg", TextureType::GRAY);
		temp.mTranslation = { 0.0f, -100.0f, -500.0f };
		temp.mRotationXYZ = {0.0f, 0.0f, 0.0f};
		temp.mScale = { 1.0f, 1.0f, 1.0f };
		mObjects.push_back(temp);
	}

	void Update(float DeltaT);

	std::vector<Model> mObjects;
	Camera mCamera; 
};