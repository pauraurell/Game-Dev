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
}

j1Particles::~j1Particles()
{}

// Load assets
bool j1Particles::Start()
{
	graphics = App->tex->Load("textures/particles.png");

	dustParticle.anim.PushBack({ 2, 10, 14, 7 }, 0.24f, 1, 1, 1, 1);
	dustParticle.anim.PushBack({ 40, 10, 26, 8 }, 0.22f, 1, 1, 1, 1);
	dustParticle.anim.PushBack({ 89, 4, 47, 15 }, 0.22f, 1, 1, 1, 1);
	dustParticle.anim.PushBack({ 154, 1, 60, 18 }, 0.24f, 1, 1, 1, 1);
	dustParticle.anim.PushBack({ 89, 4, 47, 15 }, 0.3f, 1, 1, 1, 1);
	dustParticle.anim.PushBack({ 40, 10, 26, 8 }, 0.3f, 1, 1, 1, 1);
	dustParticle.anim.PushBack({ 2, 10, 14, 7 }, 0.3f, 1, 1, 1, 1);

	runParticle.anim.PushBack({ 0, 1, 13, 7 }, 0.24f, 1, 1, 1, 1);
	runParticle.anim.PushBack({ 15, 2, 10, 6 }, 0.3f, 1, 1, 1, 1);
	runParticle.anim.PushBack({ 27, 2, 8, 6 }, 0.3f, 1, 1, 1, 1);
	runParticle.anim.PushBack({ 38, 2, 8, 6 }, 0.3f, 1, 1, 1, 1);

	return true;
}

// Unload assets
bool j1Particles::CleanUp()
{
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
			App->render->Blit(graphics, p->position.x, p->position.y, &(p->anim.GetCurrentFrame()));
		}
		//LOG("Particles: %d", active[i]);
	}


	return true;
}

void j1Particles::AddParticle(const Particle& particle, int x, int y, Uint32 delay, Uint32 lifeVar, float speedX, float speedY)
{
	Particle* p = new Particle(particle);
	p->born = SDL_GetTicks() + delay;
	p->position.x = x;
	p->position.y = y;
	p->life = lifeVar;

	p->speed.x = speedX;
	p->speed.y = speedY;
	
	dustParticle.anim.Reset();
	active[last_particle++] = p;
}

// -------------------------------------------------------------
// -------------------------------------------------------------

Particle::Particle()
{
	position.SetToZero();
	speed.SetToZero();
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
		if ((SDL_GetTicks() - born) > life)
			ret = false;
		//LOG("deleting particle");
	}
	else
		if (anim.Finished())
			ret = false;

	position.x += speed.x;
	position.y += speed.y;
	
	return ret;
}