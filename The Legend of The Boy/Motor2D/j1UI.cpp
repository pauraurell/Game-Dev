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
	heart.w = 39;
	heart.h = 36;

	emptHeart.x = 0;
	emptHeart.y = 41;
	emptHeart.w = 39;
	emptHeart.h = 36;
}

j1UI::~j1UI()
{}

bool j1UI::Start()
{
	ui_tex = App->tex->Load("textures/ui.png");

	return true;
}

bool j1UI::PreUpdate()
{
	return true;
}

bool j1UI::Update(float dt)
{
	return true;
}

bool j1UI::PostUpdate()
{
	Draw();

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
	
	App->render->Blit(ui_tex, App->render->camera.x, App->render->camera.y, &heart, SDL_FLIP_NONE, 1.0f, 1.0f, 0.0);
}
