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
#include "j1Player.h"
#include "j1FadeToBlack.h"


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

	LOG("Loading Scene");

	for (map = config.child("map"); map; map = map.next_sibling("map"))
	{
		p2SString* level = new p2SString();

		level->create(map.attribute("name").as_string());
		MapList.add(level->GetString());
	}

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	CurrentMap = MapList.start->data;

	App->map->Load(CurrentMap.GetString()); //Load the map
	App->audio->PlayMusic("audio/music.ogg");

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
	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y -= 1;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y += 1;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 2;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 2;

	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		App->fade->FadeToBlack(3.5f);

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		App->LoadGame();
	}

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		App->SaveGame();
	}

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		manualFirstLevel = true;
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

	App->map->Draw();
	App->player->Draw();
	App->render->cameraFollowingPlayer(App->player->position.x, App->player->position.y);

	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d MouseCoordinates:%d,%d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count());
				

	App->win->SetTitle(title.GetString());

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool j1Scene::Load(pugi::xml_node& data)
{
	LOG("Loading scene state");
	App->scene->CurrentMap = data.child("map").attribute("currentMap").as_string();
	if (App->scene->CurrentMap == "SecondLevel.tmx")
	{
		App->scene->StartSecondLevel();
	}
	else if (App->scene->CurrentMap == "FirstLevel.tmx")
	{
		manualFirstLevel = true;
		App->scene->StartFirstLevel();
	}
	return true;
}

// Save Game State
bool j1Scene::Save(pugi::xml_node& data) const
{
	LOG("Saving scene state");
	pugi::xml_node sceneNode = data.append_child("map");
	sceneNode.append_attribute("currentMap") = App->scene->CurrentMap.GetString();
	return true;
}

void j1Scene::RestartCurrentLevel()
{
	App->player->position.x = App->player->SpawnPointX;
	App->player->position.y = App->player->SpawnPointY;
}

void j1Scene::StartFirstLevel()
{
	if (manualFirstLevel == true)
	{
		App->map->CleanUp();
		CurrentMap.create("FirstLevel.tmx");
		App->map->Load(CurrentMap.GetString());
		RestartCurrentLevel();
		scene_change = false;
		manualFirstLevel = false;
	}
	
	else if (manualFirstLevel == false)
	{
		if (sceneChangeTimer == false)
		{
			scene_change_timer = SDL_GetTicks();
			App->fade->FadeToBlack(2);
			sceneChangeTimer = true;
			App->player->SetPlayerState(PLAYER_IDLE);
			App->player->input = false;
		}

		if (SDL_GetTicks() - scene_change_timer > 1040)
		{
			App->map->CleanUp();
			CurrentMap.create("FirstLevel.tmx");
			App->map->Load(CurrentMap.GetString());
			RestartCurrentLevel();
			scene_change = false;
			sceneChangeTimer = false;
			App->player->input = true;
		}
	}
}

void j1Scene::StartSecondLevel()
{
	App->map->CleanUp();
	CurrentMap.create("SecondLevel.tmx");
	App->map->Load(CurrentMap.GetString());
	RestartCurrentLevel();
	scene_change = true;
}

