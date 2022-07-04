#pragma once
#include "Renderer.h"
#include "Scene.h"
#include "DisplayManager.h"

class Engine
{
public:
	Engine() {}
	bool Init();
	void Run();
	void ShutDown() { mDisplayManager.ShutDown(); }

	~Engine() { ShutDown(); }

private:
	Renderer mRenderer;
	Scene mScene;
	DisplayManager mDisplayManager;
};