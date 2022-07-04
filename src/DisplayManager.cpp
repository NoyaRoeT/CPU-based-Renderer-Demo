#include "DisplayManager.h"
#include <cstdio>
#include <cstring>

bool DisplayManager::Init()
{
	//Start SDL
	if (SDL_Init(SDL_INIT_VIDEO))
	{
		printf("Failed at SDL_Init. SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow("Software Renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!mWindow) { printf("Failed to create SDL Window. SDL_Error: %s\n", SDL_GetError()); return false; }
	mSurface = SDL_GetWindowSurface(mWindow);
	if (!mWindow) { printf("Failed to get SDL Window Surface. SDL_Error: %s\n", SDL_GetError()); return false; }

	MappingFormat = mSurface->format;
	return true;
}

void DisplayManager::Update(Buffer<uint32_t>& FrameBuffer)
{
	SDL_LockSurface(mSurface);
	memcpy(mSurface->pixels, FrameBuffer.mArray, SCREEN_WIDTH * SCREEN_HEIGHT * 4);
	SDL_UnlockSurface(mSurface);
	SDL_UpdateWindowSurface(mWindow);
}

void DisplayManager::ShutDown()
{
	SDL_DestroyWindow(mWindow);
	mWindow = nullptr;
	mSurface = nullptr;
	SDL_Quit();
}