#pragma once
#include "Mesh.h"
#include "Matrix.h"
#include "Texture.h"

class Model
{
public:
	Model() {}

	void LoadMesh(const char* filename);
	void LoadAlbedoTexture(const char* filename, TextureType Type = TextureType::RGB);
	void LoadNormalMap(const char* filename, TextureType Type = TextureType::XYZ);
	void LoadSpecularMap(const char* filename, TextureType Type = TextureType::GRAY);
	Matrix4f GetModelWorldMatrix() const;

	
public:
	Vector3f mTranslation = { 0.0f, 0.0f, 0.0f };
	Vector3f mRotationXYZ = { 0.0f, 0.0f, 0.0f };
	Vector3f mScale = { 1.0f, 1.0f, 1.0f };
	Mesh mMesh;

	Texture* mAlbedo = nullptr;
	Texture* mNormal = nullptr;
	Texture* mGloss = nullptr;

};