#include "j1App.h"
#include "UI_Button.h"
#include "j1Input.h"
#include "j1UI.h"

Button::Button() {

	this->type = TYPE_UI::UI_BUTTON;
}

Button::~Button() {

}


bool Button::Awake(pugi::xml_node&)
{

	return true;
}

bool Button::Start()
{

	return true;
}

bool Button::PreUpdate()
{
	return true;
}

bool Button::Update(float dt)
{
	
	return true;
}

bool Button::PostUpdate()
{
	Draw();

	return true;
}

bool Button::CleanUp()
{
	return true;
}
