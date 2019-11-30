#ifndef __j1BAT_H__
#define __j1BAT_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "Animation.h"
#include "j1Module.h"
#include "j1Entities.h"

struct SDL_Texture;
struct Collider;

enum batStates
{
	BAT_FLIYING_LEFT,
	BAT_FLIYING_RIGHT,
	BAT_DEAD,	
	FLYING_TO_THE_PLAYER
};

class j1Bat : public j1Entities
{
public:

	j1Bat();
	j1Bat(iPoint pos);

	// Destructor
	virtual ~j1Bat();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool BatPathfinding(float dt);

	// Called each loop iteration
	void Draw(float dt);

	//bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void BatState(batStates state);
	void SetBatPosition(float dt);

	// Called before quitting
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);


	iPoint			position;
	int				SpawnPointX, SpawnPointY, yLimit;
	SDL_Texture*	batTex = nullptr;
	p2SString		orientation = "right";
	bool			dead = false;
	bool			OnGround = true;

	//ANIMATIONS
	Animation*		current_animation = nullptr;
	Animation		flying;

	//SPEED
	float			SpeedX;
	float			SpeedY;
	float			gravity;
	fPoint		    vel;

	//COLLIDERS
	Collider*	batCollider = nullptr;

	//AUDIO
	uint			BatFx;

	void Pushbacks();

	batStates state = BAT_FLIYING_RIGHT;


private:

	p2SString		folder;
	p2SString		texture_path;
	pugi::xml_node	node;
};
#endif 
