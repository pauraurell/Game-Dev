#include "j1Score.h"
#include "j1App.h"

j1Score::j1Score()
{

}

j1Score::~j1Score()
{}

bool j1Score::Awake(pugi::xml_node& config)
{
	return true;
}

bool j1Score::Start()
{
	draw = false;

	return true;
}

bool j1Score::PreUpdate()
{
	return true;
}

bool j1Score::Update(float dt)
{
	return true;
}

bool j1Score::PostUpdate()
{
	return true;
}

bool j1Score::CleanUp()
{
	return true;
}

bool j1Score::Draw()
{
	return true;
}