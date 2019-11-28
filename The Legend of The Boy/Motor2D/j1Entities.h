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
	bool Save(pugi::xml_node& data) const;

	virtual void Pushbacks();
	virtual void OnCollision(Collider* c1, Collider* c2);
	virtual void Draw();

	virtual void SetState();
	virtual void GetPosition();

	iPoint			position;
	int				SpawnPointX, SpawnPointY, yLimit;
	SDL_Texture* Character_tex = nullptr;
	p2SString		orientation = "right";
	bool			input = true;
	bool			dead = false;
	bool			dash = false;
	bool			OnGround = true;
	p2SString		texture_path;

	p2SString		entity_name;
	
};

#endif