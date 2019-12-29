#ifndef __j1SCORESCENE_H__
#define __j1SCORESCENE_H__

#include "j1Module.h"
#include "p2DynArray.h"
#include "p2List.h"
#include "SDL/include/SDL.h"

#define CREDLIMIT -1850

struct SDL_Texture;
class UIelement;

class j1Score : public j1Module
{
public:

	j1Score();

	virtual ~j1Score();

	bool Awake(pugi::xml_node& config);

	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	bool Draw();
	void DisableAll();

	bool		draw;
	SDL_Rect	background;

	//COUNTERS
	int		coins;
	int		enemies;
	int		time;

	//SCORE
	int		timeScore;
	int		coinsScore;
	int		enemiesScore;
	int		lifesScore;
	int		totalScore;
	
	//LABELS
	UIelement*		score_label;
	UIelement*		timeScore_label;
	UIelement*		coinsScore_label;
	UIelement*		enemiesScore_label;
	UIelement*		lifesScore_label;
	UIelement*		totalScore_label;
	UIelement*		escToEnd_label;

};

#endif // __j1SCORESCENE_H__
