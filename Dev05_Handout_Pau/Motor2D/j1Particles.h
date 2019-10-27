#ifndef __MODULEPARTICLES_H__
#define __MODULEPARTICLES_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"
#include "SDL/include/SDL.h"

#define MAX_ACTIVE_PARTICLES 300

struct SDL_Texture;

struct Particle
{
	Animation anim;
	iPoint position;
	iPoint speed;
	Uint32 born = 0;
	Uint32 life = 0;
	SDL_RendererFlip flip;

	Particle();
	Particle(const Particle& p);
	bool Update();
};

class j1Particles : public j1Module
{
public:
	j1Particles();
	~j1Particles();

	bool Start();
	bool Update(float) override;
	bool CleanUp();

	void AddParticle(const Particle& particle, int x, int y, Uint32 delay = 0, Uint32 lifeVar = 0, float speedX = 0, float speedY = 0, SDL_RendererFlip flip_texture = SDL_FLIP_NONE);

private:

	SDL_Texture* graphics = nullptr;
	SDL_Texture* graphics2 = nullptr;
	Particle* active[MAX_ACTIVE_PARTICLES];
	uint last_particle = 0;

public:

	Particle runParticle;
	Particle dustParticle;
	Particle DashParticle;
};

#endif // __MODULEPARTICLES_H__