#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Bat.h"
#include "j1Textures.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Window.h"
#include "j1Collision.h"
#include "j1Entities.h"
#include "SDL_image/include/SDL_image.h"
#include "Brofiler/Brofiler.h"
#include "j1PathFinding.h"
#include "j1Map.h"
#include "j1EntityManager.h"

j1Bat::j1Bat() : j1Entities(Types::bat)
{
	name.create("bat");
}

j1Bat::j1Bat(iPoint pos) : j1Entities(Types::bat)
{
	name.create("bat");
	position = pos;

}

// Destructor
j1Bat::~j1Bat()
{}

// Called before render is available
bool j1Bat::Awake(pugi::xml_node& config)
{
	bool ret = true;

	ConfigLoading();

	return ret;
}

// Called before the first frame
bool j1Bat::Start()
{
	Pushbacks();
	vel.x = 0;
	vel.y = 0;

	//BatFx = App->audio->LoadFx("audio/jumping.wav");

	current_animation = &flying;
	batCollider = App->col->AddCollider({ position.x, position.y, 15, 14 }, COLLIDER_ENEMY, this);
	batTex = App->tex->Load("textures/bat_spritesheet.png");
	return true;
}

// Called each loop iteration
bool j1Bat::PreUpdate()
{
	batCollider->SetPos(position.x, position.y);

	return true;
}

bool j1Bat::Update(float dt)
{
		switch (state) 
		{
			case BAT_FLIYING_LEFT:
				orientation = "left";
				position.x--;
				if (position.x <= SpawnPointX) { state = BAT_FLIYING_RIGHT; }
				break;

			case BAT_FLIYING_RIGHT:
				orientation = "right";
				position.x++;
				if (position.x >= SpawnPointX + 100) { state = BAT_FLIYING_LEFT; }
				break;

			case FLYING_TO_THE_PLAYER:
				current_animation = &flying;
				break;
		}

		SetBatPosition(dt);

	return true;
}

bool j1Bat::PostUpdate()
{
	return true;
}

void j1Bat::Draw(float dt)
{
		if (orientation == "right")
		{
			App->render->Blit(batTex, position.x + current_animation->pivotx[current_animation->returnCurrentFrame()], position.y + current_animation->pivoty[current_animation->returnCurrentFrame()], &(current_animation->GetCurrentFrame(dt)), SDL_FLIP_NONE, 1.0f, 1.0f, 0.0);
		}

		else if (orientation == "left")
		{
			App->render->Blit(batTex, position.x + current_animation->pivotx2[current_animation->returnCurrentFrame()], position.y + current_animation->pivoty2[current_animation->returnCurrentFrame()], &(current_animation->GetCurrentFrame(dt)), SDL_FLIP_HORIZONTAL, 1.0f, 1.0f, 0.0);
		}

}


// Called before quitting
bool j1Bat::CleanUp()
{
	App->tex->UnLoad(batTex);

	return true;
}

// Load Game State
/*bool j1Bat::Load(pugi::xml_node& data)
{
	position.x = data.child("position").attribute("posX").as_int();
	position.y = data.child("position").attribute("posY").as_int();
	return true;
}*/

// Save Game State
bool j1Bat::Save(pugi::xml_node& data) const
{
	pugi::xml_node playerNode = data;
	playerNode.append_attribute("type") = name.GetString();
	playerNode = data.append_child("position");
	playerNode.append_attribute("posX") = position.x;
	playerNode.append_attribute("posY") = position.y;
	
	return true;
}


void j1Bat::BatState(batStates stateB)
{
	state = stateB;
}

void j1Bat::Pushbacks()
{
	//Bat flying 
	flying.PushBack({ 7, 49, 19, 15 }, 0.12f, 1, 1, 1, 1);
	flying.PushBack({ 41, 33, 17, 21 }, 0.12f, 1, 1, 1, 1);
	flying.PushBack({ 73, 38, 17, 15 }, 0.12f, 1, 1, 1, 1);
	flying.PushBack({ 107, 38, 15, 21 }, 0.12f, 1, 1, 1, 1);
}


void j1Bat::OnCollision(Collider* c1, Collider* c2)
{/*
	if (c1 == colPlayerLegs && c2->type == COLLIDER_WALL)
	{
		if ((colPlayerLegs->rect.y + colPlayerLegs->rect.h) > (c2->rect.y))
		{
			vel.y = 0;
			if (dash == true)
			{
				dash = false;
			}
			if ((colPlayerLegs->rect.y + colPlayerLegs->rect.h - 3) > (c2->rect.y))
			{
				position.y -= 2;
			}

			OnGround = true;
		}
		//if the collider is under the player
	}

	if (c1 == colPlayerHead && c2->type == COLLIDER_WALL)
	{
		if ((colPlayerHead->rect.y) < (c2->rect.y + c2->rect.h))
		{
			position.y = position.y + 2;
			vel.y = 0;
		}
	}

	if (c1 == colPlayerBody && c2->type == COLLIDER_WALL)
	{
		if (state == PLAYER_DASH)
		{
			if (colPlayerBody->rect.x + colPlayerBody->rect.w > c2->rect.x && colPlayerBody->rect.x < c2->rect.x) {
				position.x = position.x - 3;
				//vel.x = 0;
			}
			if (colPlayerBody->rect.x < c2->rect.x + c2->rect.w && colPlayerBody->rect.x > c2->rect.x)
			{
				position.x = position.x + 4;
			}
		}
		else
		{
			if (colPlayerBody->rect.x + colPlayerBody->rect.w > c2->rect.x && colPlayerBody->rect.x < c2->rect.x) {
				position.x = position.x - 2;
				if (vel.y > 0) { vel.x = 0; }
			}
			if (colPlayerBody->rect.x < c2->rect.x + c2->rect.w && colPlayerBody->rect.x > c2->rect.x)
			{
				position.x = position.x + 2;
				if (vel.y > 0) { vel.x = 0; }
			}
		}
	}*/
}

void j1Bat::SetBatPosition(float dt)
{
	position.x = position.x + (vel.x * dt * 70);
	position.y = position.y + (vel.y * dt * 70);
}

bool j1Bat::BatPathfinding(float dt) {

	static iPoint InicialEntityPosition;

	int x, y;
	App->input->GetMousePosition(x, y);

	iPoint p = App->entManager->GetPlayerEntity()->position;
	p = App->map->WorldToMap(p.x + 30, p.y + 30);

	InicialEntityPosition = App->map->WorldToMap(position.x + 30, position.y + 30);
	App->pathfinding->CreatePath(InicialEntityPosition, p);

	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

	if (path->At(1) != NULL)
	{
		state = FLYING_TO_THE_PLAYER;

		if (path->At(1)->x < InicialEntityPosition.x) 
		{
			orientation = "right";
			position.x -= 2 * DTCOEFICIENT * dt;
		}

		if (path->At(1)->x > InicialEntityPosition.x) 
		{
			orientation = "left";
			position.x += 2* DTCOEFICIENT * dt;
		}

		if (path->At(1)->y < InicialEntityPosition.y)
		{
			position.y -= 2 * DTCOEFICIENT * dt;
		}

		if (path->At(1)->y > InicialEntityPosition.y) 
		{
			position.y += 2 * DTCOEFICIENT * dt;
		}
	}
	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint nextPathPosition = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
	}

	return true;
}

void j1Bat::ConfigLoading()
{
	pugi::xml_document	config_file;
	pugi::xml_node		config;
	config = App->LoadConfig(config_file);
	config = config.child("bat");
	folder.create(config.child("folder").child_value());
	texture_path = config.child("sprite_sheet").attribute("source").as_string();
	SpawnPointX = config.child("initialPosition").attribute("x").as_int();
	SpawnPointY = config.child("initialPosition").attribute("y").as_int();
	orientation = config.child("initialPosition").attribute("orientation").as_string();
	SpeedX = config.child("speed").attribute("Speedx").as_float();
	SpeedY = config.child("speed").attribute("Speedy").as_float();
	node = config;
}