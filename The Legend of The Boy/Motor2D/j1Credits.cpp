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
#include "J1MainMenu.h"

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

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT || App->input->GetMouseButtonDown(1)) { vel = 10; }
	else { vel = 2; }

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		App->fade->FadeToBlack(App->main_menu, this, 2.f);
	}

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