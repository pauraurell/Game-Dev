#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "j1Credits.h"
#include "j1Scene.h"
#include "j1Window.h"
#include "j1UI.h"
#include "j1FadeToBlack.h"
#include "j1Scene.h"
#include "j1Map.h"

j1Credits::j1Credits() : j1Module()
{
	name.create("intro");
}

// Destructor
j1Credits::~j1Credits()
{}

// Called before render is available
bool j1Credits::Awake(pugi::xml_node& config)
{
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Credits::Start()
{
	licTex = App->tex->Load("textures/Credits/Credits.png");

	enabled = true;
	vel = 2;

	/*license.add(App->ui->AddGUIelement(TYPE_UI::UI_LABEL, nullptr, { 50, 10 }, { 0,0 }, false, { 0,0,0,0 }, "MIT License", this));
	license.add(App->ui->AddGUIelement(TYPE_UI::UI_LABEL, nullptr, { 30, 50 }, { 0,0 }, false, { 0,0,0,0 }, "Copyright (c) 2019 pauraurell and pgalmor is hereby", this));
	license.add(App->ui->AddGUIelement(TYPE_UI::UI_LABEL, nullptr, { 30, 70 }, { 0,0 }, false, { 0,0,0,0 }, "granted, free of charge, to any person obtaining a copy", this));
	license.add(App->ui->AddGUIelement(TYPE_UI::UI_LABEL, nullptr, { 30, 90 }, { 0,0 }, false, { 0,0,0,0 }, "of this software and associated documentation files (the", this));
	license.add(App->ui->AddGUIelement(TYPE_UI::UI_LABEL, nullptr, { 30, 110 }, { 0,0 }, false, { 0,0,0,0 }, "-Software-), to deal in the Software without restriction,", this));
	license.add(App->ui->AddGUIelement(TYPE_UI::UI_LABEL, nullptr, { 30, 130 }, { 0,0 }, false, { 0,0,0,0 }, "including without limitation the rights to use, copy, modify,", this));
	license.add(App->ui->AddGUIelement(TYPE_UI::UI_LABEL, nullptr, { 30, 150 }, { 0,0 }, false, { 0,0,0,0 }, "merge, publish, distribute, sublicense, and/or sell copies", this));
	license.add(App->ui->AddGUIelement(TYPE_UI::UI_LABEL, nullptr, { 30, 170 }, { 0,0 }, false, { 0,0,0,0 }, "of the Software, and to permit persons to whom the", this));
	license.add(App->ui->AddGUIelement(TYPE_UI::UI_LABEL, nullptr, { 30, 190 }, { 0,0 }, false, { 0,0,0,0 }, "Software is furnished to do so, subject to the following", this));
	license.add(App->ui->AddGUIelement(TYPE_UI::UI_LABEL, nullptr, { 30, 210 }, { 0,0 }, false, { 0,0,0,0 }, "conditions:", this));
	license.add(App->ui->AddGUIelement(TYPE_UI::UI_LABEL, nullptr, { 30, 250 }, { 0,0 }, false, { 0,0,0,0 }, "The above copyright notice and this permission notice", this));
	license.add(App->ui->AddGUIelement(TYPE_UI::UI_LABEL, nullptr, { 30, 270 }, { 0,0 }, false, { 0,0,0,0 }, "shall be included in all copies or substantial portions", this));
	license.add(App->ui->AddGUIelement(TYPE_UI::UI_LABEL, nullptr, { 30, 290 }, { 0,0 }, false, { 0,0,0,0 }, "portions of the Software.", this));
	license.add(App->ui->AddGUIelement(TYPE_UI::UI_LABEL, nullptr, { 30, 330 }, { 0,0 }, false, { 0,0,0,0 }, "THE SOFTWARE IS PROVIDED -AS IS-,", this));
	license.add(App->ui->AddGUIelement(TYPE_UI::UI_LABEL, nullptr, { 30, 355 }, { 0,0 }, false, { 0,0,0,0 }, "WITHOUT WARRANTY OF ANY KIND,", this));
	license.add(App->ui->AddGUIelement(TYPE_UI::UI_LABEL, nullptr, { 30, 380 }, { 0,0 }, false, { 0,0,0,0 }, "EXPRESS OR IMPLIED, INCLUDING BUT NOT", this));
	license.add(App->ui->AddGUIelement(TYPE_UI::UI_LABEL, nullptr, { 30, 405 }, { 0,0 }, false, { 0,0,0,0 }, "LIMITED TO THE WARRANTIES OF", this));
	license.add(App->ui->AddGUIelement(TYPE_UI::UI_LABEL, nullptr, { 30, 430 }, { 0,0 }, false, { 0,0,0,0 }, "MERCHANTABILITY, FITNESS FOR A", this));
	license.add(App->ui->AddGUIelement(TYPE_UI::UI_LABEL, nullptr, { 30, 455 }, { 0,0 }, false, { 0,0,0,0 }, "PARTICULAR PURPOSE AND", this));
	license.add(App->ui->AddGUIelement(TYPE_UI::UI_LABEL, nullptr, { 30, 480 }, { 0,0 }, false, { 0,0,0,0 }, "NONINFRINGEMENT. IN NO EVENT SHALL", this));
	license.add(App->ui->AddGUIelement(TYPE_UI::UI_LABEL, nullptr, { 30, 505 }, { 0,0 }, false, { 0,0,0,0 }, "THE AUTHORS OR COPYRIGHT HOLDERS BE", this));
	license.add(App->ui->AddGUIelement(TYPE_UI::UI_LABEL, nullptr, { 30, 530 }, { 0,0 }, false, { 0,0,0,0 }, "LIABLE FOR ANY CLAIM, DAMAGES OR", this));
	license.add(App->ui->AddGUIelement(TYPE_UI::UI_LABEL, nullptr, { 30, 555 }, { 0,0 }, false, { 0,0,0,0 }, "OTHER LIABILITY, WHETHER IN AN ACTION", this));
	license.add(App->ui->AddGUIelement(TYPE_UI::UI_LABEL, nullptr, { 30, 580 }, { 0,0 }, false, { 0,0,0,0 }, "OF CONTRACT, TORT OR OTHERWISE,", this));
	license.add(App->ui->AddGUIelement(TYPE_UI::UI_LABEL, nullptr, { 30, 605 }, { 0,0 }, false, { 0,0,0,0 }, "ARISING FROM, OUT OF OR IN CONNECTION", this));
	license.add(App->ui->AddGUIelement(TYPE_UI::UI_LABEL, nullptr, { 30, 630 }, { 0,0 }, false, { 0,0,0,0 }, "WITH THE SOFTWARE OR THE USE OR", this));
	license.add(App->ui->AddGUIelement(TYPE_UI::UI_LABEL, nullptr, { 30, 655 }, { 0,0 }, false, { 0,0,0,0 }, "OTHER DEALINGS IN THE SOFTWARE.", this));*/

	return true;
}

// Called each loop iteration
bool j1Credits::PreUpdate()
{

	/*if (App->cred->IsEnabled() == false && enabled == true)
	{
		p2List_item<UIelement*>* item = license.start;
		while (item)
		{
			item->data->enabled = false;
			item = item->next;
		}
		enabled = false;
	}
	else if(App->cred->IsEnabled() == true && enabled == false)
	{
		p2List_item<UIelement*>* item = license.start;
		while (item)
		{
			item->data->enabled = true;
			item = item->next;
		}
		enabled = true;
	}*/

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) { vel = 10; }
	else { vel = 2; }

	return true;
}

// Called each loop iteration
bool j1Credits::Update(float dt)
{
	Draw();
	return true;
}

// Called each loop iteration
bool j1Credits::PostUpdate()
{
	bool ret = true;

	if (App->render->camera.y > CREDLIMIT) { App->render->camera.y -= vel; }
	if (App->render->camera.y < CREDLIMIT) { App->render->camera.y = CREDLIMIT; }

	return ret;
}

// Called before quitting
bool j1Credits::CleanUp()
{
	/*p2List_item<UIelement*>* item = license.start;
	while (item)
	{
		item->data->CleanUp();
		license.del(item);
		item = item->next;
	}
	license.clear();*/
	App->tex->UnLoad(licTex);

	return true;
}

bool j1Credits::Draw()
{
	App->render->Blit(licTex, -5, 20);

	return true;
}