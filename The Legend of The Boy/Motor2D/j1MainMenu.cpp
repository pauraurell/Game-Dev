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
#include "j1Credits.h"

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
	texture = App->tex->Load("textures/Background.png");

	testButton = App->ui->Add_UIelement(TYPE_UI::UI_BUTTON, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 50, 200 }, 22, false, { 1,38,153,53 }, { 0,-1 }, "Play", this);
	exitButton = App->ui->Add_UIelement(TYPE_UI::UI_BUTTON, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 50, 270 }, 22, false, { 220,174,153,53 }, { 0,-1 }, "Exit", this);
	creditsButton = App->ui->Add_UIelement(TYPE_UI::UI_BUTTON, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 300, 270 }, 22, false, { 230,114,145,46 }, { 6,-8 }, "Credits", this);
	return true;
}

// Called each loop iteration
bool j1MainMenu::PreUpdate()
{
	App->render->camera.y = 0;

	if (App->main_menu->IsEnabled() == false) 
	{
		testButton->enabled = false; 
		exitButton->enabled = false;
		creditsButton->enabled = false;
	}
	else 
	{
		testButton->enabled = true; 
		exitButton->enabled = true;
		creditsButton->enabled = true;
	}

	return true;
}

// Called each loop iteration
bool j1MainMenu::Update(float dt)
{
	Draw();
	return true;
}

// Called each loop iteration
bool j1MainMenu::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) 
	{
		testButton->enabled = false;
		exitButton->enabled = false;
		creditsButton->enabled = false;
		App->fade->FadeToBlack(App->scene, this, 2.f);
	}

	return ret;
}

// Called before quitting
bool j1MainMenu::CleanUp()
{
	App->tex->UnLoad(texture);
	testButton->CleanUp();
	exitButton->CleanUp();
	creditsButton->CleanUp();
	return true;
}

bool j1MainMenu::Draw()
{
	App->render->Blit(texture, 0, 0);
	return true;
}

void j1MainMenu::UIevents(uiEvent type, UIelement* element)
{

	switch (type)
	{
		case uiEvent::EVENT_ONCLICK:
		{
			if (element == testButton) 
			{ 
				App->fade->FadeToBlack(App->scene, this, 2.f);
				testButton->enabled = false;
				exitButton->enabled = false;
				creditsButton->enabled = false;
			}

			else if (element == exitButton)
			{
				App->QuitToDesktop = true;
			}

			else if (element == creditsButton)
			{
				App->fade->FadeToBlack(App->cred, this, 2.f);
				testButton->enabled = false;
				exitButton->enabled = false;
				creditsButton->enabled = false;
			}
		}
	}

}