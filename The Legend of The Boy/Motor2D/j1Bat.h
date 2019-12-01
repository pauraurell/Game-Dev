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
	BAT_FLYING_LEFT,
	BAT_FLYING_RIGHT,
	BAT_FLYING_LEFT_IDLE,
	BAT_FLYING_RIGHT_IDLE,
	BAT_FLYING_UP,
	BAT_FLYING_DOWN,
	BAT_DEAD,	

};

class j1Bat : public j1Entities
{
public:

	j1Bat();
	j1Bat(iPoint pos, bool isDead);

	// Destructor
	virtual ~j1Bat();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	void Pathfinding(float dt);
	void BatFlyingToThePlayer(float dt);

	// Called each loop iteration
	void Draw(float dt);

	//bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void BatState(batStates state);
	void SetBatPosition(float dt);

	void EntityDeath();

	// Called before quitting
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);
	void ConfigLoading();

	int				SpawnPointX, SpawnPointY, yLimit;
	SDL_Texture*	batTex = nullptr;
	p2SString		orientation = "right";
	bool			OnGround = true;

	//ANIMATIONS
	Animation*		current_animation = nullptr;
	Animation		flying;
	Animation		die;

	//SPEED
	float			SpeedX;
	float			SpeedY;
	fPoint		    vel;

	//AUDIO
	uint			BatFx;

	void Pushbacks();

	batStates state = BAT_FLYING_RIGHT_IDLE;

	int x;
	int y;
	iPoint pPos;
	iPoint origin;
	iPoint pos;
	const p2DynArray<iPoint>* lastpath;
	const p2DynArray<iPoint>* path;
	//SDL_Texture* debugTex = nullptr;

private:

	p2SString		folder;
	p2SString		texture_path;
	pugi::xml_node	node;
};
#endif 
