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
#include "j1FadeToBlack.h"
#include "j1Collision.h"
#include "SDL_image/include/SDL_image.h"
#include "j1Map.h"

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
	SpawnPointX = config.child("initialPosition").attribute("x").as_int();
	SpawnPointY = config.child("initialPosition").attribute("y").as_int();
	orientation = config.child("initialPosition").attribute("orientation").as_string();
	maxSpeed = config.child("speed").attribute("MaxSpeed").as_int();
	SpeedX = config.child("speed").attribute("Speedx").as_float();
	SpeedY = config.child("speed").attribute("Speedy").as_float();
	gravity = config.child("gravity").attribute("value").as_float();
	node = config;

	return ret;
}

// Called before the first frame
bool j1Player::Start()
{
	Pushbacks();
	vel.x = 0;
	vel.y = 0;

	position.x = SpawnPointX;
	position.y = SpawnPointY;

	current_animation = &idle;
	colPlayerHead = App->col->AddCollider({ position.x, position.y, current_animation->GetCurrentFrame().w - 5, current_animation->GetCurrentFrame().h/3 }, COLLIDER_PLAYER, this);
	colPlayerBody = App->col->AddCollider({ position.x, position.y, current_animation->GetCurrentFrame().w, current_animation->GetCurrentFrame().h/3 }, COLLIDER_PLAYER, this);
	colPlayerLegs = App->col->AddCollider({ position.x, position.y, current_animation->GetCurrentFrame().w - 8, current_animation->GetCurrentFrame().h / 3 + 3}, COLLIDER_PLAYER, this);

	Character_tex = App->tex->Load(PATH(folder.GetString(), texture_path.GetString()));
	return true;
}

// Called each loop iteration
bool j1Player::PreUpdate()
{
	colPlayerHead->SetPos(position.x + 3, position.y);
	colPlayerBody->SetPos(position.x , position.y + colPlayerHead->rect.h / 2);
	colPlayerLegs->SetPos(position.x + 4, position.y + colPlayerBody->rect.h);

	return true;
}

bool j1Player::Update(float dt)
{
	GetPlayerState();
	
	if (state == PLAYER_JUMP)
	{
		while (vel.y > -5)
		{
			current_animation = &jump;
			vel.y -= SpeedY;
		}
	}

	if (state == PLAYER_RUN_LEFT)
	{
		orientation = "left";
		vel.x -= SpeedX;
		current_animation = &running;
	}

	if (state == PLAYER_RUN_RIGHT)
	{
		orientation = "right";
		vel.x += SpeedX;
		current_animation = &running;
	}

	if (state == PLAYER_IDLE)
	{
		//Slowing down velocity
		if (vel.x != 0 && vel.x > 0) { vel.x = vel.x - 0.25; }
		if (vel.x != 0 && vel.x < 0) { vel.x = vel.x + 0.25; }

		if (vel.y == 0 && vel.x == 0)
		{
			current_animation = &idle;
		}
			
	}

	//Controlling the maximum speed that the player can go
	if (vel.x > maxSpeed) { vel.x = maxSpeed; }
	if (vel.x < -maxSpeed) { vel.x = -maxSpeed; }

	GetPlayerPosition();

	if (dead == true)
	{
		Respawn();
	}

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
	LOG("Loading player state");
	position.x = data.child("position").attribute("posX").as_int();
	position.y = data.child("position").attribute("posY").as_int();
	return true;
}

// Save Game State
bool j1Player::Save(pugi::xml_node& data) const
{
	LOG("Saving player state");
	pugi::xml_node playerNode = data.append_child("position");

	playerNode.append_attribute("posX") = position.x;
	playerNode.append_attribute("posY") = position.y;
	return true;
}

void j1Player::GetPlayerState()
{
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && vel.y == 0)
	{
		state = PLAYER_JUMP;
	}

	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		state = PLAYER_RUN_RIGHT;
	}

	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		state = PLAYER_RUN_LEFT;
	}

	else
	{
		state = PLAYER_IDLE;
	}
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

	jump.PushBack({ 15, 86, 20, 24 }, 0.25f, 1, 1, 1, 1);
	jump.PushBack({ 65, 88, 20, 22 }, 0.25f, 1, 1, 1, 1);
	jump.PushBack({ 117, 81, 19, 27 }, 0.25f, 1, 1, 1, 1);
	jump.PushBack({ 164, 79, 21, 23 }, 0.25f, 1, 1, 1, 1);
	jump.PushBack({ 218, 46, 15, 21 }, 0.25f, 1, 1, 1, 1);
	jump.PushBack({ 264, 84, 24, 17 }, 0.25f, 1, 1, 1, 1);
	jump.PushBack({ 320, 81, 19, 21 }, 0.25f, 1, 1, 1, 1);
	jump.PushBack({ 11, 124, 26, 17 }, 0.25f, 1, 1, 1, 1);
	jump.PushBack({ 68, 112, 17, 31 }, 0.25f, 1, 1, 1, 1);
	jump.PushBack({ 118, 113, 17, 30 }, 0.25f, 1, 1, 1, 1);

	ground_dash.PushBack({ 155, 132, 34, 15 }, 0.25f, 1, 1, 1, 1);
	ground_dash.PushBack({ 205, 132, 34, 15 }, 0.25f, 1, 1, 1, 1);
	ground_dash.PushBack({ 255, 131, 34, 16 }, 0.25f, 1, 1, 1, 1);
	ground_dash.PushBack({ 309, 130, 30, 17 }, 0.25f, 1, 1, 1, 1);
	ground_dash.PushBack({ 15, 167, 22, 17 }, 0.25f, 1, 1, 1, 1);
}

void j1Player::OnCollision(Collider* c1, Collider* c2)
{
	if ((c2->type == COLLIDER_WALL && c1->type == COLLIDER_PLAYER) || (c2->type == COLLIDER_PLAYER && c1->type == COLLIDER_WALL))
	{
		//*provisional*

		if ((c1->rect.y - c1->rect.h) < (c2->rect.y)) { vel.y = 0;} //if the collider is under the player


	}

	

	//Map Change
	if ((c2->type == COLLIDER_FINISH && c1->type == COLLIDER_PLAYER) || (c2->type == COLLIDER_PLAYER && c1->type == COLLIDER_FINISH))
	{
		App->scene->StartSecondLevel();
	}
}

void j1Player::GetPlayerPosition()
{
	vel.y += gravity;
	position.x = position.x + vel.x;
	position.y = position.y + vel.y;

	if (position.y > 800) { dead = true; }
}

void j1Player::Respawn()
{
	App->fade->FadeToBlack(1.5);

	position.x = SpawnPointX;
	position.y = SpawnPointY;
	vel.y = 0;
	orientation = "right";
	dead = false;
}


