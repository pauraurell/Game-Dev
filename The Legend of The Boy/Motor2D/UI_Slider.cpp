#include "j1App.h"
#include "j1Input.h"
#include "j1Fonts.h"
#include "j1Render.h"
#include "UI_Slider.h"
#include "j1Audio.h"

Slider::Slider(SLIDER_TYPE typeOfScroll) {

	this->type = TYPE_UI::UI_SLIDER;
	this->type_of_slider = typeOfScroll;
}

Slider::~Slider() {

}


bool Slider::Awake(pugi::xml_node&)
{

	return true;
}


bool Slider::Start()
{
	texture = App->ui->ui_tex;

	button = App->ui->Add_UIelement(TYPE_UI::UI_BUTTON, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { Position.x , Position.y}, 20, false, { 339,42,72,46 }, { 0,0 }, nullptr);
	button->Position.y = Position.y - button->section.h / 2 + this->section.h / 2;
	button_position = 0;

	if (this->type_of_slider == SLIDER_TYPE::Fx)
	{
		button_position = App->audio->FXvolume;

		button->Position.x = -((int)App->audio->FXvolume * (this->section.w - button->section.w)) / 128;
	}

	else if (this->type_of_slider == SLIDER_TYPE::Music)
	{
		button_position = App->audio->MUSICvolume;

		button->Position.x = -((int)App->audio->MUSICvolume * (this->section.w - button->section.w)) / 128;
	}

	return true;
}

bool Slider::PreUpdate()
{
	above = Is_above();

	return true;
}

bool Slider::Update(float dt)
{
	button->enabled = true;
	if (above && enabled)
	{
		if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
			Click();
	}

	return true;
}

bool Slider::PostUpdate()
{
	if (enabled)
		Draw();

	return true;
}

bool Slider::CleanUp()
{
	return true;
}
