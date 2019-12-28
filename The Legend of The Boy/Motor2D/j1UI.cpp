#include "j1UI.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1MainMenu.h"
#include "j1EntityManager.h"
#include "Brofiler/Brofiler.h"
#include "UI_Button.h"
#include "UI_Label.h"
#include "UI_Image.h"
#include "j1Input.h"
#include "j1FadeToBlack.h"
#include "UI_Slider.h"
#include "j1Score.h"

j1UI::j1UI()
{
	pLife = 3;
	heart.x = 0;
	heart.y = 0;
	heart.w = 26;
	heart.h = 24;
	emptHeart.x = 27;
	emptHeart.y = 0;
	emptHeart.w = 26;
	emptHeart.h = 24;

	debug = false;
	InGameMenu = false;
}

j1UI::~j1UI()
{}

bool j1UI::Awake(pugi::xml_node& config)
{
	bool ret = true;

	return ret;
}

bool j1UI::Start()
{
	ui_tex = App->tex->Load("textures/UI/atlas.png");

	CreateInGameUi();

	CreateInGameMenuUi();

	timer = new p2SString();

	return true;
}

bool j1UI::PreUpdate()
{
	bool ret = true;
	p2List_item<UIelement*>* tmp = UIelements.start;
	while (tmp != nullptr)
	{
		ret = tmp->data->PreUpdate();
		tmp = tmp->next;
	}

	return ret;
}

bool j1UI::Update(float dt)
{
	bool ret = true;
	p2List_item<UIelement*>* tmp = UIelements.start;
	while (tmp != nullptr)
	{
		ret = tmp->data->Update(dt);
		tmp = tmp->next;
	}

	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		debug = !debug;
	}

	return ret;
}

bool j1UI::PostUpdate()
{
	bool ret = true;

	p2List_item<UIelement*>* tmp = UIelements.start;
	while (tmp != nullptr)
	{
		ret = tmp->data->PostUpdate();
		tmp = tmp->next;
	}

	Draw();
	return ret;
}

bool j1UI::CleanUp()
{
	App->tex->UnLoad(ui_tex);

	for (p2List_item<UIelement*>* item = UIelements.start; item; item = item->next)
	{
		item->data->CleanUp();
	}
	UIelements.clear();

	return true;
}

void j1UI::Draw()
{
	BROFILER_CATEGORY("Draw_UI", Profiler::Color::PowderBlue)

	if (App->scene->active) 
	{
		if (pLife == 3)
		{
			App->render->Blit(ui_tex, App->render->camera.x * -1 / 2 + 4, App->render->camera.y * -1 / 2 + 4, &heart, SDL_FLIP_NONE, 1.0f, 1.0f, 0.0);
			App->render->Blit(ui_tex, App->render->camera.x * -1 / 2 + 32, App->render->camera.y * -1 / 2 + 4, &heart, SDL_FLIP_NONE, 1.0f, 1.0f, 0.0);
			App->render->Blit(ui_tex, App->render->camera.x * -1 / 2 + 60, App->render->camera.y * -1 / 2 + 4, &heart, SDL_FLIP_NONE, 1.0f, 1.0f, 0.0);
		}
		else if (pLife == 2)
		{
			App->render->Blit(ui_tex, App->render->camera.x * -1 / 2 + 4, App->render->camera.y * -1 / 2 + 4, &heart, SDL_FLIP_NONE, 1.0f, 1.0f, 0.0);
			App->render->Blit(ui_tex, App->render->camera.x * -1 / 2 + 32, App->render->camera.y * -1 / 2 + 4, &heart, SDL_FLIP_NONE, 1.0f, 1.0f, 0.0);
			App->render->Blit(ui_tex, App->render->camera.x * -1 / 2 + 60, App->render->camera.y * -1 / 2 + 4, &emptHeart, SDL_FLIP_NONE, 1.0f, 1.0f, 0.0);
		}
		else if (pLife == 1)
		{
			App->render->Blit(ui_tex, App->render->camera.x * -1 / 2 + 4, App->render->camera.y * -1 / 2 + 4, &heart, SDL_FLIP_NONE, 1.0f, 1.0f, 0.0);
			App->render->Blit(ui_tex, App->render->camera.x * -1 / 2 + 32, App->render->camera.y * -1 / 2 + 4, &emptHeart, SDL_FLIP_NONE, 1.0f, 1.0f, 0.0);
			App->render->Blit(ui_tex, App->render->camera.x * -1 / 2 + 60, App->render->camera.y * -1 / 2 + 4, &emptHeart, SDL_FLIP_NONE, 1.0f, 1.0f, 0.0);
		}
		else
		{
			App->render->Blit(ui_tex, App->render->camera.x* -1 / 2 + 4, App->render->camera.y* -1 / 2 + 4, &emptHeart, SDL_FLIP_NONE, 1.0f, 1.0f, 0.0);
			App->render->Blit(ui_tex, App->render->camera.x* -1 / 2 + 32, App->render->camera.y* -1 / 2 + 4, &emptHeart, SDL_FLIP_NONE, 1.0f, 1.0f, 0.0);
			App->render->Blit(ui_tex, App->render->camera.x* -1 / 2 + 60, App->render->camera.y* -1 / 2 + 4, &emptHeart, SDL_FLIP_NONE, 1.0f, 1.0f, 0.0);
		}

		coin_image->enabled = true;
		coin_label->enabled = true;
		timer_image->enabled = true;
		timer_label->enabled = true;

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

	else 
	{ 
		coin_image->enabled = false;
		coin_label->enabled = false;
		timer_image->enabled = false;
		timer_label->enabled = false;
	}
}


UIelement* j1UI::Add_UIelement(TYPE_UI type, SLIDER_TYPE typeOfScroll,  UIelement* parent, iPoint Position, int size, bool enabled, SDL_Rect section, iPoint PositionOffset, const char* text, j1Module* listener)
{
	UIelement* ui_element = nullptr;

	switch (type)
	{

	case TYPE_UI::UI_BUTTON:
		ui_element = new Button();
		break;

	case TYPE_UI::UI_LABEL:
		ui_element = new Label();
		break;

	case TYPE_UI::UI_IMAGE:
		ui_element = new Image();
		break;

	case TYPE_UI::UI_SLIDER:
		ui_element = new Slider(typeOfScroll);
		break;

	}

	if (ui_element !=nullptr)
	{
		ui_element->parent = parent;		
		ui_element->listener = listener;	
		ui_element->Position = Position;
		ui_element->enabled = enabled;
		ui_element->section = section;
		ui_element->text = text;
		ui_element->size = size;
		ui_element->posOffset = PositionOffset;

		UIelements.add(ui_element)->data->Start();
	}

	return ui_element;
}

void j1UI::CreateInGameMenuUi()
{
	inGameMenu_image = Add_UIelement(TYPE_UI::UI_IMAGE, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 166, 66 }, 20, false, { 1,114,197,272 }, { 0,0 }, nullptr, this);
	inGameMenu_label_settings = Add_UIelement(TYPE_UI::UI_LABEL, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { inGameMenu_image->Position.x + 46,  inGameMenu_image->Position.y + 10 }, 20, false, { 0,0,0,0 }, { 0,0 }, "Settings", this);
	inGameMenu_button_QuitToDesktop = Add_UIelement(TYPE_UI::UI_BUTTON, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { inGameMenu_image->Position.x + 27, inGameMenu_image->Position.y + 216 }, 15, false, { 176,42,145,46 }, { -7,-2 }, "Quit to desktop", this);
	inGameMenu_button_Save = Add_UIelement(TYPE_UI::UI_BUTTON, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { inGameMenu_image->Position.x + 23, inGameMenu_image->Position.y + 120 }, 17, false, { 339,42,72,46 }, { 0,0 }, "Save", this);
	inGameMenu_button_Load = Add_UIelement(TYPE_UI::UI_BUTTON, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { inGameMenu_image->Position.x + 102, inGameMenu_image->Position.y + 120 }, 17, false, { 339,42,72,46 }, { 0,0 }, "Load", this);
	inGameMenu_button_MainMenu = Add_UIelement(TYPE_UI::UI_BUTTON, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { inGameMenu_image->Position.x + 27, inGameMenu_image->Position.y + 166 }, 17, false, { 230,114,145,46 }, { 0,0 }, "Main Menu", this);
	SliderTest = App->ui->Add_UIelement(TYPE_UI::UI_SLIDER, SLIDER_TYPE::Fx, nullptr, { inGameMenu_image->Position.x + 10,  inGameMenu_image->Position.y + 50 }, 20, false, { 284, 62, 167, 4 }, { 0,0 }, nullptr, this);
}

void j1UI::CreateInGameUi()
{
	coin_image = App->ui->Add_UIelement(TYPE_UI::UI_IMAGE, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 4, 35 }, 20, false, { 62,0,22,22 }, { 0,0 }, nullptr, this);
	coin_label = App->ui->Add_UIelement(TYPE_UI::UI_LABEL, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 32, 36 }, 20,  false, { 0,0,0,0 }, { 0,0 }, "0", this);
	timer_image = App->ui->Add_UIelement(TYPE_UI::UI_IMAGE, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 7, 65 }, 20, false, { 89,0,15,22 }, { 0,0 }, nullptr, this);
	timer_label = App->ui->Add_UIelement(TYPE_UI::UI_LABEL, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 32, 66 }, 20, false, { 0,0,0,0 }, { 0,0 }, "00:00", this);
}

void j1UI::UIevents(uiEvent type, UIelement* element)
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
	}
	}
}

void j1UI::EnableAll()
{
	inGameMenu_image->enabled = true;
	inGameMenu_label_settings->enabled = true;
	inGameMenu_button_QuitToDesktop->enabled = true;
	inGameMenu_button_Save->enabled = true;
	inGameMenu_button_Load->enabled = true;
	inGameMenu_button_MainMenu->enabled = true;
}

void j1UI::DisableAll()
{
	inGameMenu_image->enabled = false;
	inGameMenu_label_settings->enabled = false;
	inGameMenu_button_QuitToDesktop->enabled = false;
	inGameMenu_button_Save->enabled = false;
	inGameMenu_button_Load->enabled = false;
	inGameMenu_button_MainMenu->enabled = false;
}
