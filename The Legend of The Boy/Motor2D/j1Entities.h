#ifndef __J1ENTITIES_H__
#define __J1ENTITIES_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "Animation.h"
#include "j1Module.h"
#include "p2DynArray.h"

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

	void OnCollision(Collider* c1, Collider* c2);

	virtual ~j1Entities();

	//bool Load(pugi::xml_node& data);

	virtual void Pushbacks();
	virtual void Draw(float dt);

	bool Save(pugi::xml_node& data) const;

	void EntityDeath();

	iPoint			position;
	bool			dead = false;
	bool			OnGround = true;
	p2SString		orientation = "right";
	int				SpawnPointX, SpawnPointY;
	
	//COLLIDERS
	Collider*		colliderHead = nullptr;
	Collider*		colliderBody = nullptr;
	Collider*		colliderLegs = nullptr;

	p2SString		entity_name;
};

#endif