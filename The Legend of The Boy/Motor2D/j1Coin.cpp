#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Coin.h"
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
#include "j1Map.h"
#include "j1EntityManager.h"
#include "j1Score.h"
#include "j1EntityManager.h"
#include "j1Player.h"

j1Coin::j1Coin() : j1Entities(Types::coin)
{
	name.create("coin");
	orientation = "right";
	OnGround = true;
}

j1Coin::j1Coin(iPoint pos, bool isDead) : j1Entities(Types::coin)
{
	name.create("bat");
	position = pos;
	dead = isDead;
	to_die = false;
	orientation = "right";
	OnGround = true;
}

// Destructor
j1Coin::~j1Coin()
{}

// Called before render is available
bool j1Coin::Awake(pugi::xml_node& config)
{
	bool ret = true;

	ConfigLoading();

	return ret;
}

// Called before the first frame
bool j1Coin::Start()
{
	Pushbacks();

	current_animation = &coin;
	colliderBody = App->col->AddCollider({ position.x, position.y, 15, 14 }, COLLIDER_COIN, this);
	coinTex = App->tex->Load("textures/coin_texture.png");
	return true;
}

// Called each loop iteration
bool j1Coin::PreUpdate()
{
	colliderBody->SetPos(position.x, position.y);

	return true;
}

bool j1Coin::Update(float dt)
{
	

	return true;
}

bool j1Coin::PostUpdate()
{
	return true;
}

void j1Coin::Draw(float dt)
{
	if (orientation == "right")
	{
		App->render->Blit(coinTex, position.x + current_animation->pivotx[current_animation->returnCurrentFrame()], position.y + current_animation->pivoty[current_animation->returnCurrentFrame()], &(current_animation->GetCurrentFrame(dt)), SDL_FLIP_NONE, 1.0f, 1.0f, 0.0);
	}

	else if (orientation == "left")
	{
		App->render->Blit(coinTex, position.x + current_animation->pivotx2[current_animation->returnCurrentFrame()], position.y + current_animation->pivoty2[current_animation->returnCurrentFrame()], &(current_animation->GetCurrentFrame(dt)), SDL_FLIP_HORIZONTAL, 1.0f, 1.0f, 0.0);
	}

}


// Called before quitting
bool j1Coin::CleanUp()
{
	App->tex->UnLoad(coinTex);

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
bool j1Coin::Save(pugi::xml_node& data) const
{
	pugi::xml_node batNode = data;
	batNode.append_attribute("type") = name.GetString();
	batNode.append_attribute("dead") = dead;
	batNode = data.append_child("position");
	batNode.append_attribute("posX") = position.x;
	batNode.append_attribute("posY") = position.y;

	return true;
}

void j1Coin::ConfigLoading()
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
	gravity = config.child("gravity").attribute("value").as_float();
	node = config;
}


void j1Coin::Pushbacks()
{
	coin.PushBack({ 1, 0, 14, 16 }, 0.12f, 1, 1, 1, 1);
	coin.PushBack({ 19, 0, 10, 16 }, 0.12f, 1, 1, 1, 1);
	coin.PushBack({ 37, 0, 6, 16 }, 0.12f, 1, 1, 1, 1);
	coin.PushBack({ 51, 0, 10, 16 }, 0.12f, 1, 1, 1, 1);
	coin.PushBack({ 61, 0, 13, 16 }, 0.12f, 1, 1, 1, 1);
}


void j1Coin::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 == colliderBody && c2->type == COLLIDER_PLAYER)
	{
		LOG(true, "Coin Collected");
		App->score->coins++;
		App->entManager->DestroyCoin(this);
	}
}
