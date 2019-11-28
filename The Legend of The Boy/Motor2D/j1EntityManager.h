#ifndef __J1ENTITYMANAGER_H__
#define __J1ENTITYMANAGER_H__

#include "j1Module.h"
#include "j1Entities.h"
#include "PugiXml/src/pugixml.hpp"

struct SDL_Texture;

class j1EntityManager : public j1Module
{
public:
	j1EntityManager();
	~j1EntityManager();
	bool Awake(pugi::xml_node& config);
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&)const;
};


#endif
