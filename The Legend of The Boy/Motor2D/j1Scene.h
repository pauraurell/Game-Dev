#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//Debug
	void RestartCurrentLevel(); //F3
	void StartFirstLevel(); //F1
	void StartSecondLevel(); //F2

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	bool scene_change = false;
	bool scene_changed = false;
	bool manualFirstLevel = false;
	bool secret_map = false;

	Uint32 scene_change_timer;
	bool sceneChangeTimer = false;

	p2SString CurrentMap;


private:

	p2List<p2SString> MapList;
	//p2SString CurrentMap;
};

#endif // __j1SCENE_H__