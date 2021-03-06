#ifndef __UI_ELEMENT__
#define __UI_ELEMENT__

#include "j1Module.h"
#include "p2Point.h"
#include "p2Log.h"
#include "SDL/include/SDL_render.h"

struct SDL_Color;

enum class TYPE_UI
{
	UI_BUTTON,
	UI_LABEL,
	UI_IMAGE,
	UI_SLIDER,
	UI_UNKNOWN
};

enum class SLIDER_TYPE
{
	Music,
	Fx,
	NOT_A_SLIDER
};

class UIelement
{
public:

	UIelement();
	~UIelement();

	virtual bool Awake(pugi::xml_node& config) { return true; };
	virtual bool Start();
	virtual bool PreUpdate();
	virtual bool Update(float dt);
	virtual bool PostUpdate();

	virtual bool CleanUp();

	bool OnTop();
	virtual void Click();
	void Blit();

	void Draw();

	void SetPos(int x, int y);

public:

	SDL_Texture* texture = nullptr;
	UIelement* parent = nullptr;
	j1Module* mod = nullptr;
	TYPE_UI type;
	iPoint Position;
	bool canMoveIn_X_axis = false;
	const char* text = nullptr;
	bool enabled;
	int size;
	iPoint posOffset;
	SDL_Color color;
	uint click;
	uint above;
	bool played;
	SDL_Rect section;

};

#endif
