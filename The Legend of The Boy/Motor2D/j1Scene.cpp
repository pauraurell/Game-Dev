#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Entities.h"
#include "j1Player.h"
#include "j1Skeleton.h"
#include "j1Bat.h"
#include "j1FadeToBlack.h"
#include "j1EntityManager.h"
#include "j1UI.h"
#include "j1Console.h"
#include "j1MainMenu.h"
#include "Brofiler/Brofiler.h"
#include "UI_Element.h"


j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	bool ret = true;
	pugi::xml_node map;

	LOG(false, "Loading Scene");

	for (map = config.child("map"); map; map = map.next_sibling("map"))
	{
		p2SString* level = new p2SString();

		level->create(map.attribute("name").as_string());
		MapList.add(level->GetString());
	}
	PlayerSpawnPointX = config.child("player").attribute("PosX").as_int();
	PlayerSpawnPointX2 = config.child("player").attribute("PosX2").as_int();
	PlayerSpawnPointY = config.child("player").attribute("PosY").as_int();
	BatSpawnPointX = config.child("bat").attribute("PosX").as_int();
	BatSpawnPointY = config.child("bat").attribute("PosY").as_int();
	BatSpawnPointX2 = config.child("bat").attribute("PosX2").as_int();
	BatSpawnPointY2 = config.child("bat").attribute("PosY2").as_int();
	SkeletonSpawnPointX = config.child("skeleton").attribute("PosX").as_int();
	SkeletonSpawnPointY = config.child("skeleton").attribute("PosY").as_int();
	SkeletonSpawnPointX2 = config.child("skeleton").attribute("PosX2").as_int();
	SkeletonSpawnPointY2 = config.child("skeleton").attribute("PosY2").as_int();
	Bat2SpawnPointX = config.child("bat2").attribute("PosX").as_int();
	Bat2SpawnPointY = config.child("bat2").attribute("PosY").as_int();
	Bat2SpawnPointX2 = config.child("bat2").attribute("PosX2").as_int();
	Bat2SpawnPointY2 = config.child("bat2").attribute("PosY2").as_int();
	Skeleton2SpawnPointX = config.child("skeleton2").attribute("PosX").as_int();
	Skeleton2SpawnPointY = config.child("skeleton2").attribute("PosY").as_int();
	Skeleton2SpawnPointX2 = config.child("skeleton2").attribute("PosX2").as_int();
	Skeleton2SpawnPointY2 = config.child("skeleton2").attribute("PosY2").as_int();
	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	CurrentMap = MapList.start->data;
	//CurrentMap = "FirstLevel.tmx";

	scene_changed = false;
	secret_map = false;
	input = true;
	sceneChangeTimer = false;
	cameraTracking = false;
	QuitToDesktop = false;

	App->map->Load(CurrentMap.GetString()); //Load the map
	App->audio->PlayMusic("audio/music.ogg");

	App->ui->time.Start();
	App->ui->pLife = 3;

	CreateEnt();
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY("Update_Scene", Profiler::Color::CornflowerBlue)

	//Save and load:

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		App->LoadGame();
	}

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		App->SaveGame();
	}

	//Debug keys:

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		StartFirstLevel();
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		StartSecondLevel();
	}

	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		RestartCurrentLevel();
	}

	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
	{
		App->map->drawSecretMap();
	}

	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) 
	{ 
		if (App->framerate == 30) { App->framerate = 60; }
		else if (App->framerate == 60) { App->framerate = 30; }
	}

	if (App->input->GetKey(SDL_SCANCODE_GRAVE) == KEY_DOWN)
	{
		if (App->cons->active == true) { App->cons->active = false; input = true; }
		else if (App->cons->active == false) { App->cons->active = true; input = false; }
	}

	App->map->Draw();
	App->entManager->DrawEntities(dt);

	if (cameraTracking) { App->render->cameraFollowingPlayer(App->render->cameraPos.x, App->render->cameraPos.y); }
	else 
	{
		if (App->render->camera.x > -550)App->render->camera.y = -400;
		if(App->render->camera.x <= -550) App->render->camera.y --;
		App->render->camera.x -= 4;

		if (App->render->camera.x <= -1000) { cameraTracking = true; }
	}


	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) 
	{ 
		App->ui->InGameMenu = !App->ui->InGameMenu;
	}

	if (QuitToDesktop) { return false; }

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG(false, "Freeing scene");
	App->entManager->CleanUp();
	App->map->CleanUp();

	return true;
}

void j1Scene::Disable()
{
	if (active == true)
	{
		active = false;
		CleanUp();
		App->entManager->CleanUp();
	}
}

bool j1Scene::Load(pugi::xml_node& data)
{
	LOG(true, "Loading scene state");
	//Loading the map the player was on
	App->scene->CurrentMap = data.child("map").attribute("currentMap").as_string();
	if (App->scene->CurrentMap == "SecondLevel.tmx")
	{
		App->scene->StartSecondLevel();
	}
	else if (App->scene->CurrentMap == "FirstLevel.tmx")
	{
		App->scene->StartFirstLevel();
	}
	//Loading whether or not the secret map was visible for the player when he saved
	App->scene->secret_map = data.child("map").attribute("secretMap").as_bool();
	return true;
}

// Save Game State
bool j1Scene::Save(pugi::xml_node& data) const
{
	LOG(true, "Saving scene state");
	pugi::xml_node sceneNode = data.append_child("map");
	//Saving the map where the player is
	sceneNode.append_attribute("currentMap") = App->scene->CurrentMap.GetString();
	//Saving whether or not the secret map is visible for the player
	sceneNode.append_attribute("secretMap") = App->scene->secret_map;
	return true;
}

//Restartint current level
void j1Scene::RestartCurrentLevel()
{
	//App->entManager->RestartEntities();
	if (CurrentMap == "FirstLevel.tmx") { Create1MapEnemies(); }
	if (CurrentMap == "SecondLevel.tmx") { Create2MapEnemies(); }
	player->position.x = PlayerSpawnPointX;
	player->position.y = PlayerSpawnPointY;
	if (App->ui->pLife < 3) { App->ui->pLife = 3; LOG(true, "Player Lifes: %i", App->ui->pLife); }
	LOG(true, "Restarting Current Level");
}

//Restarting the first level
void j1Scene::StartFirstLevel()
{
	//If the level was restarted by pressing F1
	App->map->CleanUp();
	CurrentMap.create("FirstLevel.tmx");
	App->map->Load(CurrentMap.GetString());
	App->entManager->RestartEntities();
	if (App->ui->pLife < 3) { App->ui->pLife = 3; LOG(true, "Player Lifes: %i", App->ui->pLife); }
	LOG(true, "Starting First Level");
	
	//If the level was restarted by getting there while playing
	/*if (sceneChangeTimer == false)
	{
		scene_change_timer = SDL_GetTicks();
		App->fade->FadeToBlackVisualEffect(2);
		sceneChangeTimer = true;
		p2List_item<j1Entities*>* entityList = App->entManager->entities.start;
		while (entityList) {
			if (entityList->data->entity_type == j1Entities::Types::player) {
				entityList->data->SetPlayerState(j1Entities::playerStates::PLAYER_IDLE);
			}
			entityList = entityList->next;
			
		}
		input = false;
	}

	if (SDL_GetTicks() - scene_change_timer > 1040)
	{
		App->map->CleanUp();
		CurrentMap.create("FirstLevel.tmx");
		App->map->Load(CurrentMap.GetString());
		App->entManager->RestartEntities();
		Create1MapEnemies();
		sceneChangeTimer = false;
		input = true;
		if (App->ui->pLife < 3) { App->ui->pLife += 1; LOG(true, "Player Lifes: %i", App->ui->pLife); }
		LOG(true, "Starting First Level");
	}*/

}

//Restarting the second level
void j1Scene::StartSecondLevel()
{
	App->map->CleanUp();
	CurrentMap.create("SecondLevel.tmx");
	App->map->Load(CurrentMap.GetString());
	App->entManager->RestartEntities();
	Create2MapEnemies();
	secret_map = false;
	if (App->ui->pLife < 3) { App->ui->pLife += 1; LOG(true, "Player Lifes: %i", App->ui->pLife); }
	LOG(true, "Starting Second Level");
}

void j1Scene::EndScene()
{
	p2List_item<j1Entities*>* entityList = App->entManager->entities.start;
	while (entityList) {
		if (entityList->data->entity_type == j1Entities::Types::player) {
			entityList->data->SetPlayerState(j1Entities::playerStates::PLAYER_IDLE);
		}
		entityList = entityList->next;

	}
	input = false;
	LOG(true, "Ending Main Scene");
	App->fade->FadeToBlack(App->main_menu, this, 2.f);
}

void j1Scene::CreateEnt()
{
	player = App->entManager->CreateEntity(j1Entities::Types::player, { PlayerSpawnPointX, PlayerSpawnPointY }, false);
	bat1 = App->entManager->CreateEntity(j1Entities::Types::bat, { BatSpawnPointX, BatSpawnPointY }, false);
	skeleton1 = App->entManager->CreateEntity(j1Entities::Types::skeleton, { SkeletonSpawnPointX, SkeletonSpawnPointY }, false);
	bat2 = App->entManager->CreateEntity(j1Entities::Types::bat, { Bat2SpawnPointX, Bat2SpawnPointY }, false);
	skeleton2 = App->entManager->CreateEntity(j1Entities::Types::skeleton, { Skeleton2SpawnPointX, Skeleton2SpawnPointY }, false);
}

void j1Scene::Create1MapEnemies()
{
	App->entManager->DestroyEnemies();
	bat1 = App->entManager->CreateEntity(j1Entities::Types::bat, { BatSpawnPointX, BatSpawnPointY }, false);
	skeleton1 = App->entManager->CreateEntity(j1Entities::Types::skeleton, { SkeletonSpawnPointX, SkeletonSpawnPointY }, false);
	bat2 = App->entManager->CreateEntity(j1Entities::Types::bat, { Bat2SpawnPointX, Bat2SpawnPointY }, false);
	skeleton2 = App->entManager->CreateEntity(j1Entities::Types::skeleton, { Skeleton2SpawnPointX, Skeleton2SpawnPointY}, false);
}

void j1Scene::Create2MapEnemies()
{
	App->entManager->DestroyEnemies();
	bat1 = App->entManager->CreateEntity(j1Entities::Types::bat, { BatSpawnPointX2, BatSpawnPointY2 }, false);
	skeleton1 = App->entManager->CreateEntity(j1Entities::Types::skeleton, { SkeletonSpawnPointX2, SkeletonSpawnPointY2}, false);
	//bat2 = App->entManager->CreateEntity(j1Entities::Types::bat, { Bat2SpawnPointX2, Bat2SpawnPointY2 }, false);
	skeleton2 = App->entManager->CreateEntity(j1Entities::Types::skeleton, { Skeleton2SpawnPointX2, Skeleton2SpawnPointY2 }, false);
}
