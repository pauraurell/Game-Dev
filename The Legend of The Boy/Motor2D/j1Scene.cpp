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
#include "j1Score.h"
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
	App->ui->Start();
	timer = new p2SString();
	CurrentMap = MapList.start->data;
	//CurrentMap = "FirstLevel.tmx";

	scene_changed = false;
	secret_map = false;
	input = false;
	sceneChangeTimer = false;
	cameraTracking = false;
	sceneEnded = false;
	to_end = false;

	App->map->Load(CurrentMap.GetString()); //Load the map
	App->audio->PlayMusic("audio/music.ogg");

	pLife = 3;
	heart.x = 0;
	heart.y = 0;
	heart.w = 26;
	heart.h = 24;
	emptHeart.x = 27;
	emptHeart.y = 0;
	emptHeart.w = 26;
	emptHeart.h = 24;
	InGameMenu = false;
	App->score->enemies = 0;
	App->score->coins = 0;

	CreateEnt();

	CreateInGameUi();
	CreateInGameMenuUi();

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
		scene_changed = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		StartSecondLevel();
		scene_changed = false;
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
		if (App->frameratecap == 30) { App->frameratecap = 60; }
		else if (App->frameratecap == 60) { App->frameratecap = 30; }
	}

	if (App->input->GetKey(SDL_SCANCODE_GRAVE) == KEY_DOWN)
	{
		if (App->cons->active == true) { App->cons->active = false; input = true; App->cons->EmptyBuffer(); }
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
		input = false;

		if (App->render->camera.x <= -1000) { cameraTracking = true; time.Start(); input = true; }
	}

	if (to_end == true) { to_end = false; EndScene(); }

	p2SString* string = new p2SString("%i", App->score->coins);
	coin_label->text = string->GetString();

	string->create("%i", App->ui->pts);
	pts_label->text = string->GetString();

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) 
	{ 
		InGameMenu = !InGameMenu;
		input = !input;
	}

	//last thing to draw: ui
	drawSceneUi();

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG(false, "Freeing scene");
	App->entManager->CleanUp();
	App->map->CleanUp();
	coin_image->enabled = false;
	coin_label->enabled = false;
	timer_image->enabled = false;
	timer_label->enabled = false;
	SettingsButton->enabled = false;
	pts_image->enabled = false;
	pts_label->enabled = false;

	App->ui->CleanUp();
	return true;
}

void j1Scene::Enable()
{
	if (active == false)
	{
		active = true;
		Start();
		App->score->Init(true);
		App->cons->Init(true);
	}
}

void j1Scene::Disable()
{
	if (active == true)
	{
		active = false;
		CleanUp();
		App->score->Disable();
		App->cons->Disable();
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
	if (CurrentMap == "FirstLevel.tmx") { Create1MapEnemies(); player->position.x = PlayerSpawnPointX; }
	if (CurrentMap == "SecondLevel.tmx") { Create2MapEnemies(); player->position.x = PlayerSpawnPointX2; }
	player->position.y = PlayerSpawnPointY;
	if (App->scene->pLife < 3) { App->scene->pLife = 3; LOG(true, "Player Lifes: %i", App->scene->pLife); }
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
	Create1MapEnemies();
	scene_changed = true;
	time.Start();
	App->score->coins = 0;
	App->score->enemies = 0;
	if (App->scene->pLife < 3) { App->scene->pLife = 3; LOG(true, "Player Lifes: %i", App->scene->pLife); }
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
	scene_changed = true;
	secret_map = false;
	if (App->scene->pLife < 3) { App->scene->pLife += 1; LOG(true, "Player Lifes: %i", App->scene->pLife); }
	LOG(true, "Starting Second Level");
}

void j1Scene::EndScene()
{
	InGameMenu = false;
	DisableAll();

	p2List_item<j1Entities*>* entityList = App->entManager->entities.start;
	while (entityList) {
		if (entityList->data->entity_type == j1Entities::Types::player) {
			entityList->data->SetPlayerState(j1Entities::playerStates::PLAYER_IDLE);
		}
		entityList = entityList->next;

	}
	input = false;
	App->score->DisableAll();
	App->score->draw = false;
	App->render->camera.x = 0;
	App->render->camera.y = 0;
	LOG(true, "Ending Main Scene");
	App->fade->FadeToBlack(App->main_menu, this, 2.f);
}

void j1Scene::CreateEnt()
{
	player = App->entManager->CreateEntity(j1Entities::Types::player, { PlayerSpawnPointX, PlayerSpawnPointY }, false);
	coin = App->entManager->CreateEntity(j1Entities::Types::coin, { PlayerSpawnPointX + 60, PlayerSpawnPointY }, false);
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

void j1Scene::drawSceneUi()
{
	if (pLife == 3)
	{
		App->render->Blit(App->ui->ui_tex, App->render->camera.x * -1 / 2 + 4, App->render->camera.y * -1 / 2 + 4, &heart, SDL_FLIP_NONE, 1.0f, 1.0f, 0.0);
		App->render->Blit(App->ui->ui_tex, App->render->camera.x * -1 / 2 + 32, App->render->camera.y * -1 / 2 + 4, &heart, SDL_FLIP_NONE, 1.0f, 1.0f, 0.0);
		App->render->Blit(App->ui->ui_tex, App->render->camera.x * -1 / 2 + 60, App->render->camera.y * -1 / 2 + 4, &heart, SDL_FLIP_NONE, 1.0f, 1.0f, 0.0);
	}
	else if (pLife == 2)
	{
		App->render->Blit(App->ui->ui_tex, App->render->camera.x * -1 / 2 + 4, App->render->camera.y * -1 / 2 + 4, &heart, SDL_FLIP_NONE, 1.0f, 1.0f, 0.0);
		App->render->Blit(App->ui->ui_tex, App->render->camera.x * -1 / 2 + 32, App->render->camera.y * -1 / 2 + 4, &heart, SDL_FLIP_NONE, 1.0f, 1.0f, 0.0);
		App->render->Blit(App->ui->ui_tex, App->render->camera.x * -1 / 2 + 60, App->render->camera.y * -1 / 2 + 4, &emptHeart, SDL_FLIP_NONE, 1.0f, 1.0f, 0.0);
	}
	else if (pLife == 1)
	{
		App->render->Blit(App->ui->ui_tex, App->render->camera.x * -1 / 2 + 4, App->render->camera.y * -1 / 2 + 4, &heart, SDL_FLIP_NONE, 1.0f, 1.0f, 0.0);
		App->render->Blit(App->ui->ui_tex, App->render->camera.x * -1 / 2 + 32, App->render->camera.y * -1 / 2 + 4, &emptHeart, SDL_FLIP_NONE, 1.0f, 1.0f, 0.0);
		App->render->Blit(App->ui->ui_tex, App->render->camera.x * -1 / 2 + 60, App->render->camera.y * -1 / 2 + 4, &emptHeart, SDL_FLIP_NONE, 1.0f, 1.0f, 0.0);
	}
	else
	{
		App->render->Blit(App->ui->ui_tex, App->render->camera.x* -1 / 2 + 4, App->render->camera.y* -1 / 2 + 4, &emptHeart, SDL_FLIP_NONE, 1.0f, 1.0f, 0.0);
		App->render->Blit(App->ui->ui_tex, App->render->camera.x* -1 / 2 + 32, App->render->camera.y* -1 / 2 + 4, &emptHeart, SDL_FLIP_NONE, 1.0f, 1.0f, 0.0);
		App->render->Blit(App->ui->ui_tex, App->render->camera.x* -1 / 2 + 60, App->render->camera.y* -1 / 2 + 4, &emptHeart, SDL_FLIP_NONE, 1.0f, 1.0f, 0.0);
	}

	coin_image->enabled = true;
	coin_label->enabled = true;
	timer_image->enabled = true;
	timer_label->enabled = true;
	SettingsButton->enabled = true;
	pts_image->enabled = true;
	pts_label->enabled = true;
	
	if (App->scene->cameraTracking == true)
	{
		App->score->time = time.ReadSec();
		min = App->score->time / 60;
		sec = App->score->time - (min * 60);

		if (min < 10)
		{
			if (sec < 10) { timer->create("0%i:0%i", min, sec); }
			else if (sec >= 10) { timer->create("0%i:%i", min, sec); }
		}
		else if (min >= 10)
		{
			if (sec < 10) { timer->create("%i:0%i", min, sec); }
			else if (sec >= 10) { timer->create("%i:%i", min, sec); }
		}

		timer_label->text = timer->GetString();

	}
	else { timer_label->text = "00:00"; }

	p2SString* coins = new p2SString("%i", App->score->coins);
	coin_label->text = coins->GetString();

	//----- In Game Menu -----// ---------------------------------
	if (InGameMenu)
	{
		EnableAll();
	}
	else
	{
		DisableAll();
	}
	//------------------------------------------------------------
}


void j1Scene::CreateInGameMenuUi()
{
	inGameMenu_image = App->ui->Add_UIelement(TYPE_UI::UI_IMAGE, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 166, 30 }, 20, false, { 1,114,197,322 }, { 0,0 }, nullptr, this);
	inGameMenu_label_settings = App->ui->Add_UIelement(TYPE_UI::UI_LABEL, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { inGameMenu_image->Position.x + 46,  inGameMenu_image->Position.y + 10 }, 20, false, { 0,0,0,0 }, { 0,0 }, "Settings", this);
	inGameMenu_button_QuitToDesktop = App->ui->Add_UIelement(TYPE_UI::UI_BUTTON, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { inGameMenu_image->Position.x + 27, inGameMenu_image->Position.y + 216 }, 15, false, { 176,42,145,46 }, { -7,-2 }, "Quit to desktop", this);
	inGameMenu_button_Save = App->ui->Add_UIelement(TYPE_UI::UI_BUTTON, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { inGameMenu_image->Position.x + 23, inGameMenu_image->Position.y + 120 }, 17, false, { 339,42,72,46 }, { 0,0 }, "Save", this);
	inGameMenu_button_Load = App->ui->Add_UIelement(TYPE_UI::UI_BUTTON, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { inGameMenu_image->Position.x + 102, inGameMenu_image->Position.y + 120 }, 17, false, { 339,42,72,46 }, { 0,0 }, "Load", this);
	inGameMenu_button_MainMenu = App->ui->Add_UIelement(TYPE_UI::UI_BUTTON, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { inGameMenu_image->Position.x + 27, inGameMenu_image->Position.y + 166 }, 17, false, { 230,114,145,46 }, { 0,0 }, "Main Menu", this);
	SliderMusic = App->ui->Add_UIelement(TYPE_UI::UI_SLIDER, SLIDER_TYPE::Music, nullptr, { inGameMenu_image->Position.x + 45,  inGameMenu_image->Position.y + 52 }, 20, false, { 220, 248, 128, 4 }, { 0,0 }, nullptr, this);
	SliderFx = App->ui->Add_UIelement(TYPE_UI::UI_SLIDER, SLIDER_TYPE::Fx, nullptr, { inGameMenu_image->Position.x + 45,  inGameMenu_image->Position.y + 90 }, 20, false, { 220, 248, 128, 4 }, { 0,0 }, nullptr, this);
	label_music = App->ui->Add_UIelement(TYPE_UI::UI_LABEL, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { inGameMenu_image->Position.x + 15,  inGameMenu_image->Position.y + 52 }, 20, false, { 0, 0, 0, 0 }, { 0,0 }, "Music", this);
	label_fx = App->ui->Add_UIelement(TYPE_UI::UI_LABEL, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { inGameMenu_image->Position.x + 15,  inGameMenu_image->Position.y + 90 }, 20, false, { 0,0,0,0 }, { 0,0 }, "Fx", this);
	inGameMenu_button_Continue = App->ui->Add_UIelement(TYPE_UI::UI_BUTTON, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { inGameMenu_image->Position.x + 35, inGameMenu_image->Position.y + 269 }, 17, false, { 223,285,132,37 }, { 0,0 }, "Continue", this);
}

void j1Scene::CreateInGameUi()
{
	coin_image = App->ui->Add_UIelement(TYPE_UI::UI_IMAGE, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 4, 35 }, 20, false, { 62,0,22,22 }, { 0,0 }, nullptr, this);
	coin_label = App->ui->Add_UIelement(TYPE_UI::UI_LABEL, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 32, 36 }, 20, false, { 0,0,0,0 }, { 0,0 }, "0", this);
	timer_image = App->ui->Add_UIelement(TYPE_UI::UI_IMAGE, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 7, 65 }, 20, false, { 89,0,15,22 }, { 0,0 }, nullptr, this);
	timer_label = App->ui->Add_UIelement(TYPE_UI::UI_LABEL, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 32, 66 }, 20, false, { 0,0,0,0 }, { 0,0 }, "00:00", this);
	SettingsButton = App->ui->Add_UIelement(TYPE_UI::UI_BUTTON, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 460 , 6 }, 17, false, { 388,243,40,36 }, { 0,0 }, nullptr, this);
	pts_image = App->ui->Add_UIelement(TYPE_UI::UI_IMAGE, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 4, 94 }, 20, false, { 108,0,21,22 }, { 0,0 }, nullptr, this);
	pts_label = App->ui->Add_UIelement(TYPE_UI::UI_LABEL, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 32, 96 }, 20, false, { 0,0,0,0 }, { 0,0 }, "00", this);
}

void j1Scene::UIevents(uiEvent type, UIelement* element)
{

	switch (type)
	{
	case uiEvent::EVENT_ONCLICK:
	{
		if (element == inGameMenu_button_QuitToDesktop)
		{
			App->QuitToDesktop = true;
		}

		else if (element == inGameMenu_button_MainMenu)
		{
			App->render->camera.x = 0;
			App->render->camera.y = 0;
			App->scene->EndScene();
		}

		else if (element == inGameMenu_button_Save)
		{
			App->SaveGame();
		}

		else if (element == inGameMenu_button_Load)
		{
			App->LoadGame();
		}

		else if (element == inGameMenu_button_Continue)
		{
			InGameMenu = false;
		}

		else if (element == SettingsButton)
		{
			InGameMenu = !InGameMenu;
		}
	}
	}
}

void j1Scene::EnableAll()
{
	inGameMenu_image->enabled = true;
	inGameMenu_label_settings->enabled = true;
	inGameMenu_button_QuitToDesktop->enabled = true;
	inGameMenu_button_Save->enabled = true;
	inGameMenu_button_Load->enabled = true;
	inGameMenu_button_MainMenu->enabled = true;
	SliderMusic->enabled = true;
	SliderFx->enabled = true;
	label_music->enabled = true;
	label_fx->enabled = true;
	inGameMenu_button_Continue->enabled = true;
}

void j1Scene::DisableAll()
{
	inGameMenu_image->enabled = false;
	inGameMenu_label_settings->enabled = false;
	inGameMenu_button_QuitToDesktop->enabled = false;
	inGameMenu_button_Save->enabled = false;
	inGameMenu_button_Load->enabled = false;
	inGameMenu_button_MainMenu->enabled = false;
	SliderMusic->enabled = false;
	SliderFx->enabled = false;
	label_music->enabled = false;
	label_fx->enabled = false;
	inGameMenu_button_Continue->enabled = false;
}


