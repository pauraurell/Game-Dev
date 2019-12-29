#include "j1App.h"
#include "UI_Button.h"
#include "j1Input.h"
#include "j1Audio.h"
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
	click = App->audio->LoadFx("audio/button_click.wav");	
	above = App->audio->LoadFx("audio/button_hover.wav");
	if (text != nullptr) { ButtonLabel = App->ui->Add_UIelement(TYPE_UI::UI_LABEL, SLIDER_TYPE::NOT_A_SLIDER, this, { Position.x + 16 + posOffset.x, Position.y + section.h / 3 + posOffset.y }, size, true, { 0,0,0,0 }, { 0,0 }, text); }
	return true;
}

bool Button::PreUpdate()
{
	if (ButtonLabel != nullptr) { ButtonLabel->enabled = enabled; }

	return true;
}

bool Button::Update(float dt)
{
	if (OnTop() && enabled)
	{
		if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
		{
			Click();
		}
		
		if (App->input->GetMouseButtonDown(1) == KEY_REPEAT)
		{
			iPoint pos_click = { 0,0 };App->input->GetMousePosition(pos_click.x, pos_click.y);
			MouseMovement = { pos_click.x - (this->Position.x) };
			if (canMoveIn_X_axis) { drag = true; }
		}
	}

	if (drag) 
	{
		if (App->input->GetMouseButtonDown(1) == KEY_IDLE || App->input->GetMouseButtonDown(1) == KEY_UP) {drag = false;}
			
		else
		{
			Drag(dt);
		}
	}

	return true;
}

bool Button::PostUpdate()
{
	if (enabled) { Draw(); }
	
	return true;
}

bool Button::CleanUp()
{
	if (ButtonLabel != nullptr) { ButtonLabel->enabled = false; }
	return true;
}

void Button::Drag(float dt)
{
	iPoint MousePos = { 0,0 };
	App->input->GetMousePosition(MousePos.x, MousePos.y);
	iPoint currentposition = this->Position;

	if (canMoveIn_X_axis) { this->Position.x += ((MousePos.x - this->Position.x) - MouseMovement)*1/2; }
		
	if (parent != nullptr)
	{
		if (canMoveIn_X_axis) 
		{
			this->posOffset.x += currentposition.x - this->Position.x;
			this->Position.x = parent->Position.x - posOffset.x;
		}
	}
}
