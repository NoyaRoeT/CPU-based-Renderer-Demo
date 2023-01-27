#pragma once
#include <cstdint>
#include <SDL.h>
#include "Buffer.h"

struct SDL_Surface;
struct SDL_Window;

class DisplayManager
{
public:
	//Dummy Constructor and Destructor
	DisplayManager(){}
	~DisplayManager(){}

public:
	const static int SCREEN_WIDTH = 1280;
	const static int SCREEN_HEIGHT = 720;
	constexpr static float SCREEN_ASPECT_RATIO = SCREEN_WIDTH / (float)SCREEN_HEIGHT;

	bool Init();
	void ShutDown();
	void Update(Buffer<uint32_t>& FrameBuffer);

	SDL_PixelFormat* MappingFormat = nullptr;

private:
	SDL_Surface* mSurface = nullptr;
	SDL_Window* mWindow = nullptr;
	
};