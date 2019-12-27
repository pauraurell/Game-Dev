#include "UI_Label.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"

Label::Label()
{
	this->type = TYPE_UI::UI_LABEL;
}

Label::~Label() {

}

bool Label::Awake(pugi::xml_node&)
{

	return true;
}

bool Label::Start()
{
	texture = App->fonts->Print(text);
	return true;
}


bool Label::PreUpdate()
{
	CleanUp();
	App->fonts->CalcSize(App->input->GetText().GetString(), rect.w, rect.h);
	return true;
}

bool Label::Update(float dt)
{
	texture = App->fonts->Print(text);
	return true;
}

bool Label::PostUpdate()
{
	if (enabled)
	{
		App->render->Blit(texture, globalPosition.x + localPosition.x, globalPosition.y + localPosition.y, nullptr);
	}

	return true;
}

bool Label::CleanUp()
{
	App->tex->UnLoad(texture);
	return true;
}
