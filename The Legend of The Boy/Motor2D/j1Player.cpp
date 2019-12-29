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
#include "j1EntityManager.h"
#include "SDL_image/include/SDL_image.h"
#include "j1Map.h"
#include "j1UI.h"
#include "j1Timer.h"
#include "j1Score.h"
#include "Brofiler/Brofiler.h"

j1Player::j1Player() : j1Entities(Types::player)
{
	name.create("player");
	state = PLAYER_IDLE;
	current_animation = &idle;
	orientation = "right";
	dash = false;
	OnGround = true;
	dead = false;
	dashTimer = false;
	attackTimer = false;
	respawnTimer = false;
	ColOffsset = 5;
	ColOffssetON = false;
	godMode = false;
	godModeRight = false;
	godModeLeft = false;
	godModeUp = false;
	godModeDown = false;
	hit = false;
	cantGetHit = false;
}

j1Player::j1Player(iPoint pos) : j1Entities(Types::player)
{
	name.create("player");
	position = pos;
	state = PLAYER_IDLE;
	current_animation = &idle;
	orientation = "right";
	dash = false;
	OnGround = true;
	dead = false;
	dashTimer = false;
	attackTimer = false;
	respawnTimer = false;
	ColOffsset = 5;
	ColOffssetON = false;
	godMode = false;
	godModeRight = false;
	godModeLeft = false;
	godModeUp = false;
	godModeDown = false;
}

// Destructor
j1Player::~j1Player()
{}

// Called before render is available
bool j1Player::Awake(pugi::xml_node& config)
{
	bool ret = true;

	ConfigLoading();

	return ret;
}

// Called before the first frame
bool j1Player::Start()
{
	Pushbacks();
	vel.x = 0;
	vel.y = 0;

	hitTimer = new j1Timer;

	JumpFx = App->audio->LoadFx("audio/jumping.wav");
	RunFx = App->audio->LoadFx("audio/running.wav");
	HitFx = App->audio->LoadFx("audio/hit.wav");

	colliderHead = App->col->AddCollider({ position.x, position.y, 15, 8 }, COLLIDER_PLAYER, this);
	colliderBody = App->col->AddCollider({ position.x, position.y, 22, 16}, COLLIDER_PLAYER, this);
	colliderLegs = App->col->AddCollider({ position.x, position.y, 14, 12 }, COLLIDER_PLAYER, this);
	Character_tex = App->tex->Load("textures/adventurer-v1.5-Sheet.png");
	return true;

}

// Called each loop iteration
bool j1Player::PreUpdate()
{
	colliderHead->SetPos(position.x + 3, position.y);
	colliderBody->SetPos(position.x, position.y + colliderHead->rect.h - 12);
	colliderLegs->SetPos(position.x + 4, position.y + colliderHead->rect.h + 2);

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
			App->audio->PlayFx(JumpFx, App->audio->volume_fx, 0, 2);
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
				if (orientation == "right") { dashAttackCollider = App->col->AddCollider({ position.x + 15, position.y + 10, 25, 10 }, COLLIDER_PLAYER_ATTACK); }
				if (orientation == "left") { dashAttackCollider = App->col->AddCollider({ position.x - 10, position.y + 10, 25, 10 }, COLLIDER_PLAYER_ATTACK); }
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
				App->scene->input = true;
				dashTimer = false;
				dashAttackCollider->to_delete = true;
			}
			else
			{
				App->scene->input = false;
			}
			break;

		case PLAYER_ATTACK:
			if (attackTimer == false)
			{
				attack_timer = SDL_GetTicks();
				attackTimer = true;
				if (orientation == "right") 
				{
					attackCollider= App->col->AddCollider({ position.x + 15, position.y +4, 22, 22 }, COLLIDER_PLAYER_ATTACK);
				}
				else 
				{
					attackCollider = App->col->AddCollider({ position.x - 10, position.y + 4, 22, 22 }, COLLIDER_PLAYER_ATTACK);
				}
			}
			
			if (SDL_GetTicks() - attack_timer > attackTime)
			{
				App->scene->input = true;
				attackTimer = false;
				attackCollider->to_delete = true;
			}
			else
			{
				App->scene->input = false;
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

		if (dead == true) { Respawn(); }

	}
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		godMode = !godMode;
	}

	//God Mode Input, State & Movement
	if (godMode == true && App->scene->input == true)
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
			position.y = position.y - 3 * dt * DTCOEFICIENT;
			if (godModeRight == true) { position.x = position.x + 3; orientation = "right"; current_animation = &god_mode_run; }
			if (godModeLeft == true) { position.x = position.x - 3; orientation = "left"; current_animation = &god_mode_run; }
			if (godModeDown == true) { position.y = position.y + 3; current_animation = &idle; }
			else { current_animation = &god_mode_up; }
		}
		
		else if (godModeDown == true)
		{
			position.y = position.y + 3 * dt * DTCOEFICIENT;
			if (godModeRight == true) { position.x = position.x + 3; orientation = "right"; current_animation = &god_mode_run; }
			if (godModeLeft == true) { position.x = position.x - 3; orientation = "left"; current_animation = &god_mode_run; }
			else { current_animation = &god_mode_down; }
		}

		else if (godModeRight == true)
		{
			position.x = position.x + 3 * dt * DTCOEFICIENT;
			orientation = "right";
			if (godModeLeft == true) { position.x = position.x - 3; current_animation = &god_mode_run; }
			else { current_animation = &god_mode_run; }
		}

		else if (godModeLeft == true)
		{
			position.x = position.x - 3 * dt * DTCOEFICIENT;
			orientation = "left";
			current_animation = &god_mode_run;
		}

		else
		{
			current_animation = &idle;
		}
	}

	if (OnGround == true && cantGetHit == true) { App->scene->input = true; }

	if (hit == true)
	{
		cantGetHit = true;
		hit = false;
		//hitTimer = new j1Timer();
		hitTimer->Start();
	}

	if (hitTimer->ReadSec() > 3 && cantGetHit == true) { cantGetHit = false; }

	if (App->ui->pLife == 0) { dead = true; }

	OnGround = false;

	App->render->cameraPos = position;

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



// Save Game State
bool j1Player::Save(pugi::xml_node& data) const
{
	LOG(true, "Saving player state");

	pugi::xml_node playerNode = data;
	playerNode.append_attribute("type") = name.GetString();
	playerNode.append_attribute("dead") = dead;
	playerNode = data.append_child("position");
	playerNode.append_attribute("posX") = position.x;
	playerNode.append_attribute("posY") = position.y;

	return true;
}

void j1Player::GetPlayerState()
{
	if (App->scene->input == true && godMode == false)
	{
		
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && vel.y == 0)
		{
			state = PLAYER_JUMP;
			jump.Reset();
			App->particles->AddParticle(App->particles->dustParticle, position.x - 8, position.y + 16, 0, 500, 0, 0);
		}

		else if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN && dash == false)
		{
			App->audio->PlayFx(JumpFx, App->audio->volume_fx, 0, 2);
			if (orientation == "right") {App->particles->AddParticle(App->particles->DashParticle, position.x - 8, position.y + 16, 0, 250, 0, 0);}
			if (orientation == "left") { App->particles->AddParticle(App->particles->DashParticle, position.x - 8, position.y + 16, 0, 250, 0, 0, SDL_FLIP_HORIZONTAL); }
			
			state = PLAYER_DASH;
			dash = true;
		}

		else if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN && dash == false)
		{
			attack.Reset();
			state = PLAYER_ATTACK;
			App->scene->input = false;
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
			App->scene->input = true;
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
	if (c1 == colliderLegs && c2->type == COLLIDER_WALL)
	{
		if ((colliderLegs->rect.y + colliderLegs->rect.h) > (c2->rect.y))
		{
			vel.y = 0;
			if (dash == true)
			{
				dash = false;
			}
			if ((colliderLegs->rect.y + colliderLegs->rect.h - 3) > (c2->rect.y))
			{
				position.y -= 2;
			}

			OnGround = true;
		} 
		//if the collider is under the player
	}

	if (c1 == colliderHead && c2->type == COLLIDER_WALL)
	{
		if ((colliderHead->rect.y) < (c2->rect.y + c2->rect.h))
		{
			position.y = position.y + 2;
			vel.y = 0;
		}
	}

	if (c1 == colliderBody && c2->type == COLLIDER_WALL)
	{
		if (state == PLAYER_DASH)
		{
			if (colliderBody->rect.x + colliderBody->rect.w > c2->rect.x && colliderBody->rect.x < c2->rect.x) {
				position.x = position.x - 7;
				vel.x = 0;
			}
			if (colliderBody->rect.x < c2->rect.x + c2->rect.w && colliderBody->rect.x > c2->rect.x)
			{
				position.x = position.x + 8;
				vel.x = 0;
			}
		}
		else
		{
			if (colliderBody->rect.x + colliderBody->rect.w > c2->rect.x && colliderBody->rect.x < c2->rect.x) {
				position.x = position.x - 2;
				vel.x = 0;
			}
			if (colliderBody->rect.x < c2->rect.x + c2->rect.w && colliderBody->rect.x > c2->rect.x)
			{
				position.x = position.x + 2;
				vel.x = 0;
			}
		}
	}

	//Secret path activation
	if (c1 == colliderBody && c2->type == COLLIDER_SECRET)
	{
		App->scene->secret_map = true;
	}

	//Collision with enemies
	if ((c1 == colliderBody || c1 == colliderHead || c1 == colliderLegs) && c2->type == COLLIDER_ENEMY)
	{
		if (godMode == false && cantGetHit == false)
		{
			//dead = true;
			if (App->ui->pLife > 0)
			{
				App->ui->pLife -= 1;
				App->audio->PlayFx(HitFx, App->audio->volume_fx, 0, 2);
				if (App->scene->cameraTracking == true) { LOG(true, "Player Lifes: %i", App->ui->pLife); }
				hit = true;
				App->scene->input = false;
				vel.y = -4;
				if (orientation == "right") { vel.x = -40; }
				else if (orientation == "left") { vel.x = 40; }
				state = PLAYER_IDLE;
			}
		}
	}

	//Map Change
	if (c1 == colliderBody && c2->type == COLLIDER_FINISH || c1->type == COLLIDER_FINISH && c2 == colliderBody)
	{
		if (App->scene->CurrentMap == "FirstLevel.tmx" && App->scene->scene_changed == false)
		{
			App->scene->StartSecondLevel();
		}
		if (App->scene->CurrentMap == "SecondLevel.tmx" && App->scene->sceneEnded == false)
		{
			//App->scene->EndScene();
			App->score->draw = true;
			App->ui->time.Stop();
			App->scene->input = false;
			SetPlayerState(PLAYER_IDLE);
			App->scene->sceneEnded = true;
		}
		App->scene->scene_changed = false;
	}
}

void j1Player::GetPlayerPosition(float dt)
{
	vel.y += (gravity * dt * DTCOEFICIENT);
	position.x = position.x + (vel.x * dt * DTCOEFICIENT);
	position.y = position.y + (vel.y * dt * DTCOEFICIENT);

	if (position.y > yLimit && godMode == false) { Respawn(); }
}

void j1Player::Respawn()
{
	if (respawnTimer == false)
	{
		respawn_timer = SDL_GetTicks();
		App->fade->FadeToBlackVisualEffect(1.2);
		App->scene->input = false;
		SetPlayerState(PLAYER_IDLE);
		respawnTimer = true;
		if (attackCollider != nullptr) { attackCollider->to_delete = true; }
		if (dashAttackCollider != nullptr) { dashAttackCollider->to_delete = true; }
	}

	if (SDL_GetTicks() - respawn_timer > 615) {
		position.y = App->scene->PlayerSpawnPointY;
		vel.y = 0;
		orientation = "right";
		dead = false;
		App->scene->input = true;
		respawnTimer = false;
		App->scene->secret_map = false;
		if (App->ui->pLife == 0 || App->ui->pLife - 1 == 0)
		{ 
			//App->ui->pLife = 3;
			//if (App->scene->CurrentMap == "FirstLevel.tmx") { App->scene->Create1MapEnemies(); position.x = App->scene->PlayerSpawnPointX; }
			//if (App->scene->CurrentMap == "SecondLevel.tmx") { App->scene->Create2MapEnemies(); position.x = 20; }
			App->fade->ResetFade();
			App->scene->to_end = true;
			LOG(true, "Player Died");
		}
		else 
		{
			App->ui->pLife -= 1;
			if (App->scene->CurrentMap == "FirstLevel.tmx") { position.x = App->scene->PlayerSpawnPointX; }
			if (App->scene->CurrentMap == "SecondLevel.tmx") { position.x = 20; }
			LOG(true, "Respawning Player");
		}
		LOG(true, "Player Lifes: %i", App->ui->pLife);
	}
}

void j1Player::ConfigLoading()
{
	pugi::xml_document	config_file;
	pugi::xml_node		config;
	config = App->LoadConfig(config_file);
	config = config.child("player");
	folder.create(config.child("folder").child_value());
	texture_path = config.child("sprite_sheet").attribute("source").as_string();
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
}