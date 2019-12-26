#include "j1UI.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1EntityManager.h"
#include "Brofiler/Brofiler.h"

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
}

j1UI::~j1UI()
{}

bool j1UI::Start()
{
	ui_tex = App->tex->Load("textures/UI/atlas.png");

	return true;
}

bool j1UI::PreUpdate()
{
	return true;
}

bool j1UI::Update(float dt)
{
	Draw();
	return true;
}

bool j1UI::PostUpdate()
{


	return true;
}

bool j1UI::CleanUp()
{
	App->tex->UnLoad(ui_tex);

	return true;
}

void j1UI::Draw()
{
	BROFILER_CATEGORY("Draw_UI", Profiler::Color::PowderBlue)
	
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
}
