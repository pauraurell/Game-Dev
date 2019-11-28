#include "j1EntityManager.h"
#include "j1Entities.h"
#include "j1Player.h"
#include "j1Bat.h"
#include "j1Scene.h"
#include "Animation.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Particles.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Audio.h"
#include "PugiXml/src/pugixml.hpp"
#include "Brofiler/Brofiler.h"

j1EntityManager::j1EntityManager()
{
	
}

j1EntityManager::~j1EntityManager()
{}

bool j1EntityManager::Awake(pugi::xml_node& config) {
	node = config;
	return true;
}

bool j1EntityManager::Start()
{
	return true;
}

bool j1EntityManager::PreUpdate()
{
	return true;
}

bool j1EntityManager::Update(float dt)
{
	return true;
}

bool j1EntityManager::PostUpdate()
{
	return true;
}

bool j1EntityManager::CleanUp()
{
	return true;
}

bool j1EntityManager::Load(pugi::xml_node& data)
{
	return true;
}

bool j1EntityManager::Save(pugi::xml_node& data) const
{
	return true;
}

j1Entities* j1EntityManager::CreateEntity(j1Entities::Types type, iPoint pos)
{
	j1Entities* ret = nullptr;

	switch (type)
	{
	case j1Entities::Types::player:
		ret = new j1Player(pos);
		break;
	case j1Entities::Types::minotaur:
		ret = new j1Bat(pos);
		break;
	case j1Entities::Types::bat:
		ret = new j1Bat(pos);
		break;
	}

	if (ret != nullptr)
	{
		entities.add(ret);
		entities.end->data->Awake(node);
		entities.end->data->Start();
	}
	return ret;
}