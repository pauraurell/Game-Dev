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
		coin,
		unknown
	};

	enum playerStates
	{
		PLAYER_IDLE,
		PLAYER_RUN_LEFT,
		PLAYER_RUN_RIGHT,
		PLAYER_JUMP,
		PLAYER_JUMP_RIGHT,
		PLAYER_JUMP_LEFT,
		PLAYER_DASH,
		PLAYER_ATTACK
	};

	Types entity_type;



	j1Entities(Types entity_type);

	void OnCollision(Collider* c1, Collider* c2);

	virtual ~j1Entities();

	//bool Load(pugi::xml_node& data);

	virtual void Pushbacks();
	virtual void Draw(float dt);
	virtual void SetPlayerState(playerStates);

	bool Save(pugi::xml_node& data) const;

	void EntityDeath();

	iPoint			position;
	bool			to_die;
	bool			dead;
	bool			OnGround;
	p2SString		orientation;
	int				SpawnPointX, SpawnPointY;
	float			gravity;
	bool			godMode;
	
	//COLLIDERS
	Collider*		colliderHead = nullptr;
	Collider*		colliderBody = nullptr;
	Collider*		colliderLegs = nullptr;

	p2SString		entity_name;
};

#endif