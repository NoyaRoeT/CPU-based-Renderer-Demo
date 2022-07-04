#include "Engine.h"
#include "Timer.h"

bool Engine::Init()
{
	if (!mDisplayManager.Init())
	{
		printf("Failed to Init mDisplayManager");
		return false;
	}
	return true;
}

void Engine::Run()
{
	// Engine exit flag
	bool quit = false;

	// Iteration Timers and Counters
	unsigned int TotalElapsedTime = 0;
	unsigned int FrameCount = 0;
	unsigned int Start = 0;
	float DeltaT = 0;

	//Event Handling. To be Abstracted to an Input Manager
	SDL_Event e;

	// Application Loop. Going to Abstract all of this in the future.
	while (!quit)
	{
		++FrameCount;
		Timer PerFrameTimer;

		//Poll for input
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT) quit = true;
		}

		//Update
		mScene.Update(DeltaT);

		//Render
		mRenderer.RenderScene(mScene);

		// Update Display
		mDisplayManager.Update(*mRenderer.FrameBuffer);

		// Iteration Timing
		DeltaT = PerFrameTimer.GetElapsedTime();
		TotalElapsedTime += DeltaT;
	}
	printf("Average frame time over %2.1d frames:%2.fms.\n", FrameCount, TotalElapsedTime / (float)FrameCount);
}