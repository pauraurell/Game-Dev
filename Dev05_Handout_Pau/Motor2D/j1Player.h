#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "Animation.h"
#include "j1Module.h"

struct SDL_Texture;
struct Collider;

enum playerStates
{
	PLAYER_IDLE,
	PLAYER_RUN_LEFT,
	PLAYER_RUN_RIGHT,
	PLAYER_JUMP,
};

class j1Player : public j1Module
{
public:

	j1Player();

	// Destructor
	virtual ~j1Player();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();


	// Called each loop iteration
	void Draw();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void GetPlayerState();
	void GetPlayerPosition();

	// Called before quitting
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);

	iPoint			position;
	Animation*		current_animation = nullptr;
	Animation		idle;
	Animation		running;
	Animation		jump;
	SDL_Texture*	Character_tex;
	int				maxSpeed;
	int				gravity;
	fPoint		    vel;
	p2SString		orientation = "right";

	Collider* colPlayer;

	void Pushbacks();

	playerStates state;


private:

	p2SString		folder;
	p2SString		texture_path;
	pugi::xml_node	node;
};

#endif // __j1MAP_H__


