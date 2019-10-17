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
#include "j1Collision.h"
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
	speed = config.child("speed").attribute("value").as_int();
	node = config;

	return ret;
}

// Called before the first frame
bool j1Player::Start()
{
	Pushbacks();

	current_animation = &idle;
	colPlayer = App->col->AddCollider({ position.x, position.y, current_animation->GetCurrentFrame().w, current_animation->GetCurrentFrame().h }, COLLIDER_PLAYER, this);

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
		current_animation = &jump;
		position.y = position.y - speed;
	}

	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		current_animation = &running;
		orientation = "right";
		position.x = position.x + speed;
	}

	else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		current_animation = &idle;
		position.y = position.y + speed;
	}

	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		current_animation = &running;
		orientation = "left";
		position.x = position.x - speed;
	}
		
	else 
	{
		current_animation = &idle;
	}
		
	colPlayer->SetPos(position.x , position.y);

	return true;
}

bool j1Player::PostUpdate()
{

	return true;
}

void j1Player::Draw()
{
	if (orientation == "right")
	{
		App->render->Blit(Character_tex, position.x, position.y, &(current_animation->GetCurrentFrame()));
	}

	else if (orientation == "left")
	{
		App->render->Blit(Character_tex, position.x, position.y, &(current_animation->GetCurrentFrame()), SDL_FLIP_HORIZONTAL);
	}

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
	idle.PushBack({ 14, 7, 19, 29 }, 0.12f, 1, 1, 1, 1);
	idle.PushBack({ 66, 6, 17, 30 }, 0.12f, 1, 1, 1, 1);
	idle.PushBack({ 115, 6, 19, 30 }, 0.12f, 1, 1, 1, 1);
	idle.PushBack({ 163, 7, 20, 29 }, 0.12f, 1, 1, 1, 1);

	running.PushBack({ 67, 45, 20, 28 }, 0.16f, 1, 1, 1, 1);
	running.PushBack({ 116, 46, 20, 27 }, 0.16f, 1, 1, 1, 1);
	running.PushBack({ 166, 48, 20, 25 }, 0.16f, 1, 1, 1, 1);
	running.PushBack({ 217, 45, 23, 28 }, 0.16f, 1, 1, 1, 1);
	running.PushBack({ 266, 46, 20, 27 }, 0.16f, 1, 1, 1, 1);
	running.PushBack({ 316, 48, 20, 25 }, 0.16f, 1, 1, 1, 1);

	jump.PushBack({ 15, 86, 20, 24 }, 0.16f, 1, 1, 1, 1);
	jump.PushBack({ 65, 88, 20, 22 }, 0.16f, 1, 1, 1, 1);
	jump.PushBack({ 117, 81, 19, 27 }, 0.16f, 1, 1, 1, 1);
	jump.PushBack({ 164, 79, 21, 23 }, 0.16f, 1, 1, 1, 1);
	jump.PushBack({ 218, 46, 15, 21 }, 0.16f, 1, 1, 1, 1);
	jump.PushBack({ 264, 84, 24, 17 }, 0.16f, 1, 1, 1, 1);
	jump.PushBack({ 320, 81, 19, 21 }, 0.16f, 1, 1, 1, 1);
	jump.PushBack({ 11, 124, 26, 17 }, 0.16f, 1, 1, 1, 1);
	jump.PushBack({ 68, 112, 17, 31 }, 0.16f, 1, 1, 1, 1);
	jump.PushBack({ 118, 113, 17, 30 }, 0.16f, 1, 1, 1, 1);
}

void j1Player::OnCollision(Collider* c1, Collider* c2)
{
	if (c2->type == COLLIDER_WALL || c2->type == COLLIDER_PLAYER)
	{
		//what does the collision here
	}

}