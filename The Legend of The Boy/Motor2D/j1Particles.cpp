#include <math.h>
#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Particles.h"
#include "j1Player.h"
#include "p2Defs.h"
#include "p2Log.h"

#include "SDL/include/SDL_timer.h"

j1Particles::j1Particles()
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		active[i] = nullptr;

	name.create("particles");
}

j1Particles::~j1Particles()
{}

// Load assets
bool j1Particles::Start()
{
	graphics = App->tex->Load("textures/particles.png");

	last_particle = 0;

	//Particle that appeares when the player jumps
	dustParticle.anim.PushBack({ 1, 6, 10, 6 }, 0.22f, 1, 1, 1, 1);
	dustParticle.anim.PushBack({ 30, 5, 19, 6 }, 0.2f, 1, 1, 1, 1);
	dustParticle.anim.PushBack({ 66, 0, 35, 11 }, 0.2f, 1, 1, 1, 1);
	dustParticle.anim.PushBack({ 114, 0, 43, 11 }, 0.2f, 1, 1, 1, 1);
	dustParticle.anim.PushBack({ 114, 16, 46, 8 }, 0.18f, 1, 1, 1, 1);
	dustParticle.anim.PushBack({ 114, 28, 47, 9 }, 0.15f, 1, 1, 1, 1);
	dustParticle.anim.PushBack({ 165, 5, 53, 8 }, 0.1f, 1, 1, 1, 1);
	
	//Particle that appeares when the player dash
	DashParticle.anim.PushBack({ 40, 21, 14, 12 }, 0.1f, 1, 1, 1, 1);
	DashParticle.anim.PushBack({ 2, 21, 14, 12 }, 0.1f, 1, 1, 1, 1);

	//Dirt particle that appeares when the player runs
	runParticle.anim.PushBack({ 68, 23, 8, 5 }, 0.3f, 1, 1, 1, 1);
	runParticle.anim.PushBack({ 85, 24, 8, 5 }, 0.3f, 1, 1, 1, 1);
	runParticle.anim.PushBack({ 98, 25, 8, 5 }, 0.3f, 1, 1, 1, 1);

	return true;
}

// Unload assets
bool j1Particles::CleanUp()
{
	LOG(false, "Unloading particles");
	App->tex->UnLoad(graphics);

	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (active[i] != nullptr)
		{
			delete active[i];
			active[i] = nullptr;
		}
	}

	return true;
}

// Update: draw background
bool j1Particles::Update(float dt)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* p = active[i];

		if (p == nullptr)
			continue;

		if (p->Update() == false)
		{
			delete p;
			active[i] = nullptr;
		}
		else if (SDL_GetTicks() >= p->born)
		{
			if (p->flip == SDL_FLIP_NONE)
			{
				App->render->Blit(graphics, p->position.x + p->anim.pivotx[p->anim.returnCurrentFrame()], p->position.y + p->anim.pivoty[p->anim.returnCurrentFrame()], &(p->anim.GetCurrentFrame(dt)), SDL_FLIP_NONE);
			}
			else
			{
				App->render->Blit(graphics, p->position.x + p->anim.pivotx2[p->anim.returnCurrentFrame()], p->position.y + p->anim.pivoty2[p->anim.returnCurrentFrame()], &(p->anim.GetCurrentFrame(dt)), SDL_FLIP_HORIZONTAL);
			}
		}
	}



	return true;
}

void j1Particles::AddParticle(const Particle& particle, int x, int y, Uint32 delay, Uint32 lifeVar, float speedX, float speedY, SDL_RendererFlip flip_texture)
{
	Particle* p = new Particle(particle);
	p->born = SDL_GetTicks() + delay;
	p->position.x = x;
	p->position.y = y;
	p->life = lifeVar;
	p->flip = flip_texture;

	p->speed.x = speedX;
	p->speed.y = speedY;
	
	dustParticle.anim.Reset();

	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (active[i] == nullptr)
		{
			Particle* p = new Particle(particle);
			p->born = SDL_GetTicks() + delay;
			p->position.x = x;
			p->position.y = y;
			p->life = lifeVar;
			p->flip = flip_texture;

			p->speed.x = speedX;
			p->speed.y = speedY;

			dustParticle.anim.Reset();
			
			active[i] = p;
			break;
		}
	}
}

// -------------------------------------------------------------
// -------------------------------------------------------------

Particle::Particle()
{
	position.SetToZero();
	speed.SetToZero();

	born = 0;
	life = 0;
}

Particle::Particle(const Particle& p) :
	anim(p.anim), position(p.position), speed(p.speed),
	born(p.born), life(p.life)
{}

bool Particle::Update()
{
	bool ret = true;

	if (life > 0)
	{
		if (((int)SDL_GetTicks() - born) > life)
			ret = false;
	}
	else
		if (anim.Finished())
			ret = false;

	return ret;
}