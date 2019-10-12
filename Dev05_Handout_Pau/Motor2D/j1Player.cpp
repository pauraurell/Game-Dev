#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Player.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "SDL_image/include/SDL_image.h"


j1Player::j1Player()
{

}

j1Player::~j1Player()
{}

bool j1Player::Awake(pugi::xml_node& config)
{
	bool ret = true;

	return ret;
}

// Load assets
bool j1Player::Start()
{
	return true;
}

// Unload assets
bool j1Player::CleanUp()
{
	LOG("Unloading player");

	return true;
}

bool j1Player::PreUpdate()
{
	return true;
}

bool j1Player::Update(float dt)
{
	return true;
}

void j1Player::Draw()
{
	r = current_animation->GetCurrentFrame();

	App->render->Blit(character_tex, position.x, position.y - r.h, &r);
}

bool j1Player::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool j1Player::Load(pugi::xml_node& data)
{
	//Load
	return true;
}

bool j1Player::Save(pugi::xml_node& data) const
{
	//Save
	return true;
}
