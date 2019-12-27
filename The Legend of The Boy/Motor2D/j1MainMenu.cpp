#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "j1MainMenu.h"
#include "j1Scene.h"
#include "j1Window.h"
#include "j1UI.h"
#include "j1FadeToBlack.h"
#include "j1Scene.h"
#include "j1Map.h"

j1MainMenu::j1MainMenu() : j1Module()
{
	name.create("intro");
}

// Destructor
j1MainMenu::~j1MainMenu()
{}

// Called before render is available
bool j1MainMenu::Awake(pugi::xml_node& config)
{
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1MainMenu::Start()
{
	texture = App->tex->Load("textures/UI/atlas.png");

	testLabel = App->ui->AddGUIelement(TYPE_UI::UI_LABEL, nullptr, { 230, 250 }, { 0,0 }, false, false, { 0,0,1000,1000 }, "MUSIC", this);

	return true;
}

// Called each loop iteration
bool j1MainMenu::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1MainMenu::Update(float dt)
{
	App->render->Blit(texture, 0, 0);
	return true;
}

// Called each loop iteration
bool j1MainMenu::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) 
	{
		App->fade->FadeToBlack(App->scene, this, 2.f);
	}

	return ret;
}

// Called before quitting
bool j1MainMenu::CleanUp()
{
	App->tex->UnLoad(texture);
	return true;
}