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
	position.x = config.child("initialPosition").attribute("x").as_int();
	position.y = config.child("initialPosition").attribute("y").as_int();
	node = config;

	return ret;
}

// Called before the first frame
bool j1Player::Start()
{
	Pushbacks();


	current_animation = &idle;
	
	Character_tex = App->tex->Load(PATH(folder.GetString(), texture_path.GetString()));
	return true;
}

// Called each loop iteration
bool j1Player::PreUpdate()
{
	return true;
}

bool j1Player::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		current_animation = &running;
		position.y--;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		current_animation = &running;
		position.x++;
	}
		
	else
		current_animation = &idle;
	return true;
}

bool j1Player::PostUpdate()
{
	return true;
}

void j1Player::Draw()
{
	App->render->Blit(Character_tex, position.x, position.y, &(current_animation->GetCurrentFrame()));
}


// Called before quitting
bool j1Player::CleanUp()
{
	App->tex->UnLoad(Character_tex);

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


void j1Player::Pushbacks()
{
	idle.PushBack({ 14, 7, 19, 29 });
	idle.PushBack({ 66, 6, 17, 30 });
	idle.PushBack({ 115, 6, 19, 30 });
	idle.PushBack({ 163, 7, 20, 29 });
	idle.speed = 0.065f;

	running.PushBack({ 67, 45, 20, 28 });
	running.PushBack({ 116, 46, 20, 27 });
	running.PushBack({ 166, 48, 20, 25 });
	running.PushBack({ 217, 45, 23, 28 });
	running.PushBack({ 266, 46, 20, 27 });
	running.PushBack({ 316, 48, 20, 25 });
	running.speed = 0.08f;
}