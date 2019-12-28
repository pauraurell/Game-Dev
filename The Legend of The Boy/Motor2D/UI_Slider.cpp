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
	
	button = App->ui->Add_UIelement(TYPE_UI::UI_BUTTON, SLIDER_TYPE::NOT_A_SLIDER, this, Position, 20, false, { 220, 256, 6 , 10 }, posOffset, nullptr, this->listener, true);
	button->Position.y = Position.y - button->section.h / 2 + section.h / 2;
	button_position = 0;

	if (type_of_slider == SLIDER_TYPE::Fx)
	{
		button->posOffset.x = -((int)App->audio->volume_fx * (section.w - button->section.w)) / 100; button->Position.x = Position.x - button->posOffset.x;
	}
	else if (type_of_slider == SLIDER_TYPE::Music)
	{
		button->posOffset.x = -((int)App->audio->volume_music * (section.w - button->section.w)) / 100; button->Position.x = Position.x - button->posOffset.x;
	}

	return true;
}

bool Slider::PreUpdate()
{
	if (enabled) { button->enabled = true; }
	above = Is_above();

	return true;
}

bool Slider::Update(float dt)
{
	if (above && enabled)
	{
		if (App->input->GetMouseButtonDown(1) == KEY_DOWN) { Click(); }
	}

	return true;
}
bool Slider::PostUpdate()
{
	int v = 0;
	float volume = ((float)-button->posOffset.x / 122.f) *100.f;

	if (button->posOffset.x > 0)
	{
		button->posOffset.x = 0;
		button->Position.x = button->parent->Position.x - button->posOffset.x;
	}
	else if (button->posOffset.x < (-section.w + button->section.w))
	{
		button->posOffset.x = -section.w + button->section.w;
		button->Position.x = button->parent->Position.x - button->posOffset.x;
	}

	v = (int)volume;

	/*if (this->type_of_slider == SLIDER_TYPE::Fx)
	{
	
	}*/
	if (this->type_of_slider == SLIDER_TYPE::Music)
	{
		Mix_VolumeMusic(v);
	}

	if (enabled) { Draw(); }

	return true;
}

bool Slider::CleanUp()
{
	return true;
}

