#include "j1Console.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1Fonts.h"

j1Console::j1Console()
{
	active = false;
}

j1Console::~j1Console()
{

}

bool j1Console::Start()
{
	cons.x = App->render->camera.x * -1 / 2;
	cons.y = App->render->camera.y * 1 / 1000;
	cons.w = App->render->camera.w;
	cons.h = App->render->camera.h / 2;

	return true;
}

bool j1Console::PreUpdate()
{
	return true;
}

bool j1Console::Update(float dt)
{
	if (active == true) { input = true; }
	else if (active == false) { input = false; }

	return true;
}

bool j1Console::PostUpdate()
{
	if (active == true)
	{
		SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, 200);
		SDL_RenderFillRect(App->render->renderer, &cons);

		App->render->Blit(tex, cons.x, cons.y);
	}

	return true;
}

bool j1Console::CleanUp()
{
	return true;
}

void j1Console::Log(const char* string)
{
	if (active == true)
	{
		tex = App->fonts->PrintTextBox(string, { 255, 255, 255, 255 }, App->fonts->default, cons.w);
	}
}