#pragma once
#include "Vector.h"

enum TextureType {
	RGB, XYZ, GRAY
};

struct Texture
{
public:
	Texture(const char* filename, TextureType Type);
	Vector3f GetTextureValue(float u, float v);
	float GetIntensityValue(float u, float v);
	~Texture() { delete[] PixelData; }

private:
	float* PixelData;
	int Width, Height, BytesPerChannel;
};