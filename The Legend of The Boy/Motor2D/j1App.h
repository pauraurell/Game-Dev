#ifndef __j1APP_H__
#define __j1APP_H__

#include "j1Timer.h"
#include "j1PerfTimer.h"
#include "p2List.h"
#include "j1Module.h"
#include "PugiXml\src\pugixml.hpp"

// Modules
class j1Window;
class j1Input;
class j1Render;
class j1Textures;
class j1Audio;
class j1Scene;
class j1Player;
class j1Bat;
class j1Skeleton;
class j1Map;
class j1FadeToBlack;
class j1Collision;
class j1Particles;
class j1EntityManager;


class j1App
{
public:

	// Constructor
	j1App(int argc, char* args[]);

	// Destructor
	virtual ~j1App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(j1Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	void LoadGame();
	void SaveGame() const;
	void GetSaveGames(p2List<p2SString>& list_to_fill) const;

private:

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadGameNow();
	bool SavegameNow() const;

public:

	// Modules
	j1Window*			win;
	j1Input*			input;
	j1Render*			render;
	j1Textures*			tex;
	j1Audio*			audio;
	j1Scene*			scene;
	j1Player*			player;
	j1Bat*				bat;
	j1Skeleton*			skeleton;
	j1Map*				map;
	j1FadeToBlack*		fade;
	j1Collision*		col;
	j1Particles*		particles;
	j1EntityManager*	entManager;

	#define SCALE 2

	bool FrameCapEnabled = false;
	uint framerate = 0u;

private:

	p2List<j1Module*>	modules;
	uint				frames;
	int					argc;
	char**				args;

	p2SString			title;
	p2SString			organization;

	mutable bool		want_to_save;
	bool				want_to_load;
	p2SString			load_game;
	mutable p2SString	save_game;

	bool pause = false;

	j1Timer* mainTimer = nullptr;
	j1PerfTimer* PerfTimer = nullptr;
	j1Timer* lastSecFrames = nullptr;
	j1Timer lastFrameTimer;
	float avg_fps = 0.0f;

	uint64 frame_count = 0u;
	uint last_second_frame_count = 0u;
	uint32 last_frame_ms = 0u;
	uint32 frames_on_last_update = 0u;
	
	float	dt = 0.0f;
	p2SString capStr;

};

extern j1App* App; // No student is asking me about that ... odd :-S

#endif