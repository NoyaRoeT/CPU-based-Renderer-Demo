#pragma once
#include "Buffer.h"
#include "Vector.h"
#include <cstdint>
#include "Shader.h"

struct SDL_PixelFormat;

class Rasterizer
{
public:
	//Draws a line using Bresenham's Line Drawing algorithms that uses only integer arithmetic
	static void DrawLine(const Vector4f& v0, const Vector4f& v1, const uint32_t& Color, Buffer<uint32_t>& FrameBuffer);

	//Draws a wireframe triangle by calling DrawLine function for each edge
	static void DrawWireFrame(Vector4f* Vertices, Buffer<uint32_t>& FrameBuffer);

	static float EdgeFunction(const Vector4f& a, const Vector4f& b, const Vector4f& c);

	static float Clamp(float x, float lower, float upper);

	static void DrawTriangle(Vector4f* Vertices, IShader& Shader, Buffer<uint32_t>& FrameBuffer, Buffer<float>& ZBuffer);

	static const SDL_PixelFormat* MappingFormat;

};