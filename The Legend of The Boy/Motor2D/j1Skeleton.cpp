#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Skeleton.h"
#include "j1Textures.h"
#include "j1Map.h"
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
#include "j1Pathfinding.h"

j1Skeleton::j1Skeleton() : j1Entities(Types::skeleton)
{
	name.create("skeleton");
}

j1Skeleton::j1Skeleton(iPoint pos) : j1Entities(Types::skeleton)
{
	name.create("skeleton");
	position = pos;
	
}

// Destructor
j1Skeleton::~j1Skeleton()
{}

// Called before render is available
bool j1Skeleton::Awake(pugi::xml_node& config)
{
	bool ret = true;
	ConfigLoading();
	return ret;
}

// Called before the first frame
bool j1Skeleton::Start()
{
	Pushbacks();
	vel.x = 0;
	vel.y = 0;

	//SkeletonFx = App->audio->LoadFx("audio/jumping.wav");

	current_animation = &idle;
	skeletonColliderBody = App->col->AddCollider({ position.x + 1, position.y + 8, 15, 14 }, COLLIDER_ENEMY, this);
	skeletonColliderLegs = App->col->AddCollider({ position.x + 3, position.y + 21, 11, 12 }, COLLIDER_ENEMY, this);
	skeletonTex = App->tex->Load("textures/skeleton_spritesheet.png");
	return true;
}

// Called each loop iteration
bool j1Skeleton::PreUpdate()
{
	skeletonColliderBody->SetPos(position.x + 1, position.y + 8);
	skeletonColliderLegs->SetPos(position.x + 3, position.y + 21);

	return true;
}

bool j1Skeleton::Update(float dt)
{

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		state = SKELETON_MOVING_LEFT;
	}

	else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		state = SKELETON_MOVING_RIGHT;
	}

	else 
	{
		if (state != SKELETON_DEAD) { state = SKELETON_IDLE; }
	} 

	switch (state)
	{
	case SKELETON_MOVING_LEFT:
		orientation = "left";
		position.x = position.x - 2;
		current_animation = &walking;
		break;

	case SKELETON_MOVING_RIGHT:
		orientation = "right";
		position.x = position.x + 2;
		current_animation = &walking;
		break;

	case SKELETON_DEAD:
		current_animation = &deadAnim;
		
		break;

	case SKELETON_IDLE:
		current_animation = &idle;
		break;
		
	}

	SetSkeletonPosition(dt);
	return true;
}

bool j1Skeleton::PostUpdate()
{
	
	return true;
}

void j1Skeleton::Draw(float dt)
{
	if (orientation == "right")
	{
		App->render->Blit(skeletonTex, position.x + current_animation->pivotx[current_animation->returnCurrentFrame()], position.y + current_animation->pivoty[current_animation->returnCurrentFrame()], &(current_animation->GetCurrentFrame(dt)), SDL_FLIP_NONE, 1.0f, 1.0f, 0.0);
	}

	else if (orientation == "left")
	{
		App->render->Blit(skeletonTex, position.x + current_animation->pivotx2[current_animation->returnCurrentFrame()], position.y + current_animation->pivoty2[current_animation->returnCurrentFrame()], &(current_animation->GetCurrentFrame(dt)), SDL_FLIP_HORIZONTAL, 1.0f, 1.0f, 0.0);
	}

}


// Called before quitting
bool j1Skeleton::CleanUp()
{
	App->tex->UnLoad(skeletonTex);
	skeletonColliderBody->to_delete = true;
	skeletonColliderLegs->to_delete = true;

	return true;
}

// Load Game State
/*bool j1Skeleton::Load(pugi::xml_node& data)
{
	position.x = data.child("position").attribute("posX").as_int();
	position.y = data.child("position").attribute("posY").as_int();
	return true;
}*/

// Save Game State
bool j1Skeleton::Save(pugi::xml_node& data) const
{
	pugi::xml_node playerNode = data;
	playerNode.append_attribute("type") = name.GetString();
	playerNode = data.append_child("position");
	playerNode.append_attribute("posX") = position.x;
	playerNode.append_attribute("posY") = position.y;
	return true;
}


void j1Skeleton::SkeletonState(skeletonStates stateS)
{
	state = stateS;
}

void j1Skeleton::Pushbacks()
{
	//Skeleton animations

	idle.PushBack({ 0, 118, 22, 32 }, 0.12f, 1, 1, 1, 1);
	idle.PushBack({ 24, 118, 22, 32 }, 0.12f, 1, 1, 1, 1);
	idle.PushBack({ 48, 118, 22, 32 }, 0.12f, 1, 1, 1, 1);
	idle.PushBack({ 72, 118, 22, 32 }, 0.12f, 1, 1, 1, 1);
	idle.PushBack({ 97, 118, 22, 32 }, 0.12f, 1, 1, 1, 1);
	idle.PushBack({ 122, 118, 22, 32 }, 0.12f, 1, 1, 1, 1);
	idle.PushBack({ 146, 118, 22, 32 }, 0.12f, 1, 1, 1, 1);
	idle.PushBack({ 169, 118, 22, 32 }, 0.12f, 1, 1, 1, 1);
	idle.PushBack({ 192, 118, 22, 32 }, 0.12f, 1, 1, 1, 1);
	idle.PushBack({ 216, 118, 22, 32 }, 0.12f, 1, 1, 1, 1);
	idle.PushBack({ 240, 118, 22, 32 }, 0.12f, 1, 1, 1, 1);

	deadAnim.PushBack({ 0, 82, 22, 32 }, 0.12f, 1, 1, 1, 1);
	deadAnim.PushBack({ 30, 82, 22, 32 }, 0.12f, 1, 1, 1, 1);
	deadAnim.PushBack({ 62, 82, 22, 32 }, 0.12f, 1, 1, 1, 1);
	deadAnim.PushBack({ 93, 82, 22, 32 }, 0.12f, 1, 1, 1, 1);
	deadAnim.PushBack({ 126, 82, 22, 32 }, 0.12f, 1, 1, 1, 1);
	deadAnim.PushBack({ 159, 82, 22, 32 }, 0.12f, 1, 1, 1, 1);
	deadAnim.PushBack({ 192, 82, 22, 32 }, 0.12f, 1, 1, 1, 1);
	deadAnim.PushBack({ 225, 82, 22, 32 }, 0.12f, 1, 1, 1, 1);
	deadAnim.PushBack({ 258, 83, 23, 31 }, 0.12f, 1, 1, 1, 1);
	deadAnim.PushBack({ 287, 86, 23, 28 }, 0.12f, 1, 1, 1, 1);
	deadAnim.PushBack({ 323, 82, 24, 25 }, 0.12f, 1, 1, 1, 1);
	deadAnim.PushBack({ 356, 82, 25, 16 }, 0.12f, 1, 1, 1, 1);
	deadAnim.PushBack({ 385, 82, 30, 7 }, 0.12f, 1, 1, 1, 1);
	deadAnim.loop = false;

	walking.PushBack({ 0, 42, 20, 33 }, 0.12f, 1, 1, 1, 1);
	walking.PushBack({ 22, 42, 21, 33 }, 0.12f, 1, 1, 1, 1);
	walking.PushBack({ 44, 42, 22, 33 }, 0.12f, 1, 1, 1, 1);
	walking.PushBack({ 66, 43, 22, 32 }, 0.12f, 1, 1, 1, 1);
	walking.PushBack({ 88, 43, 22, 32 }, 0.12f, 1, 1, 1, 1);
	walking.PushBack({ 110, 43, 22, 32 }, 0.12f, 1, 1, 1, 1);
	walking.PushBack({ 132, 43, 22, 32 }, 0.12f, 1, 1, 1, 1);
	walking.PushBack({ 155, 43, 20, 32 }, 0.12f, 1, 1, 1, 1);
	walking.PushBack({ 178, 43, 18, 32 }, 0.12f, 1, 1, 1, 1);
	walking.PushBack({ 200, 43, 17, 32 }, 0.12f, 1, 1, 1, 1);
	
}


void j1Skeleton::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 == skeletonColliderLegs && c2->type == COLLIDER_WALL)
	{
		if ((skeletonColliderLegs->rect.y + skeletonColliderLegs->rect.h) > (c2->rect.y))
		{
			vel.y = 0;
			if ((skeletonColliderLegs->rect.y + skeletonColliderLegs->rect.h - 3) > (c2->rect.y))
			{
				position.y -= 2;
			}

			OnGround = true;
		}
	}

	/*
	if (c1 == skeletonColliderLegs && c2->type == COLLIDER_WALL)
	{
		if ((skeletonColliderLegs->rect.y) < (c2->rect.y + c2->rect.h))
		{
			position.y = position.y + 2;
			vel.y = 0;
		}
	}
	*/

	/*
	if (c1 == skeletonColliderBody && c2->type == COLLIDER_WALL)
	{
		if (skeletonColliderBody->rect.x + skeletonColliderBody->rect.w > c2->rect.x && skeletonColliderBody->rect.x < c2->rect.x) {
			position.x = position.x - 2;
			if (vel.y > 0) { vel.x = 0; }
		}
		if (skeletonColliderBody->rect.x < c2->rect.x + c2->rect.w && skeletonColliderBody->rect.x > c2->rect.x)
		{
			position.x = position.x + 2;
			if (vel.y > 0) { vel.x = 0; }
		}
	}
	*/

	if (c1 == skeletonColliderBody && c2->type == COLLIDER_PLAYER_ATTACK)
	{
		//LOG("coliding hehehe");
		state = SKELETON_DEAD;
	}
}

void j1Skeleton::SetSkeletonPosition(float dt)
{
	vel.y += gravity;
	position.x = position.x + (vel.x * dt * DTCOEFICIENT);
	position.y = position.y + (vel.y * dt * DTCOEFICIENT);
}

bool j1Skeleton::SkeletonPathFinding(float dt) {

	static iPoint InicialEntityPosition;

	int x, y;
	App->input->GetMousePosition(x, y);

	iPoint rightCell(position.x, position.y);iPoint upCell(position.x, position.y);
	iPoint leftCell(position.x, position.y);iPoint DownCell(position.x, position.y);

	iPoint playerPos = App->entManager->GetPlayerEntity()->position;
	playerPos = App->map->WorldToMap(playerPos.x + 30, playerPos.y + 30);

	InicialEntityPosition = App->map->WorldToMap(position.x, position.y);
	App->pathfinding->CreatePath(InicialEntityPosition, playerPos);


	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();
	if (path->At(1) != NULL)
	{
		if (state != SKELETON_DEAD)
		{
			if (path->At(1)->x < InicialEntityPosition.x && !App->pathfinding->IsWalkable(DownCell))
			{
				position.x -= 2 * DTCOEFICIENT * dt;
				orientation = "left";
			}
			if (path->At(1)->x > InicialEntityPosition.x && !App->pathfinding->IsWalkable(rightCell))
			{
				position.x += 2 * DTCOEFICIENT * dt;
				orientation = "right";
			}
		}
	}
	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint nextPathPosition = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
	}

	return true;
}

void j1Skeleton::ConfigLoading()
{
	pugi::xml_document	config_file;
	pugi::xml_node		config;
	config = App->LoadConfig(config_file);
	config = config.child("skeleton");
	folder.create(config.child("folder").child_value());
	texture_path = config.child("sprite_sheet").attribute("source").as_string();
	SpawnPointX = config.child("initialPosition").attribute("x").as_int();
	SpawnPointY = config.child("initialPosition").attribute("y").as_int();
	orientation = config.child("initialPosition").attribute("orientation").as_string();
	SpeedX = config.child("speed").attribute("Speedx").as_float();
	SpeedY = config.child("speed").attribute("Speedy").as_float();
	gravity = config.child("gravity").attribute("value").as_float();
	node = config;
}
