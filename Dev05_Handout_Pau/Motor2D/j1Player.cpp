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


j1Player::j1Player() : j1Module()
{
	name.create("player");
}

// Destructor
j1Player::~j1Player()
{}

// Called before render is available
bool j1Player::Awake(pugi::xml_node& config)
{
	bool ret = true;

	folder.create(config.child("folder").child_value());
	texture_path = config.child("sprite_sheet").attribute("source").as_string();
	Player.position.x = config.child("initialPosition").attribute("x").as_int();
	Player.position.y = config.child("initialPosition").attribute("y").as_int();
	node = config;

	return ret;
}

// Called before the first frame
bool j1Player::Start()
{
	Player.LoadPushbacks();


	Player.current_animation = &Player.idle;
	
	Player.Character_tex = App->tex->Load(PATH(folder.GetString(), texture_path.GetString()));
	return true;
}

// Called each loop iteration
bool j1Player::PreUpdate()
{
	return true;
}

bool j1Player::Update(float dt)
{
	return true;
}

bool j1Player::PostUpdate()
{
	return true;
}

void j1Player::Draw()
{
	App->render->Blit(Player.Character_tex, Player.position.x, Player.position.y, &(Player.current_animation->GetCurrentFrame()));
}



// Called before quitting
bool j1Player::CleanUp()
{

	return true;
}

// Load Game State
bool j1Player::Load(pugi::xml_node& data)
{

	return true;
}

// Save Game State
bool j1Player::Save(pugi::xml_node& data) const
{

	return true;
}


void PlayerData::LoadPushbacks()
{
	idle.PushBack({ 13, 7, 19, 29 });
}