#ifndef __J1ENTITIES_H__
#define __J1ENTITIES_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "Animation.h"
#include "j1Module.h"

struct SDL_Texture;
struct Collider;

class j1Entities : public j1Module
{
public:

	enum class Types
	{
		player,
		skeleton,
		bat,
		unknown
	};

	Types entity_type;

	j1Entities(Types entity_type);

	virtual ~j1Entities();

	bool Load(pugi::xml_node& data);

	virtual void Draw(float dt);

	bool Save(pugi::xml_node& data) const;

	iPoint			position;

	p2SString		entity_name;
};

#endif