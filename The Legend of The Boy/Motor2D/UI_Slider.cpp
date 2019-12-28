#include "j1App.h"
#include "j1Input.h"
#include "j1Fonts.h"
#include "j1Render.h"
#include "UI_Slider.h"

Slider::Slider() {

	this->type = TYPE_UI::UI_SLIDER;
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
	return true;
}

bool Slider::PreUpdate()
{
	above = Is_above();

	return true;
}

bool Slider::Update(float dt)
{
	if (above)
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
