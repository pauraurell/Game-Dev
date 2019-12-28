// ----------------------------------------------------
// j1Timer.cpp
// Fast timer with milisecons precision
// ----------------------------------------------------

#include "j1Timer.h"
#include "SDL\include\SDL_timer.h"

// ---------------------------------------------
j1Timer::j1Timer()
{
	Start();
}

// ---------------------------------------------
void j1Timer::Start()
{
	// TODO 1: Fill Start(), Read(), ReadSec() methods
	// they are simple, one line each!
	started_at = SDL_GetTicks();
	skipped = 0;
	skippedBefore = 0;
	stopped_at = 0;
	running = true;
}

// ---------------------------------------------
uint32 j1Timer::Read()
{
	if (running == true) { return SDL_GetTicks() - skipped - started_at; }
	else if (running == false)
	{
		skipped = skippedBefore + SDL_GetTicks() - stopped_at;
		return SDL_GetTicks() - skipped - started_at;
	}
}

// ---------------------------------------------
float j1Timer::ReadSec()
{
	if (running == true) { return (SDL_GetTicks() - skipped - started_at) / 1000.0f; }
	else if (running == false)
	{
		skipped = skippedBefore + SDL_GetTicks() - stopped_at;
		return (SDL_GetTicks() - skipped - started_at) / 1000.0f;
	}
}

void j1Timer::Stop()
{
	stopped_at = SDL_GetTicks();
	running = false;
}

void j1Timer::Restart()
{
	skipped = skippedBefore + SDL_GetTicks() - stopped_at;
	running = true;
}