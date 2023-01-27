#include "Rasterizer.h"
#include <utility>
#include <SDL.h>
#include <limits>

const SDL_PixelFormat* Rasterizer::MappingFormat(SDL_AllocFormat(SDL_PIXELFORMAT_RGB888));

void Rasterizer::DrawLine(const Vector4f& v0, const Vector4f& v1, const uint32_t& Color, Buffer<uint32_t>& FrameBuffer)
{
	int x0 = v0.x; int x1 = v1.x;
	int y0 = v0.y; int y1 = v1.y;

	//If line is steep, swap x and y values so we loop through y.
	//This guarantees no vertical gaps when dy is large.
	bool steep = false;
	if (fabs(x1 - x0) < fabs(y1 - y0))
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}

	//Ensure left to right
	if (x1 < x0) { std::swap(x1, x0); std::swap(y1, y0); }

	int dx = (x1 - x0);
	int dy = (y1 - y0);
	int derror = std::abs(dy) * 2;
	int error = 0;
	int y = y0;
	for (int x = x0; x <= x1; ++x)
	{
		if (steep)
		{
			FrameBuffer(y, x) = Color;
		}
		else
		{
			FrameBuffer(x, y) = Color;
		}
		error += derror;
		if (error > dx)
		{
			y += (y1 > y0) ? 1 : -1;
			error -= 2 * dx;
		}
	}
}

void Rasterizer::DrawWireFrame(Vector4f* Vertices, Buffer<uint32_t>& FrameBuffer)
{
	for (int i = 0; i != 3; ++i)
	{
		Vertices[i] = Vertices[i] / Vertices[i].w;
	}

	for (int i = 0; i != 3; ++i)
	{
		Vertices[i].x = ((Vertices[i].x + 1.0f) * 0.5f) * FrameBuffer.mWidth;
		Vertices[i].y = ((-Vertices[i].y + 1.0f) * 0.5f) * FrameBuffer.mHeight;
	}

	DrawLine(Vertices[0], Vertices[1], SDL_MapRGB(MappingFormat, 255, 0, 0), FrameBuffer);
	DrawLine(Vertices[1], Vertices[2], SDL_MapRGB(MappingFormat, 0, 255, 0), FrameBuffer);
	DrawLine(Vertices[2], Vertices[0], SDL_MapRGB(MappingFormat, 0, 0, 255), FrameBuffer);
}

float Rasterizer::EdgeFunction(const Vector4f& a, const Vector4f& b, const Vector4f& c)
{
	return  (b.y - a.y) * (c.x - a.x) - (b.x - a.x) * (c.y - a.y);
}

float Rasterizer::Clamp(float x, float lower, float upper)
{
	if (x < lower) return lower;
	if (x > upper) return upper;
	return x;
}

void Rasterizer::DrawTriangle(Vector4f* Vertices, IShader& Shader, Buffer<uint32_t>& FrameBuffer, Buffer<float>& ZBuffer)
{
	Vector3f ReciprocalZs;
	//Viewport Transformation
	// Get ReciprocalZ values at each vertex for Perspective-Correct Interpolation
	for (int i = 0; i != 3; ++i)
	{
		Vertices[i].x = ((Vertices[i].x + 1.0f) * 0.5f) * FrameBuffer.mWidth;
		Vertices[i].y = ((-Vertices[i].y + 1.0f) * 0.5f) * FrameBuffer.mHeight;
		ReciprocalZs[i] = 1.0f / Vertices[i].w;
	}
	
	// Computing the triangle bounding box
	Vector3f Bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), 0.0f);
	Vector3f Bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), 0.0f);
	Vector3i Clamp(FrameBuffer.mWidth - 1, FrameBuffer.mHeight - 1, 0.0f);

	for (int i = 0; i != 3; ++i) {
		for (int j = 0; j != 2; ++j)
		{
			Bboxmin[j] = fmax(0, fmin(Bboxmin[j], (int)Vertices[i][j]));
			Bboxmax[j] = fmin(Clamp[j], fmax(Bboxmax[j],(int) Vertices[i][j]));
		}
	}

	float Area = EdgeFunction(Vertices[0], Vertices[1], Vertices[2]);
	Area = 1.0f / Area;

	Vector4f PixelSample(Bboxmin.x + 0.5f,Bboxmin.y + 0.5f, 0.0f, 0.0f);
	float RowE0 = EdgeFunction(Vertices[1], Vertices[2], PixelSample);
	float RowE1 = EdgeFunction(Vertices[2], Vertices[0], PixelSample);
	float RowE2 = EdgeFunction(Vertices[0], Vertices[1], PixelSample);
	float IncrColE0 = Vertices[2].y - Vertices[1].y; float IncrRowE0 = Vertices[2].x - Vertices[1].x;
	float IncrColE1 = Vertices[0].y - Vertices[2].y; float IncrRowE1 = Vertices[0].x - Vertices[2].x;
	float IncrColE2 = Vertices[1].y - Vertices[0].y; float IncrRowE2 = Vertices[1].x - Vertices[0].x;
	Vector3f e;
	Vector3f Barycentric;
	
	for (int y = Bboxmin.y; y <= Bboxmax.y; ++y) {
		
		e.x = RowE0; e.y = RowE1; e.z = RowE2;
		for (int x = Bboxmin.x; x <= Bboxmax.x; ++x)
		{
  			if (e.x >= 0 && e.y >= 0 && e.z >= 0)
			{
				//Compute Barycentric Coordinates for this pixel
				Barycentric.x = e.x * Area;
				Barycentric.y = e.y * Area;
				Barycentric.z = e.z * Area;
			
				// Linearly Interplate ReciprocalZ values
				float invZ = Barycentric.x * ReciprocalZs[0] + Barycentric.y * ReciprocalZs[1] + Barycentric.z * ReciprocalZs[2];
				//Depth test
				if (ZBuffer(x, y) < invZ)
				{
					
					ZBuffer(x, y) = invZ;

					// Perspective Correct Barycentric Coordinates
					float z = 1 / invZ;
					Barycentric.y = z * Barycentric.y * ReciprocalZs[1];
					Barycentric.z = z * Barycentric.z * ReciprocalZs[2];
					Barycentric.x = 1 - Barycentric.y - Barycentric.z;

					ColorRGB c;
					Shader.Fragment(Barycentric, c);
					FrameBuffer(x, y) = SDL_MapRGB(MappingFormat, c.x, c.y, c.z);
				}
			}
			e.x += IncrColE0;
			e.y += IncrColE1;
			e.z += IncrColE2;
		}
		RowE0 -= IncrRowE0;
		RowE1 -= IncrRowE1;
		RowE2 -= IncrRowE2;
	}
}