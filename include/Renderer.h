#pragma once
#include "Model.h"
#include "Camera.h"
#include "Buffer.h"
#include "Shader.h"
#include "Scene.h"
#include "DisplayManager.h"
#include <memory>


class Renderer
{
public:
	Renderer() {
		CreateBuffers(DisplayManager::SCREEN_WIDTH, DisplayManager::SCREEN_HEIGHT);
		ClearBuffers();
	}

	void RenderObject(const Model& Object, const Camera& camera);
	void RenderScene(const Scene& Scene);
	void CreateBuffers(int Width, int Height);
	void ClearBuffers();
	bool FrustumCulling(Vector4f* Vertices); //Culls triangles with vertices behind near plane and triangles completely outside viewing planes. Does not clip triangles.

	std::unique_ptr<Buffer<uint32_t>> FrameBuffer;
	std::unique_ptr<Buffer<float>> ZBuffer;
};