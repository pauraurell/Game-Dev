#ifndef __J1ENTITIES_H__
#define __J1ENTITIES_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "Animation.h"
#include "j1Module.h"

class j1Entities : public j1Module
{
public:

	enum class Types
	{
		player,
		minotaur,
		el_otro,
	};

	Types entity_type;

	j1Entities(Types entity_type);

	virtual ~j1Entities();

	virtual bool CleanUp();
	bool Load(pugi::xml_node& data);
	bool Save(pugi::xml_node& data);

	virtual void Pushbacks();
	virtual void OnCollision();
	virtual void Draw();

	void GetState();
	virtual void SetState();
	void GetPosition();
	
};

#endif