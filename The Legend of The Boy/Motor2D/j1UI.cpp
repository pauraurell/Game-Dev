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
#include "j1Audio.h"
#include "UI_Image.h"
#include "j1Input.h"
#include "j1FadeToBlack.h"
#include "UI_Slider.h"
#include "j1Score.h"

j1UI::j1UI()
{
	debug = false;

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

	pts = 0;

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

	if (App->scene->cameraTracking == false)
	{
		if (App->scene->pLife < 3) { App->scene->pLife = 3; }
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

	/*if (App->scene->active) 
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
		pts_image->enabled = true;
		pts_label->enabled = true;
		SettingsButton->enabled = true;

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

		p2SString* string = new p2SString("%i", App->score->coins);
		coin_label->text = string->GetString();

		string->create("%i", pts);
		pts_label->text = string->GetString();

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
		pts_image->enabled = false;
		pts_label->enabled = false;
		SettingsButton->enabled = false;
	}*/
}


UIelement* j1UI::Add_UIelement(TYPE_UI type, SLIDER_TYPE typeOfScroll,  UIelement* parent, iPoint Position, int size, bool enabled, SDL_Rect section, iPoint PositionOffset, const char* text, j1Module* listener, bool CanDrag, SDL_Color color)
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
		ui_element->canMoveIn_X_axis = CanDrag;
		ui_element->color = color;

		UIelements.add(ui_element)->data->Start();
	}

	return ui_element;
}

