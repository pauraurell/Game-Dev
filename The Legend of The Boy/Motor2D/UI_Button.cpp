#include "j1App.h"
#include "UI_Button.h"
#include "j1Input.h"
#include "j1UI.h"

Button::Button() {

	this->type = TYPE_UI::UI_BUTTON;
	texture = App->ui->ui_tex;
}

Button::~Button() {

}


bool Button::Awake(pugi::xml_node&)
{

	return true;
}

bool Button::Start()
{
	if (text != nullptr)
		label = App->ui->Add_UIelement(TYPE_UI::UI_LABEL, this, { Position.x + 16, Position.y + rect.h/ 3}, size, true, { 0,0,0,0 }, text);

	return true;
}

bool Button::PreUpdate()
{
	if (label != nullptr)
		label->enabled = enabled;

	above = Is_above();

	return true;
}

bool Button::Update(float dt)
{
	if (above)
	{
		if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
			Click();
	}
	return true;
}

bool Button::PostUpdate()
{
	if (enabled)
		Draw();

	return true;
}

bool Button::CleanUp()
{
	label->enabled = false;
	return true;
}

