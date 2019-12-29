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
#include "SDL/include/SDL.h"

j1MainMenu::j1MainMenu() : j1Module()
{
	name.create("intro");
	SettingsMenuIsActive = false;
	stop = false;
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
	App->ui->Start();
	texture1 = App->tex->Load("maps/Layer 1.png");
	texture2 = App->tex->Load("maps/Layer 2.png");
	texture3 = App->tex->Load("maps/Layer 3.png");
	title = App->tex->Load("textures/Title/The Legend of The Boy.png");
	posX1 = 0;
	posX2 = 0;
	posX3 = 0;

	testButton = App->ui->Add_UIelement(TYPE_UI::UI_BUTTON, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 50, 170 }, 22, false, { 1,38,153,53 }, { 0,-1 }, "Play", this);
	ContinueButton = App->ui->Add_UIelement(TYPE_UI::UI_BUTTON, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 50, 224 }, 22, false, { 1,38,153,53 }, { 0,-1 }, "Continue", this);
	exitButton = App->ui->Add_UIelement(TYPE_UI::UI_BUTTON, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 50, 310 }, 22, false, { 220,174,153,53 }, { 0,-1 }, "Exit", this);
	creditsButton = App->ui->Add_UIelement(TYPE_UI::UI_BUTTON, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 300, 210 }, 22, false, { 230,114,145,46 }, { 6,-8 }, "Credits", this);
	settingsButton = App->ui->Add_UIelement(TYPE_UI::UI_BUTTON, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 300, 170 }, 22, false, { 176,42,145,46 }, { 6,-8 }, "Settings", this);

	SettingsImage = App->ui->Add_UIelement(TYPE_UI::UI_IMAGE, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 285, 217 }, 20, false, { 213,333,180,144 }, { 0,0 }, nullptr, this);
	settingsLabel = App->ui->Add_UIelement(TYPE_UI::UI_LABEL, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 299, 226 }, 18, false, { 0,0,0,0 }, { 0,0 }, "Audio Settings", this);
	settingsButtonOk = App->ui->Add_UIelement(TYPE_UI::UI_BUTTON, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 344 ,324 }, 20, false, { 423,42,61,31 }, { 0,-4 }, "OK", this);
	sliderMusic = App->ui->Add_UIelement(TYPE_UI::UI_SLIDER, SLIDER_TYPE::Music, nullptr, { 310 , 265 }, 20, false, { 220, 248, 128, 4 }, { 0,0 }, nullptr, this);
	sliderFx = App->ui->Add_UIelement(TYPE_UI::UI_SLIDER, SLIDER_TYPE::Fx, nullptr, { 310 , 300 }, 20, false, { 220, 248, 128, 4 }, { 0,0 }, nullptr, this);
	sliderMusicLabel = App->ui->Add_UIelement(TYPE_UI::UI_LABEL, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 340, 245 }, 17, false, { 0,0,0,0 }, { 0,0 }, "Music", this);
	sliderFxLabel = App->ui->Add_UIelement(TYPE_UI::UI_LABEL, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 350, 280 }, 17, false, { 0,0,0,0 }, { 0,0 }, "Fx", this);
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
		settingsButton->enabled = false;
		ContinueButton->enabled = false;
	}
	else 
	{
		testButton->enabled = true; 
		exitButton->enabled = true;
		settingsButton->enabled = true;
		ContinueButton->enabled = true;

		if (SettingsMenuIsActive)
		{
			SettingsImage->enabled = true;
			settingsLabel->enabled = true;
			sliderFx->enabled = true;
			sliderMusic->enabled = true;
			settingsButtonOk->enabled = true;
			sliderMusicLabel->enabled = true;
			sliderFxLabel->enabled = true;
			creditsButton->enabled = false;
		}

		else
		{
			SettingsImage->enabled = false;
			settingsLabel->enabled = false;
			sliderFx->enabled = false;
			sliderMusic->enabled = false;
			settingsButtonOk->enabled = false;
			sliderMusicLabel->enabled = false;
			sliderFxLabel->enabled = false;
			creditsButton->enabled = true;
		}
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

	return ret;
}

// Called before quitting
bool j1MainMenu::CleanUp()
{
	testButton = nullptr;
	exitButton = nullptr;
	creditsButton = nullptr;
	settingsButton = nullptr;
	ContinueButton = nullptr;
	settingsButtonOk = nullptr;
	sliderFx = nullptr;
	sliderFxLabel = nullptr;
	sliderMusic = nullptr;
	sliderMusicLabel = nullptr;
	App->tex->UnLoad(title);
	App->tex->UnLoad(texture1);
	App->tex->UnLoad(texture2);
	App->tex->UnLoad(texture3);
	App->ui->CleanUp();

	return true;
}

bool j1MainMenu::Draw()
{
	posX1--; posX2 = posX2 - 2; posX3 = posX3 - 7; 
	SDL_Rect r; r = { 0,0,2000,276 };
	App->render->Blit(texture1, posX1, 0, &r, SDL_FLIP_NONE);
	App->render->Blit(texture2, posX2, 40, &r, SDL_FLIP_NONE);
	App->render->Blit(texture3, posX3, 108, &r, SDL_FLIP_NONE);
	if (posX3 <= -1024) posX3 = 0; 
	if (posX2 <= -1024) posX2 = 0;
	if (posX1 <= -1024) posX1 = 0;
	App->render->Blit(title, 52, 8);

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
				
			}

			else if (element == exitButton)
			{
				App->QuitToDesktop = true;
			}

			else if (element == creditsButton)
			{
				App->fade->FadeToBlack(App->cred, this, 2.f);

			}

			else if (element == settingsButton)
			{
				SettingsMenuIsActive = !SettingsMenuIsActive;
			}

			else if (element == settingsButtonOk)
			{
				SettingsMenuIsActive = !SettingsMenuIsActive;
			}
		}
	}

}