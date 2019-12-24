#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Collision.h"
#include "j1Input.h"
#include "j1Entities.h"
#include "j1EntityManager.h"
#include "SDL_image/include/SDL_image.h"
#include "Brofiler/Brofiler.h"
#include "j1Coin.h"

j1Coin::j1Coin() : j1Entities(Types::coin)
{
	name.create("coin");
}


j1Coin::j1Coin(iPoint pos, bool IsDead) : j1Entities(Types::coin)
{
	name.create("coin");
	position = pos;
}

j1Coin::~j1Coin()
{}

// Called before render is available
bool j1Coin::Awake(pugi::xml_node& config)
{
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Coin::Start()
{
	Pushbacks();
	current_animation = &idle;
	colliderBody = App->col->AddCollider({ position.x, position.y, 15, 14 }, COLLIDER_COIN, this);
	coinTex = App->tex->Load("textures/coin_texture.png");
	return true;
}

// Called each loop iteration
bool j1Coin::PreUpdate()
{
	return true;
}

bool j1Coin::Update(float dt)
{
	current_animation = &idle;

	return true;
}

bool j1Coin::PostUpdate()
{
	return true;
}

void j1Coin::Draw(float dt)
{
	App->render->Blit(coinTex, position.x + current_animation->pivotx2[current_animation->returnCurrentFrame()], position.y + current_animation->pivoty2[current_animation->returnCurrentFrame()], &(current_animation->GetCurrentFrame(dt)), SDL_FLIP_NONE, 1.0f, 1.0f, 0.0);
}


// Called before quitting
bool j1Coin::CleanUp()
{
	App->tex->UnLoad(coinTex);
	colliderBody->to_delete = true;

	return true;
}


// Save Game State
bool j1Coin::Save(pugi::xml_node& data) const
{
	/*pugi::xml_node skeletonNode = data;
	skeletonNode.append_attribute("type") = name.GetString();
	skeletonNode.append_attribute("dead") = dead;
	skeletonNode = data.append_child("position");
	skeletonNode.append_attribute("posX") = position.x;
	skeletonNode.append_attribute("posY") = position.y;*/
	return true;
}


void j1Coin::Pushbacks()
{
	idle.PushBack({ 1, 19, 14, 16 }, 0.12f, 1, 1, 1, 1);
	idle.PushBack({ 19, 19, 10, 16 }, 0.12f, 1, 1, 1, 1);
	idle.PushBack({ 37, 19, 6, 16 }, 0.12f, 1, 1, 1, 1);
	idle.PushBack({ 51, 19, 10, 16 }, 0.12f, 1, 1, 1, 1);
	idle.PushBack({ 61, 19, 13, 16 }, 0.12f, 1, 1, 1, 1);
}


void j1Coin::OnCollision(Collider* c1, Collider* c2)
{
	if (colliderBody->type == COLLIDER_TYPE::COLLIDER_PLAYER)
	{
		App->entManager->DestroyEntity(this);
	}
}
