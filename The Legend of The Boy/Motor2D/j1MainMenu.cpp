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
	texture = App->tex->Load("textures/Background.png");

	testLabel = App->ui->Add_UIelement(TYPE_UI::UI_LABEL, nullptr, { 50, 250 }, false, { 0,0,0,0 }, "Press Space to Continue", this);
	testButton = App->ui->Add_UIelement(TYPE_UI::UI_BUTTON, nullptr, { 50, 300 }, false, { 1,38,153,53 }, "Press here", this);
	return true;
}

// Called each loop iteration
bool j1MainMenu::PreUpdate()
{
	//App->render->camera.x = 0;
	App->render->camera.y = 0;
	if (App->main_menu->IsEnabled() == false) { testButton->enabled = false; testLabel->enabled = false;}
	else { testButton->enabled = true; testLabel->enabled = true; }

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
		testLabel->enabled = false;
		testButton->enabled = false;
		App->fade->FadeToBlack(App->scene, this, 2.f);
	}

	return ret;
}

// Called before quitting
bool j1MainMenu::CleanUp()
{
	App->tex->UnLoad(texture);
	testButton->CleanUp();
	testLabel->CleanUp();
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
				testLabel->enabled = false;
				testButton->enabled = false;
			}
		}
	}

}