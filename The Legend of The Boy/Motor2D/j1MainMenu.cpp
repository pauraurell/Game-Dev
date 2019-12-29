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
	SettingsMenuIsActive = false;
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

	testButton = App->ui->Add_UIelement(TYPE_UI::UI_BUTTON, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 50, 60 }, 22, false, { 1,38,153,53 }, { 0,-1 }, "Play", this);
	ContinueButton = App->ui->Add_UIelement(TYPE_UI::UI_BUTTON, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 50, 120 }, 22, false, { 1,38,153,53 }, { 0,-1 }, "Continue", this);
	exitButton = App->ui->Add_UIelement(TYPE_UI::UI_BUTTON, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 50, 270 }, 22, false, { 220,174,153,53 }, { 0,-1 }, "Exit", this);
	creditsButton = App->ui->Add_UIelement(TYPE_UI::UI_BUTTON, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 300, 300 }, 22, false, { 230,114,145,46 }, { 6,-8 }, "Credits", this);
	settingsButton = App->ui->Add_UIelement(TYPE_UI::UI_BUTTON, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 50, 190 }, 22, false, { 176,42,145,46 }, { 6,-8 }, "Settings", this);
	SettingsImage = App->ui->Add_UIelement(TYPE_UI::UI_IMAGE, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 265, 137 }, 20, false, { 213,333,180,144 }, { 0,0 }, nullptr, this);
	settingsLabel = App->ui->Add_UIelement(TYPE_UI::UI_LABEL, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 279, 146 }, 18, false, { 0,0,0,0 }, { 0,0 }, "Audio Settings", this);
	settingsButtonOk = App->ui->Add_UIelement(TYPE_UI::UI_BUTTON, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 324 ,244 }, 20, false, { 423,42,61,31 }, { 0,-4 }, "OK", this);
	sliderMusic = App->ui->Add_UIelement(TYPE_UI::UI_SLIDER, SLIDER_TYPE::Music, nullptr, { 290 , 185 }, 20, false, { 220, 248, 128, 4 }, { 0,0 }, nullptr, this);
	sliderFx = App->ui->Add_UIelement(TYPE_UI::UI_SLIDER, SLIDER_TYPE::Fx, nullptr, { 290 , 220 }, 20, false, { 220, 248, 128, 4 }, { 0,0 }, nullptr, this);
	sliderMusicLabel = App->ui->Add_UIelement(TYPE_UI::UI_LABEL, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 320, 165 }, 17, false, { 0,0,0,0 }, { 0,0 }, "Music", this);
	sliderFxLabel = App->ui->Add_UIelement(TYPE_UI::UI_LABEL, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 330, 200 }, 17, false, { 0,0,0,0 }, { 0,0 }, "Fx", this);
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
		settingsButton->enabled = false;
		ContinueButton->enabled = false;
	}
	else 
	{
		testButton->enabled = true; 
		exitButton->enabled = true;
		creditsButton->enabled = true;
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
	App->tex->UnLoad(texture);
	testButton->CleanUp();
	exitButton->CleanUp();
	creditsButton->CleanUp();
	settingsButton->CleanUp();
	ContinueButton->CleanUp();
	settingsButtonOk->CleanUp();
	sliderFx->CleanUp();
	sliderFxLabel->CleanUp();
	sliderMusic->CleanUp();
	sliderMusicLabel->CleanUp();

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
				settingsButton->enabled = false;
				ContinueButton->enabled = false;
				SettingsImage->enabled = false;
				settingsLabel->enabled = false;
				sliderFx->enabled = false;
				sliderMusic->enabled = false;
				settingsButtonOk->enabled = false;
				sliderMusicLabel->enabled = false;
				sliderFxLabel->enabled = false;
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
				settingsButton->enabled = false;
				ContinueButton->enabled = false;
				SettingsImage->enabled = false;
				settingsLabel->enabled = false;
				sliderFx->enabled = false;
				sliderMusic->enabled = false;
				settingsButtonOk->enabled = false;
				sliderMusicLabel->enabled = false;
				sliderFxLabel->enabled = false;
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