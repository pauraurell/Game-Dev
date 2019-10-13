#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "Animation.h"

struct SDL_Texture;

struct SDL_Texture;


class j1Player : public j1Module
{
public:
	j1Player();
	virtual ~j1Player();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	//Called before the first frame
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Called each loop iteration
	void Draw();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

public:

	SDL_Texture * character_tex = nullptr;
	Animation* current_animation = &idle;
	Animation idle;
	iPoint position;
	SDL_Rect r;

private:
	p2SString		folder;
	p2SString		texture_path;
};

#endif // __j1Player_H__
