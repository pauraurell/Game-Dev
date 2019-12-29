#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "Animation.h"
#include "j1Module.h"
#include "j1Entities.h"

struct SDL_Texture;
struct Collider;

class j1Coin : public j1Entities
{
public:

	j1Coin();
	j1Coin(iPoint pos, bool isDead);

	// Destructor
	virtual ~j1Coin();

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


	// Called before quitting
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);
	void ConfigLoading();

	int				SpawnPointX, SpawnPointY, yLimit;
	SDL_Texture*	coinTex = nullptr;
	p2SString		orientation;
	bool			OnGround;

	//ANIMATIONS
	Animation*		current_animation = nullptr;
	Animation		coin;

	void Pushbacks();

private:

	p2SString		folder;
	p2SString		texture_path;
	pugi::xml_node	node;
};

