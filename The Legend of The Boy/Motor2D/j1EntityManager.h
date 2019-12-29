#ifndef __J1ENTITYMANAGER_H__
#define __J1ENTITYMANAGER_H__

#include "j1Module.h"
#include "j1Entities.h"
#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"

struct SDL_Texture;

class j1EntityManager : public j1Module
{
public:
	j1EntityManager();
	~j1EntityManager();
	bool Awake(pugi::xml_node& config);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	j1Entities* CreateEntity(j1Entities::Types type, iPoint pos, bool isDead, bool secret = false);
	void DestroyEntity(j1Entities* entity);
	void DestroyCoins();
	void DestroyEntities();
	void DestroyEnemies();
	void RestartEntities();
	void DrawEntities(float dt);
	j1Entities* GetPlayerEntity();

	pugi::xml_node node;
	p2List<j1Entities*> entities;
};


#endif
