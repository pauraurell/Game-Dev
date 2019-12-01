#ifndef __j1SKELETON_H__
#define __j1SKELETON_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "Animation.h"
#include "j1Module.h"
#include "j1Entities.h"

struct SDL_Texture;
struct Collider;

enum skeletonStates
{	
	SKELETON_IDLE,
	SKELETON_MOVING_LEFT,
	SKELETON_MOVING_RIGHT,
	SKELETON_DEAD,
};

class j1Skeleton : public j1Entities
{
public:

	j1Skeleton();
	j1Skeleton(iPoint pos);

	// Destructor
	virtual ~j1Skeleton();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool SkeletonPathFinding(float dt);

	// Called each loop iteration
	void Draw(float dt);

	//bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void SkeletonState(skeletonStates state);
	void SetSkeletonPosition(float dt);

	void EntityDeath();

	// Called before quitting
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);
	void ConfigLoading();


	int				SpawnPointX, SpawnPointY, yLimit;
	SDL_Texture*	skeletonTex = nullptr;
	p2SString		orientation = "left";
	bool			OnGround = true;

	//ANIMATIONS
	Animation*		current_animation = nullptr;
	Animation		idle;
	Animation		walking;
	Animation		deadAnim;

	//SPEED
	float			SpeedX;
	float			SpeedY;
	float			gravity;
	fPoint		    vel;

	//AUDIO
	uint			SkeletonFx;

	void Pushbacks();

	skeletonStates state = SKELETON_IDLE;


private:

	p2SString		folder;
	p2SString		texture_path;
	pugi::xml_node	node;
};

#endif


