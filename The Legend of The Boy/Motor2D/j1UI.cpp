#include "j1UI.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1EntityManager.h"
#include "Brofiler/Brofiler.h"
#include "UI_Button.h"
#include "UI_Label.h"
#include "UI_Image.h"
#include "j1Input.h"

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

		if (InGameMenu) 
		{
			inGameMenu_image->enabled = true;
			inGameMenu_label_settings->enabled = true;
			inGameMenu_button_QuitToDesktop->enabled = true;
		}
		else 
		{ 
			inGameMenu_image->enabled = false; 
			inGameMenu_label_settings->enabled = false;
			inGameMenu_button_QuitToDesktop->enabled = false;
		}
	}

	else 
	{ 
		coin_image->enabled = false;
		coin_label->enabled = false;
	}
}

UIelement* j1UI::Add_UIelement(TYPE_UI type, UIelement* parent, iPoint Position, int size, bool enabled, SDL_Rect section, char* text, j1Module* listener)
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

	}

	if (ui_element !=nullptr)
	{
		ui_element->parent = parent;		
		ui_element->listener = listener;	
		ui_element->Position = Position;
		ui_element->enabled = enabled;
		ui_element->rect = section;
		ui_element->text = text;
		ui_element->size = size;

		UIelements.add(ui_element)->data->Start();
	}

	return ui_element;
}

void j1UI::CreateInGameMenuUi()
{
	inGameMenu_image =Add_UIelement(TYPE_UI::UI_IMAGE, nullptr, { 190, 120 }, 20, false, { 1,114,197,230 }, nullptr, this);
	inGameMenu_label_settings = Add_UIelement(TYPE_UI::UI_LABEL, nullptr, { inGameMenu_image->Position.x + 46,  inGameMenu_image->Position.y + 10 }, 20, false, { 0,0,0,0 }, "Settings", this);
	inGameMenu_button_QuitToDesktop = Add_UIelement(TYPE_UI::UI_BUTTON, nullptr, { inGameMenu_image->Position.x + 27, inGameMenu_image->Position.y + 140 }, 15, false, { 176,42,145,46 }, "Quit to desktop", this);
}

void j1UI::CreateInGameUi()
{
	coin_image = App->ui->Add_UIelement(TYPE_UI::UI_IMAGE, nullptr, { 4, 35 }, 20, false, { 62,0,22,22 }, nullptr, this);
	coin_label = App->ui->Add_UIelement(TYPE_UI::UI_LABEL, nullptr, { 32, 36 }, 20,  false, { 0,0,0,0 }, "0", this);
}

void j1UI::UIevents(uiEvent type, UIelement* element)
{

	switch (type)
	{
	case uiEvent::EVENT_ONCLICK:
	{
		if (element == inGameMenu_button_QuitToDesktop)
		{
			App->scene->QuitToDesktop = true;
		}
	}
	}

}