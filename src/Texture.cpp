#include "Texture.h"
#include "../external/image.h"
#include <algorithm>

// Texture data is stored in [0, 1.0] range for RGB and GRAY,[-1, 1] range for XYZ values.
Texture::Texture(const char* filename, TextureType Type)
{
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filename, &Width, &Height, &BytesPerChannel, 0);
	int End = Width * Height * BytesPerChannel;
	PixelData = new float[End];
	if (Type == XYZ)
	{
		for (int i = 0; i != End; ++i)
		{
			PixelData[i] = (data[i] * (2.0f / 255.0f)) - 1.0f;
			
		}
	}
	else if (Type == RGB || Type == GRAY)
	{
		for (int i = 0; i != End; ++i)
		{
			PixelData[i] = (data[i] * (1.0f / 255.0f));
			
		}
	}
}

// Need to convert UV coords to Texel Coordinates
Vector3f Texture::GetTextureValue(float u, float v)
{
	
	int i = (int(u * Width)  + int(v * Height) * Width) * BytesPerChannel;
	return Vector3f(PixelData[i], PixelData[i + 1], PixelData[i + 2]);
}

float Texture::GetIntensityValue(float u, float v)
{
	int i = (int(u * Width) + int(v * Height) * Width) * BytesPerChannel;
	return PixelData[i] + PixelData[i + 1] + PixelData[i + 2];
}