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
	PLAYER_JUMP_RIGHT,
	PLAYER_JUMP_LEFT,
	PLAYER_DASH,
	PLAYER_ATTACK
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
	void SetPlayerState(playerStates state);
	void GetPlayerPosition();

	// Called before quitting
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);

	void Respawn();

	iPoint			position;
	int		SpawnPointX, SpawnPointY, yLimit;
	SDL_Texture*	Character_tex = nullptr;
	p2SString		orientation = "right";
	bool			input = true;
	bool			dead = false;
	bool			dash = false;
	bool			OnGround = true;

	//ANIMATIONS
	Animation*		current_animation = nullptr;
	Animation		idle;
	Animation		running;
	Animation		jump;
	Animation		attack;
	Animation		ground_dash;
	Animation		god_mode_up;
	Animation		god_mode_run;
	Animation		god_mode_down;

	//SPEED
	float			maxSpeedY;
	int				maxSpeedX;
	int				maxJumpSpeed;
	float			speedDecrease;
	float			SpeedX;
	float			SpeedY;
	float			gravity;
	float			dashSpeed;
	fPoint		    vel;

	//TIMERS
	Uint32		dash_timer;
	int		    dashTime;
	bool		dashTimer = false;
	Uint32		attack_timer;
	int		    attackTime;
	bool		attackTimer = false;
	Uint32		respawn_timer;
	bool		respawnTimer = false;
		
	//COLLIDERS
	Collider*	colPlayerHead = nullptr;
	Collider*	colPlayerBody = nullptr;
	Collider*	colPlayerLegs = nullptr;
	int			ColOffsset = 5;
	bool		ColOffssetON = false;

	//GOD MODE
	bool godMode = false;
	bool godModeRight = false;
	bool godModeLeft = false;
	bool godModeUp = false;
	bool godModeDown = false;

	//AUDIO
	uint			JumpFx;
	uint			RunFx;
	float			step_time;

	void Pushbacks();

	playerStates state;


private:

	p2SString		folder;
	p2SString		texture_path;
	pugi::xml_node	node;
};

#endif // __j1MAP_H__


