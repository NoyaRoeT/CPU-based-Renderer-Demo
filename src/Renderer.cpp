#include "Renderer.h"
#include "Shader.h"
#include "Rasterizer.h"

void Renderer::CreateBuffers(int Width, int Height)
{
	FrameBuffer = std::make_unique<Buffer<uint32_t>>(Width, Height);
	ZBuffer = std::make_unique<Buffer<float>>(Width, Height);
}

void Renderer::ClearBuffers()
{
	FrameBuffer->Clear(0);
	ZBuffer->Clear(1e-8);
}

//void Renderer::RenderObject(const Model& Object, const Camera& camera)
//{
//	ClearBuffers();
//	FlatShader mShader;
//	// Get per object variables
//	mShader.DiffuseColor = ColorRGB(255.0f, 255.0f, 255.0f);
//	mShader.LightVector = Vector3f(0.0f, 0.0f, 1.0f);
//	mShader.ModelView = Object.GetModelWorldMatrix();
//	mShader.ModelViewProjection = camera.mPerspProjection * mShader.ModelView;
//
//	for (int i = 0; i != Object.mMesh.nFaces; ++i)
//	{
//		Vector3f fVertices[3];
//		fVertices[0] = Object.mMesh.Vertices[Object.mMesh.VertexIndices[i][0]];
//		fVertices[1] = Object.mMesh.Vertices[Object.mMesh.VertexIndices[i][1]];
//		fVertices[2] = Object.mMesh.Vertices[Object.mMesh.VertexIndices[i][2]];
//
//		Vector3f Normal = Object.mMesh.FaceNormals[i];
//
//		Vector4f vClip[3];
//		for (int j = 0; j != 3; ++j)
//		{
//			vClip[j] = mShader.Vertex(fVertices[j], Normal, j);
//		}
//
//		Rasterizer::DrawTriangle(vClip, mShader, *FrameBuffer, *ZBuffer);
//
//	}
//}

void Renderer::RenderScene(const Scene& Scene)
{
	ClearBuffers();
	// Create Shader
	TextureShader mShader;

	mShader.AmbientColor = { 20.0f, 20.0f, 20.0f };
	mShader.SpecularColor = { 255.0f, 255.0f, 255.0f };
	mShader.LightVector = Normalize(Vector3f(0.0f, 0.0f, 1.0f));

	for (const auto& Object : Scene.mObjects)
	{
		// Get per object variables for the Shader.
		mShader.AlbedoMap = Object.mAlbedo;
		mShader.GlossMap = Object.mGloss;
		mShader.NormalMap = Object.mNormal;
		mShader.ModelView = Object.GetModelWorldMatrix();
		mShader.ModelViewProjection = Scene.mCamera.mPerspProjection * mShader.ModelView;
		mShader.N = Transpose(Inverse(mShader.ModelView));

		for (int i = 0; i != Object.mMesh.nFaces; ++i)
		{
			// Get per-face variables
			Vector3f fVertices[3];
			fVertices[0] = Object.mMesh.Vertices[Object.mMesh.VertexIndices[i][0]];
			fVertices[1] = Object.mMesh.Vertices[Object.mMesh.VertexIndices[i][1]];
			fVertices[2] = Object.mMesh.Vertices[Object.mMesh.VertexIndices[i][2]];

			// Back-face culling
			Vector3f Normal = MultiplyMatrixVector(mShader.ModelView, Object.mMesh.FaceNormals[i]);
			Vector3f ViewDir = -MultiplyMatrixPoint(mShader.ModelView, fVertices[0]);
			if (Dot(Normal, ViewDir) < 0) continue;

			// Get Vertex Normals
			Vector3f vNormals[3];
			vNormals[0] = Object.mMesh.VertexNormals[Object.mMesh.NormalIndices[i][0]];
			vNormals[1] = Object.mMesh.VertexNormals[Object.mMesh.NormalIndices[i][1]];
			vNormals[2] = Object.mMesh.VertexNormals[Object.mMesh.NormalIndices[i][2]];

			// Get Vertex Tangents
			Vector3f vTangents[3];
			vTangents[0] = Object.mMesh.VertexTangents[Object.mMesh.VertexIndices[i][0]];
			vTangents[1] = Object.mMesh.VertexTangents[Object.mMesh.VertexIndices[i][1]];
			vTangents[2] = Object.mMesh.VertexTangents[Object.mMesh.VertexIndices[i][2]];

			// Get Vertex UVs
			mShader.varying_uv[0] = Object.mMesh.VertexUVs[Object.mMesh.TextureIndices[i][0]];
			mShader.varying_uv[1] = Object.mMesh.VertexUVs[Object.mMesh.TextureIndices[i][1]];
			mShader.varying_uv[2] = Object.mMesh.VertexUVs[Object.mMesh.TextureIndices[i][2]];
		
			Vector4f vClip[3];
			for (int j = 0; j != 3; ++j)
			{
				vClip[j] = mShader.Vertex(fVertices[j], vNormals[j], vTangents[j], j);
			}

			if (FrustumCulling(vClip)) continue;

			//Perspective Divide
			for (int i = 0; i != 3; ++i)
			{
				vClip[i].x = vClip[i].x / vClip[i].w;
				vClip[i].y = vClip[i].y / vClip[i].w;
				vClip[i].z = vClip[i].z / vClip[i].w;
			}

			Rasterizer::DrawTriangle(vClip, mShader, *FrameBuffer, *ZBuffer);
			//Rasterizer::DrawWireFrame(vWire, *FrameBuffer);

		}
	}
}

bool Renderer::FrustumCulling(Vector4f* Vertices)
{
	int count = 0;
	for (int i = 0; i != 3; ++i)
	{
		Vector4f Cull = Vertices[i];
		if (Cull.z < 0) return true;
		else if (Cull.x < -Cull.w || Cull.x > Cull.w || Cull.y < -Cull.w || Cull.y > Cull.w || Cull.z > Cull.w)
			++count;
	}
	if (count == 3) return true;
	return false;
}