#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Player.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "j1Particles.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1FadeToBlack.h"
#include "j1Collision.h"
#include "j1Entities.h"
#include "SDL_image/include/SDL_image.h"
#include "j1Map.h"
#include "Brofiler/Brofiler.h"

j1Player::j1Player() : j1Entities(Types::player)
{
	name.create("player");
}

j1Player::j1Player(iPoint pos) : j1Entities(Types::player)
{
	name.create("player");
	position = pos;
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
	yLimit = config.child("initialPosition").attribute("yLimit").as_int();
	maxSpeedX = config.child("speed").attribute("MaxSpeedX").as_int();
	maxSpeedY = config.child("speed").attribute("MaxSpeedY").as_float();
	SpeedX = config.child("speed").attribute("Speedx").as_float();
	SpeedY = config.child("speed").attribute("Speedy").as_float();
	dashSpeed = config.child("speed").attribute("DashSpeed").as_float();
	gravity = config.child("gravity").attribute("value").as_float();
	maxJumpSpeed = config.child("speed").attribute("jumpMaxSpeed").as_int();
	speedDecrease = config.child("speed").attribute("SpeedDecrease").as_float();
	dashTime = config.child("Dash").attribute("dashTime").as_int();
	attackTime = config.child("Attacking").attribute("attackTime").as_int();
	node = config;

	return ret;
}

// Called before the first frame
bool j1Player::Start()
{
	Pushbacks();
	vel.x = 0;
	vel.y = 0;

	JumpFx = App->audio->LoadFx("audio/jumping.wav");
	RunFx = App->audio->LoadFx("audio/running.wav");

	position.x = SpawnPointX;
	position.y = SpawnPointY;

	current_animation = &idle;
	colPlayerHead = App->col->AddCollider({ position.x, position.y, 15, 8 }, COLLIDER_PLAYER, this);
	colPlayerBody = App->col->AddCollider({ position.x, position.y, 22, 13}, COLLIDER_PLAYER, this);
	colPlayerLegs = App->col->AddCollider({ position.x, position.y, 14, 12 }, COLLIDER_PLAYER, this);
	Character_tex = App->tex->Load(PATH(folder.GetString(), texture_path.GetString()));
	return true;
}

// Called each loop iteration
bool j1Player::PreUpdate()
{
	colPlayerHead->SetPos(position.x + 3, position.y);
	colPlayerBody->SetPos(position.x, position.y + colPlayerHead->rect.h - 12);
	colPlayerLegs->SetPos(position.x + 4, position.y + colPlayerHead->rect.h + 2);

	return true;
}

bool j1Player::Update(float dt)
{
	
	BROFILER_CATEGORY("Update_Player", Profiler::Color::MediumSlateBlue)

	if (godMode == false)
	{
		GetPlayerState();
		switch (state) {
		case PLAYER_JUMP:
			OnGround = false;
			App->audio->PlayFx(JumpFx, 0, 2);
			while (vel.y > -4.5)
			{
				current_animation = &jump;
				vel.y -= SpeedY;
			}
			break;

		case PLAYER_RUN_LEFT:
			orientation = "left";
			vel.x -= SpeedX;
			current_animation = &running;
			if (OnGround)
			{
				if (position.x % 5 == 0)
				{
					App->particles->AddParticle(App->particles->runParticle, position.x + 6, position.y + 23, 0, 125, 0, 0);
					//App->audio->PlayFx(RunFx, 0, 4);
				}
			}
			if (OnGround == false) { current_animation = &jump; }
			break;

		case PLAYER_JUMP_LEFT:
			orientation = "left";
			vel.x -= SpeedX;
			current_animation = &jump;
			break;

		case PLAYER_RUN_RIGHT:
			orientation = "right";
			vel.x += SpeedX;
			current_animation = &running;
			if (OnGround) 
			{
				if (position.x % 5 == 0)
				{
					//App->audio->PlayFx(RunFx, 0, 4);
					App->particles->AddParticle(App->particles->runParticle, position.x, position.y + 23, 0, 125, 0, 0);
				}
			}
			if (OnGround == false) { current_animation = &jump; }
			break;

		case PLAYER_JUMP_RIGHT:
			orientation = "right";
			vel.x += SpeedX;
			current_animation = &jump;
			break;

		case PLAYER_DASH:
			if (dashTimer == false)
			{
				dash_timer = SDL_GetTicks();
				dashTimer = true;
				if (orientation == "right") { dashAttackCollider = App->col->AddCollider({ position.x + 15, position.y + 10, 15, 10 }, COLLIDER_PLAYER_ATTACK); }
				if (orientation == "left") { dashAttackCollider = App->col->AddCollider({ position.x - 10, position.y + 10, 15, 10 }, COLLIDER_PLAYER_ATTACK); }
			}
			if (orientation == "right")
			{
				vel.x = dashSpeed;
				dashAttackCollider->SetPos(position.x + 15, position.y + 10);
			}
			else
			{
				vel.x = -dashSpeed;
				dashAttackCollider->SetPos(position.x - 10, position.y + 10);
			}
			
			vel.y = 0;
			current_animation = &ground_dash;
			if (SDL_GetTicks() - dash_timer > dashTime)
			{
				input = true;
				dashTimer = false;
				dashAttackCollider->to_delete = true;
			}
			else
			{
				input = false;
			}
			break;

		case PLAYER_ATTACK:
			if (attackTimer == false)
			{
				attack_timer = SDL_GetTicks();
				attackTimer = true;
				if (orientation == "right") 
				{
					attackCollider= App->col->AddCollider({ position.x + 15, position.y +4, 20, 20 }, COLLIDER_PLAYER_ATTACK);
				}
				else 
				{
					attackCollider = App->col->AddCollider({ position.x - 10, position.y + 4, 20, 20 }, COLLIDER_PLAYER_ATTACK);
				}
			}
			
			if (SDL_GetTicks() - attack_timer > attackTime)
			{
				input = true;
				attackTimer = false;
				attackCollider->to_delete = true;
			}
			else
			{
				input = false;
				current_animation = &attack;
			}
			break;

		case PLAYER_IDLE:
			//Slowing down velocity
			if (vel.x != 0 && vel.x > 0) { vel.x = vel.x - speedDecrease; }
			if (vel.x != 0 && vel.x < 0) { vel.x = vel.x + speedDecrease; }
			
			if (OnGround == false) { current_animation = &jump; }

			if (vel.y == 0 && vel.x == 0)
			{
				current_animation = &idle;
			}
			break;
		}

		//Controlling the maximum speed that the player can go
		if (state != PLAYER_DASH) {
			if (vel.x > maxSpeedX) { vel.x = maxSpeedX; }
			if (vel.x < -maxSpeedX) { vel.x = -maxSpeedX; }
		}
		if (vel.y > maxSpeedY) { vel.y = maxSpeedY; }

		GetPlayerPosition(dt);

		if (dead == true)
		{
			Respawn();
		}

	}
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		godMode = !godMode;
	}

	//God Mode Input, State & Movement
	if (godMode)
	{
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) { godModeUp = true; }
		else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_UP) { godModeUp = false; }

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) { godModeDown = true; }
		else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_UP) { godModeDown = false; }

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) { godModeRight = true; }
		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP) { godModeRight = false; }

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) { godModeLeft = true; }
		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP) { godModeLeft = false; }

		if (godModeUp == true)
		{
			position.y = position.y - 3;
			if (godModeRight == true) { position.x = position.x + 3; orientation = "right"; current_animation = &god_mode_run; }
			if (godModeLeft == true) { position.x = position.x - 3; orientation = "left"; current_animation = &god_mode_run; }
			if (godModeDown == true) { position.y = position.y + 3; current_animation = &idle; }
			else { current_animation = &god_mode_up; }
		}
		
		else if (godModeDown == true)
		{
			position.y = position.y + 3;
			if (godModeRight == true) { position.x = position.x + 3; orientation = "right"; current_animation = &god_mode_run; }
			if (godModeLeft == true) { position.x = position.x - 3; orientation = "left"; current_animation = &god_mode_run; }
			else { current_animation = &god_mode_down; }
		}

		else if (godModeRight == true)
		{
			position.x = position.x + 3;
			orientation = "right";
			if (godModeLeft == true) { position.x = position.x - 3; current_animation = &god_mode_run; }
			else { current_animation = &god_mode_run; }
		}

		else if (godModeLeft == true)
		{
			position.x = position.x - 3;
			orientation = "left";
			current_animation = &god_mode_run;
		}

		else
		{
			current_animation = &idle;
		}
	}
	OnGround = false;

	return true;
}

bool j1Player::PostUpdate()
{
	return true;
}

void j1Player::Draw(float dt)
{
	BROFILER_CATEGORY("Draw_Player", Profiler::Color::Crimson)

	if (orientation == "right")
	{
		App->render->Blit(Character_tex, position.x + current_animation->pivotx[current_animation->returnCurrentFrame()], position.y + current_animation->pivoty[current_animation->returnCurrentFrame()], &(current_animation->GetCurrentFrame(dt)), SDL_FLIP_NONE, 1.0f, 1.0f, 0.0);
	}

	else if (orientation == "left")
	{
		App->render->Blit(Character_tex, position.x + current_animation->pivotx2[current_animation->returnCurrentFrame()], position.y + current_animation->pivoty2[current_animation->returnCurrentFrame()], &(current_animation->GetCurrentFrame(dt)), SDL_FLIP_HORIZONTAL, 1.0f, 1.0f, 0.0);
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
	if (input == true && godMode == false)
	{
		
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && vel.y == 0)
		{
			state = PLAYER_JUMP;
			jump.Reset();
			App->particles->AddParticle(App->particles->dustParticle, position.x - 8, position.y + 16, 0, 500, 0, 0);
		}

		else if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN && dash == false)
		{
			App->audio->PlayFx(JumpFx, 0, 3);
			if (orientation == "right") {App->particles->AddParticle(App->particles->DashParticle, position.x - 8, position.y + 16, 0, 250, 0, 0);}
			if (orientation == "left") { App->particles->AddParticle(App->particles->DashParticle, position.x - 8, position.y + 16, 0, 250, 0, 0, SDL_FLIP_HORIZONTAL); }
			
			state = PLAYER_DASH;
			dash = true;
		}

		else if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN && dash == false)
		{
			attack.Reset();
			state = PLAYER_ATTACK;
			input = false;
		}

		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			if (state == PLAYER_JUMP)
			{
				state = PLAYER_JUMP_RIGHT;
			}
			if (state != PLAYER_JUMP)
			{
				state = PLAYER_RUN_RIGHT;
			}
		}

		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			if (state == PLAYER_JUMP)
			{
				state = PLAYER_JUMP_LEFT;
			}
			if (state != PLAYER_JUMP)
			{
				state = PLAYER_RUN_LEFT;
			}
		}

		else
		{
			state = PLAYER_IDLE;
			input = true;
		}
	}
}

void j1Player::SetPlayerState(playerStates stateP)
{
	state = stateP;
}

void j1Player::Pushbacks()
{
	//idle
	idle.PushBack({ 14, 7, 19, 29 }, 0.12f, 1, 1, 1, 1);
	idle.PushBack({ 66, 6, 17, 30 }, 0.12f, 1, 1, 1, 1);
	idle.PushBack({ 115, 6, 19, 30 }, 0.12f, 1, 1, 1, 1);
	idle.PushBack({ 163, 7, 20, 29 }, 0.12f, 1, 1, 1, 1);

	//run
	running.PushBack({ 67, 45, 20, 28 }, 0.16f, 1, 1, 1, 1);
	running.PushBack({ 116, 46, 20, 27 }, 0.16f, 1, 1, 1, 1);
	running.PushBack({ 166, 48, 20, 25 }, 0.16f, 1, 1, 1, 1);
	running.PushBack({ 217, 45, 23, 28 }, 0.16f, 1, 1, 1, 1);
	running.PushBack({ 266, 46, 20, 27 }, 0.16f, 1, 1, 1, 1);
	running.PushBack({ 316, 48, 20, 25 }, 0.16f, 1, 1, 1, 1);

	//jump
	jump.PushBack({ 15, 86, 20, 24 }, 0.25f, 1, 1, 1, 1);
	jump.PushBack({ 65, 88, 20, 22 }, 0.25f, 1, 1, 1, 1);
	jump.PushBack({ 117, 81, 19, 27 }, 0.25f, 1, 1, 1, 1);
	jump.PushBack({ 164, 79, 21, 23 }, 0.25f, 1, 1, 1, 1);
	jump.PushBack({ 218, 46, 15, 21 }, 0.25f, 1, 1, 1, 1);
	jump.PushBack({ 264, 84, 24, 17 }, 0.25f, 1, 1, 1, 1);
	jump.PushBack({ 320, 81, 19, 21 }, 0.25f, 1, 1, 1, 1);
	jump.PushBack({ 11, 124, 26, 17 }, 0.25f, 1, 1, 1, 1);
	jump.PushBack({ 68, 112, 17, 31 }, 0.05f, 1, 1, 1, 1);
	jump.PushBack({ 118, 113, 17, 30 }, 0.05f, 1, 1, 1, 1);
	jump.loop = false;

	//dash
	ground_dash.PushBack({ 155, 132, 34, 15 }, 0.25f, 1, 1, 1, 1);
	ground_dash.PushBack({ 205, 132, 34, 15 }, 0.25f, 1, 1, 1, 1);
	ground_dash.PushBack({ 255, 131, 34, 16 }, 0.25f, 1, 1, 1, 1);
	ground_dash.PushBack({ 309, 130, 30, 17 }, 0.25f, 1, 1, 1, 1);
	ground_dash.PushBack({ 15, 167, 22, 17 }, 0.25f, 1, 1, 1, 1);

	//attack
	attack.PushBack({ 60, 266, 37, 29 }, 0.14f, 1, 1, 1, 1);
	attack.PushBack({ 102, 274, 32, 21 }, 0.14f, 1, 1, 1, 1);
	attack.PushBack({ 152, 273, 31, 22 }, 0.14f, 1, 1, 1, 1);
	
	//God Mode Animations
	god_mode_up.PushBack({70, 150, 14, 34}, 0.25f, 1, 1, 1, 1);
	god_mode_up.PushBack({120, 150, 14, 34}, 0.25f, 1, 1, 1, 1);
	god_mode_up.PushBack({169, 150, 15, 34}, 0.25f, 1, 1, 1, 1);
	god_mode_up.PushBack({220, 150, 14, 34}, 0.25f, 1, 1, 1, 1);

	god_mode_run.PushBack({16, 413, 23, 29}, 0.25f, 1, 1, 1, 1);
	god_mode_run.PushBack({67, 413, 22, 29}, 0.25f, 1, 1, 1, 1);

	god_mode_down.PushBack({67, 111, 17, 31}, 0.25f, 1, 1, 1, 1);
	god_mode_down.PushBack({117, 112, 17, 30}, 0.25f, 1, 1, 1, 1);
}


void j1Player::OnCollision(Collider* c1, Collider* c2)
{
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
	}

	//Map Change
	if (c1 == colPlayerBody && c2->type == COLLIDER_FINISH || c1->type == COLLIDER_FINISH && c2 == colPlayerBody)
	{
		if (App->scene->scene_change == true && App->scene->scene_changed == false)
		{
			App->scene->StartFirstLevel();
			App->scene->scene_changed = true;
		}
		if (App->scene->scene_change == false && App->scene->scene_changed == false)
		{
			App->scene->StartSecondLevel();
			App->scene->scene_changed = true;
		}
		App->scene->scene_changed = false;
	}

	//Secret path activation
	if (c1 == colPlayerBody && c2->type == COLLIDER_SECRET)
	{
		App->scene->secret_map = true;
	}
}

void j1Player::GetPlayerPosition(float dt)
{
	vel.y += (gravity * dt * 70);
	position.x = position.x + (vel.x * dt * 70);
	position.y = position.y + (vel.y * dt * 70);

	if (position.y > yLimit && godMode == false) { dead = true; }
}

void j1Player::Respawn()
{
	if (respawnTimer == false)
	{
		respawn_timer = SDL_GetTicks();
		App->fade->FadeToBlack(1.2);
		input = false;
		respawnTimer = true;
	}

	if (SDL_GetTicks() - respawn_timer > 615) {
		position.x = SpawnPointX;
		position.y = SpawnPointY;
		vel.y = 0;
		orientation = "right";
		dead = false;
		input = true;
		respawnTimer = false;
		App->scene->secret_map = false;
	}
}