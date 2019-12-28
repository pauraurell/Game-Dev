#include "UI_Image.h"
#include "j1App.h"

Image::Image() 
{
	this->type = TYPE_UI::UI_IMAGE;

	texture = App->ui->ui_tex;
}

Image::~Image() {

}


bool Image::Awake(pugi::xml_node&)
{

	return true;
}

bool Image::PreUpdate()
{

	return true;
}

bool Image::PostUpdate()
{
	if (enabled)
		Draw();

	return true;
}

bool Image::CleanUp()
{
	return true;
}

