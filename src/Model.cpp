#include "Model.h"

void Model::LoadMesh(const char* filename)
{
	mMesh.LoadObj(filename);
}

Matrix4f Model::GetModelWorldMatrix() const
{
	Matrix4f Transform = MakeRotationZYX(mRotationXYZ.x, mRotationXYZ.y, mRotationXYZ.z) * MakeScale(mScale.x, mScale.y, mScale.z);
	Transform.SetTranslation(mTranslation);
	return Transform;
}

void Model::LoadAlbedoTexture(const char* filename, TextureType Type)
{
	mAlbedo = new Texture(filename, Type);
}

void Model::LoadNormalMap(const char* filename, TextureType Type)
{
	mNormal = new Texture(filename, Type);
}
void Model::LoadSpecularMap(const char* filename, TextureType Type)
{
	mGloss = new Texture(filename, Type);
}