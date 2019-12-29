#ifndef __UI_ELEMENT__
#define __UI_ELEMENT__

#include "j1Module.h"
#include "p2Point.h"
#include "p2Log.h"
#include "SDL/include/SDL_render.h"

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

	bool Is_above();
	virtual void Click();

	void Draw();

	void SetPos(int x, int y);

public:

	SDL_Texture* texture = nullptr;

	UIelement* parent = nullptr;
	j1Module* listener = nullptr;

	TYPE_UI type;

	iPoint Position;

	bool canMoveIn_X_axis = false;

	const char* text = nullptr;
	bool enabled;
	int size;
	iPoint posOffset;
	int v;
	uint click;
	uint above;
	bool played;

	SDL_Rect section;

};

#endif
