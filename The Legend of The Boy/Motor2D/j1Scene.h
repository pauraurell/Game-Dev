#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;
class j1Entities;
class UIelement;

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
	void CreateEnt();
	void Create1MapEnemies();
	void Create2MapEnemies();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const; 
	bool scene_change;
	bool scene_changed;
	bool manualFirstLevel;
	bool secret_map;
	bool input;
	bool cameraTracking;

	int scene_change_timer;
	bool sceneChangeTimer;

	p2SString CurrentMap;
	j1Entities* player;
	j1Entities* bat1;
	j1Entities* skeleton1;
	j1Entities* bat2;
	j1Entities* skeleton2;

	iPoint playerSpawnPoint;

	int PlayerSpawnPointX;
	int PlayerSpawnPointX2;
	int PlayerSpawnPointY;

	int BatSpawnPointX;
	int BatSpawnPointY;
	int BatSpawnPointX2;
	int BatSpawnPointY2;
	int Bat2SpawnPointX;
	int Bat2SpawnPointY;
	int Bat2SpawnPointX2;
	int Bat2SpawnPointY2;

	int SkeletonSpawnPointX;
	int SkeletonSpawnPointY;
	int SkeletonSpawnPointX2;
	int SkeletonSpawnPointY2;
	int Skeleton2SpawnPointX;
	int Skeleton2SpawnPointY;
	int Skeleton2SpawnPointX2;
	int Skeleton2SpawnPointY2;


private:

	p2List<p2SString> MapList;
	//p2SString CurrentMap;
};

#endif // __j1SCENE_H__