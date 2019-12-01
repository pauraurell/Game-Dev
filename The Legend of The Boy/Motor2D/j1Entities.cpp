#include "j1Entities.h"
#include "Animation.h"
#include "j1App.h"
#include "j1Player.h"
#include "j1Bat.h"
#include "j1Skeleton.h"
#include "j1Textures.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1Collision.h"
#include "j1Particles.h"

j1Entities::j1Entities(Types type)
{
	entity_type = type;
}

j1Entities::~j1Entities()
{}

bool j1Entities::Load(pugi::xml_node& data)
{
	position.x = data.child("position").attribute("posX").as_int();
	position.y = data.child("position").attribute("posY").as_int();
	return true;
}

bool j1Entities::Save(pugi::xml_node& data) const
{
	data.append_attribute("position");

	data.append_attribute("type") = name.GetString();
	data.child("position").append_attribute("posX") = position.x;
	data.child("position").append_attribute("posY") = position.y;

	return true;

}

void j1Entities::Draw(float dt)
{}
