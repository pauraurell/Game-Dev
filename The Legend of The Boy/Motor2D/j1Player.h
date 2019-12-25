#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "Animation.h"
#include "j1Module.h"
#include "j1Entities.h"

struct SDL_Texture;
struct Collider;
class j1Timer;

class j1Player : public j1Entities
{
public:

	/*enum playerStates
	{
		PLAYER_IDLE,
		PLAYER_RUN_LEFT,
		PLAYER_RUN_RIGHT,
		PLAYER_JUMP,
		PLAYER_JUMP_RIGHT,
		PLAYER_JUMP_LEFT,
		PLAYER_DASH,
		PLAYER_ATTACK
	};*/

	j1Player();
	j1Player(iPoint pos);

	// Destructor
	virtual ~j1Player();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();


	// Called each loop iteration
	void Draw(float dt);

	//bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void GetPlayerState();
	void SetPlayerState(playerStates state);
	void GetPlayerPosition(float dt);

	// Called before quitting
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);
	void ConfigLoading();

	void Respawn();

	int				SpawnPointX, SpawnPointY, yLimit;
	SDL_Texture*	Character_tex = nullptr;
	p2SString		orientation;
	bool			dash;
	bool			OnGround;
	bool			hit;
	bool			cantGetHit;
	j1Timer*		hitTimer = nullptr;

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
	float			dashSpeed;
	fPoint		    vel;

	//TIMERS
	Uint32		dash_timer;
	int		    dashTime;
	bool		dashTimer;
	Uint32		attack_timer;
	int		    attackTime;
	bool		attackTimer;
	Uint32		respawn_timer;
	bool		respawnTimer;
		
	//COLLIDERS
	Collider*	attackCollider = nullptr;
	Collider*   dashAttackCollider = nullptr;
	
	int			ColOffsset;
	bool		ColOffssetON;

	//GOD MODE
	bool godMode;
	bool godModeRight;
	bool godModeLeft;
	bool godModeUp;
	bool godModeDown;

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


