#pragma once
#include "Vector.h"
#include "Matrix.h"
#include "Texture.h"

struct IShader
{
	
	virtual Vector4f Vertex(const Vector3f& Vertex, const Vector3f& Normal, const Vector3f& Tangent, int Index) = 0;
	virtual bool Fragment(const Vector3f& Barycentric, ColorRGB& Color) = 0;
};

//// Per-face lighting.
//struct FlatShader : IShader
//{
//	// Scene variables
//	Vector3f LightVector;
//
//	// Object variables
//	ColorRGB DiffuseColor;
//	Matrix4f ModelViewProjection;
//	Matrix4f ModelView;
//
//	// Output
//	Vector4f OutVertex;
//	float Intensity;
//	virtual Vector4f Vertex(const Vector3f& Vertex, const Vector3f& fNormal, const Vector3f& Tangent, int Index) override
//	{
//		OutVertex = MultiplyProjectionPoint(ModelViewProjection, Vertex);
//		Intensity = fmax(0.0f, Dot(fNormal, LightVector));
//		return OutVertex;
//	}
//
//	virtual bool Fragment(const Vector3f& Barycentric, ColorRGB& Color) override
//	{
//		Color = DiffuseColor * Intensity;
//		return false;
//	}
//};
//
//// Per-vertex lighting. Results of lighting computation is linearly interpolated.
//struct GouraudShader : IShader
//{
//	// Scene variables
//	Vector3f LightVector;
//
//	// Object variables
//	ColorRGB DiffuseColor;
//	Matrix4f ModelViewProjection;
//	Matrix4f ModelView;
//
//	// Per-Vertex Variables
//	Vector3f varying_intensity;
//
//	// Output
//	Vector4f OutVertex;
//	float Intensity;
//	virtual Vector4f Vertex(const Vector3f& Vertex, const Vector3f& vNormal, const Vector3f& Tangent, int Index) override
//	{
//		OutVertex = MultiplyProjectionPoint(ModelViewProjection, Vertex);
//		Vector3f n = MultiplyMatrixVector(ModelView, vNormal);
//		varying_intensity[Index] = fmax(0.0f, Dot(n, LightVector));
//		return OutVertex;
//	}
//
//	virtual bool Fragment(const Vector3f& Barycentric, ColorRGB& Color) override
//	{
//		Intensity = Dot(Barycentric, varying_intensity);
//		Color = DiffuseColor * Intensity;
//		return false;
//	}
//};
//
//// Per-fragment Lighting. Vertex normals are interpolated and used for lighting computation in fragment shader.
//struct PhongShader : IShader
//{
//	// Scene variables
//	Vector3f LightVector;
//
//	// Object variables
//	ColorRGB DiffuseColor;
//	ColorRGB AmbientColor;
//	ColorRGB SpecularColor;
//	Matrix4f ModelViewProjection;
//	Matrix4f ModelView;
//
//	// Per-Vertex Variables
//	Vector3f varying_intensity;
//	Vector3f varying_norm[3];
//	Vector3f varying_viewdir[3];
//
//	// Per-fragment Variables
//	ColorRGB Diffuse, Specular, Ambient;
//
//	// Output
//	Vector4f OutVertex;
//	float Intensity;
//
//	virtual Vector4f Vertex(const Vector3f& Vertex, const Vector3f& vNormal, const Vector3f& Tangent, int Index) override
//	{
//		OutVertex = MultiplyProjectionPoint(ModelViewProjection, Vertex);
//		varying_norm[Index] = MultiplyMatrixVector(ModelView, vNormal);
//		varying_viewdir[Index] = -MultiplyMatrixPoint(ModelView, Vertex);
//		return OutVertex;
//	}
//
//	virtual bool Fragment(const Vector3f& Barycentric, ColorRGB& Color) override
//	{
//		Vector3f n = Normalize(varying_norm[0] + (varying_norm[1] - varying_norm[0]) * Barycentric.y + (varying_norm[2] - varying_norm[0]) * Barycentric.z);
//		Vector3f ViewDir = Normalize(varying_viewdir[0] + (varying_viewdir[1] - varying_viewdir[0]) * Barycentric.y + (varying_viewdir[2] - varying_viewdir[0]) * Barycentric.z);
//		
//
//		// Diffuse Component;
//		Intensity = fmax(0.0f, Dot(n, LightVector));
//		Diffuse = DiffuseColor * Intensity;
//
//		// Specular Component;
//		Vector3f ReflectDir = Reflect(n, LightVector);
//		Intensity = std::pow(fmax(0.0f, Dot(ViewDir, ReflectDir)), 20.0f);
//		
//		Specular = SpecularColor * Intensity;
//
//		Color = 0.2f * AmbientColor + 0.8f * Diffuse + 0.4f * Specular;
//		return false;
//	}
//};
//
//// Similar to Phong Shader, but uses a half-vector rather than a reflection vector.
//struct BlinnPhongShader : IShader
//{
//	// Scene variables
//	Vector3f LightVector;
//
//	// Object variables
//	ColorRGB DiffuseColor;
//	ColorRGB AmbientColor;
//	ColorRGB SpecularColor;
//	Matrix4f ModelViewProjection;
//	Matrix4f ModelView;
//	Texture* AlbedoMap;
//
//	// Per-Vertex Variables
//	Vector3f varying_intensity;
//	Vector3f varying_norm[3];
//	Vector3f varying_viewdir[3];
//	Vector3f varying_uv[3];
//
//	// Per-fragment Variables
//	ColorRGB Diffuse, Specular, Ambient;
//
//	// Output
//	Vector4f OutVertex;
//	float Intensity;
//
//	virtual Vector4f Vertex(const Vector3f& Vertex, const Vector3f& vNormal, const Vector3f& Tangent, int Index) override
//	{
//		Vector4f OutVertex = MultiplyProjectionPoint(ModelViewProjection, Vertex);
//		varying_norm[Index] = MultiplyMatrixVector(ModelView, vNormal);
//		varying_viewdir[Index] = -MultiplyMatrixPoint(ModelView, Vertex);
//		return OutVertex;
//	}
//
//	virtual bool Fragment(const Vector3f& Barycentric, ColorRGB& Color) override
//	{
//		Vector3f n = Normalize(varying_norm[0] + (varying_norm[1] - varying_norm[0]) * Barycentric.y + (varying_norm[2] - varying_norm[0]) * Barycentric.z);
//		Vector3f ViewDir = varying_viewdir[0] + (varying_viewdir[1] - varying_viewdir[0]) * Barycentric.y + (varying_viewdir[2] - varying_viewdir[0]) * Barycentric.z;
//		Vector3f UV = varying_uv[0] + (varying_uv[1] - varying_uv[0]) * Barycentric.y + (varying_uv[2] - varying_uv[0]) * Barycentric.z;
//
//		// Diffuse Component;
//		Intensity = fmax(0.0f, Dot(n, LightVector));
//		Diffuse = AlbedoMap->GetTextureValue(UV.x, UV.y) * 255.0f * Intensity;
//
//		// Specular Component;
//		Vector3f HalfVector = Normalize(ViewDir + LightVector);
//		Intensity = std::pow(fmax(0.0f, Dot(HalfVector, n)), 200.0f);
//		Specular = SpecularColor * Intensity;
//
//		Color = 0.2f * AmbientColor + 0.8f * Diffuse + 0.2f * Specular;
//		return false;
//	}
//};

// Shader that uses diffuse textures, object space normal mapping, specular maps.
struct TextureShader : IShader
{
	// Scene variables
	Vector3f LightVector;

	// Object variables
	ColorRGB DiffuseColor;
	ColorRGB AmbientColor;
	ColorRGB SpecularColor;
	Matrix4f ModelViewProjection;
	Matrix4f ModelView;
	Matrix4f N;
	Texture* AlbedoMap = nullptr;
	Texture* NormalMap = nullptr;
	Texture* GlossMap = nullptr;

	// Per-Vertex Variables
	Vector3f varying_intensity;
	Vector3f varying_norm[3];
	Vector3f varying_viewdir[3];
	Vector3f varying_uv[3];
	Vector3f varying_lightdir[3];
	Matrix4f TBN;
	Vector3f t, b, n;

	// Per-fragment Variables
	ColorRGB Diffuse, Specular, Ambient;
	Vector3f ViewDir, HalfVector, LightDir, UV, fragNormal;

	// Output
	Vector4f OutVertex;
	float Intensity;

	virtual Vector4f Vertex(const Vector3f& Vertex, const Vector3f& vNormal, const Vector3f& Tangent, int Index) override
	{
		Vector4f OutVertex = MultiplyProjectionPoint(ModelViewProjection, Vertex);

		// Transform vectors from model to view space.
		n = MultiplyMatrixVector(N, vNormal);
		t = MultiplyMatrixVector(N, Tangent);
		b = Cross(n, t);
		TBN = Transpose(Matrix4f(t, b, n)); //View space to Tangent space

		varying_lightdir[Index] = MultiplyMatrixVector(TBN, LightVector);
		varying_viewdir[Index] = MultiplyMatrixVector(TBN, -MultiplyMatrixVector(ModelView, Vertex));
		return OutVertex;
	}

	virtual bool Fragment(const Vector3f& Barycentric, ColorRGB& Color) override
	{
		LightDir = varying_lightdir[0] + (varying_lightdir[1] - varying_lightdir[0]) * Barycentric.y + (varying_lightdir[2] - varying_lightdir[0]) * Barycentric.z;
		ViewDir = varying_viewdir[0] + (varying_viewdir[1] - varying_viewdir[0]) * Barycentric.y + (varying_viewdir[2] - varying_viewdir[0]) * Barycentric.z;
		UV = varying_uv[0] + (varying_uv[1] - varying_uv[0]) * Barycentric.y + (varying_uv[2] - varying_uv[0]) * Barycentric.z;
		fragNormal = Normalize(NormalMap->GetTextureValue(UV.x, UV.y));

		// Diffuse Component;
		Intensity = fmax(0.0f, Dot(fragNormal, LightDir));
		Diffuse = AlbedoMap->GetTextureValue(UV.x, UV.y) * 255.0f * Intensity;

		// Specular Component;
		HalfVector = Normalize(ViewDir + LightDir);
		Intensity = std::pow(fmax(0.0f, Dot(HalfVector, fragNormal)), GlossMap->GetIntensityValue(UV.x, UV.y) * 255.0f);
		
		Specular = SpecularColor * Intensity;

		Color = 0.2f * AmbientColor + 1.0f * Diffuse + 0.0f * Specular;
		return false;
	}
};