#include "j1EntityManager.h"
#include "j1Entities.h"
#include "j1Player.h"
#include "j1Bat.h"
#include "j1Skeleton.h"
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

bool j1EntityManager::Awake(pugi::xml_node& config)
{
	return true;
}

bool j1EntityManager::Start()
{
	return true;
}

bool j1EntityManager::PreUpdate()
{
	p2List_item<j1Entities*>* entityList = entities.start;
	while (entityList) {
		entityList->data->PreUpdate();
		entityList = entityList->next;
	}
	return true;
}

bool j1EntityManager::Update(float dt)
{
	p2List_item<j1Entities*>* entityList = entities.start;
	while (entityList) {
		entityList->data->Update(dt);
		entityList = entityList->next;
	}
	return true;
}

bool j1EntityManager::PostUpdate()
{
	p2List_item<j1Entities*>* entityList = entities.start;
	while (entityList) {
		entityList->data->PostUpdate();
		entityList = entityList->next;
	}
	return true;
}

bool j1EntityManager::CleanUp()
{
	p2List_item<j1Entities*>* entityList = entities.start;
	while (entityList) {
		entityList->data->CleanUp();
		entityList = entityList->next;
	}
	DestroyEntities();
	return true;
}

bool j1EntityManager::Load(pugi::xml_node& data)
{
	DestroyEntities();
	pugi::xml_node entity;
	j1Entities::Types type;

	for (entity = data.child("Entity"); entity; entity = entity.next_sibling("Entity"))
	{
		p2SString entType(entity.attribute("type").as_string());
		if (entType == "player")
		{
			type = j1Entities::Types::player;
		}
		if (entType == "skeleton")
		{
			type = j1Entities::Types::skeleton;
		}
		if (entType == "bat")
		{
			type = j1Entities::Types::bat;
		}

		CreateEntity(type, { entity.child("position").attribute("posX").as_int(), entity.child("position").attribute("posY").as_int() });
	}
	return true;
}

bool j1EntityManager::Save(pugi::xml_node& data) const
{
	p2List_item<j1Entities*>* entityList = entities.start;
	while (entityList)
	{
		pugi::xml_node ent = data.append_child("Entity");
		entityList->data->Save(ent);

		entityList = entityList->next;
	}
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
	case j1Entities::Types::skeleton:
		ret = new j1Skeleton(pos);
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

void j1EntityManager::DestroyEntity(j1Entities* entity)
{
	p2List_item<j1Entities*>* entityList = entities.start;
	while (entityList)
	{
		if (entityList->data = entity)
		{
			entities.del(entityList);
		}
		entityList = entityList->next;
	}
	delete entity;
}

void j1EntityManager::DestroyEntities()
{
	p2List_item<j1Entities*>* entityList = entities.start;
	while (entityList)
	{
		entityList->data->CleanUp();
		RELEASE(entityList->data);
		entities.del(entityList);
		entityList = entityList->next;
	}
	entities.clear();
}
