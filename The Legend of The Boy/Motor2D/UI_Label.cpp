#include "UI_Label.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"

Label::Label()
{
	this->type = TYPE_UI::UI_LABEL;
	this->size = size;
}

Label::~Label() {

}

bool Label::Awake(pugi::xml_node&)
{

	return true;
}

bool Label::Start()
{
	texture = App->fonts->Print(text, size,{ 15, 15, 15, 255 }, App->fonts->default);
	return true;
}


bool Label::PreUpdate()
{
	CleanUp();
	App->fonts->CalcSize(App->input->GetText().GetString(), section.w, section.h);
	return true;
}

bool Label::Update(float dt)
{
	texture = App->fonts->Print(text, size, { 15, 15, 15, 255 }, App->fonts->default);
	return true;
}

bool Label::PostUpdate()
{
	if (enabled)
	{
		App->render->Blit(texture, Position.x + App->render->camera.x/-SCALE,  Position.y + App->render->camera.y / -SCALE, nullptr);
	}

	return true;
}

bool Label::CleanUp()
{
	App->tex->UnLoad(texture);
	return true;
}
