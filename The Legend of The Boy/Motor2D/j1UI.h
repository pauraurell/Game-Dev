#ifndef __J1UI__
#define __J1UI__

#include "j1Module.h"
#include "SDL/include/SDL.h"
#include "UI_Element.h"
#include "p2List.h"
#include "j1Timer.h"

struct SDL_Texture;
struct SDL_Color;

class j1UI : public j1Module
{
public:

	j1UI();
	~j1UI();

	bool Awake(pugi::xml_node& config);
	bool Start() override;
	bool PreUpdate() override;
	bool Update(float dt) override;
	bool PostUpdate() override;
	bool CleanUp() override;

	void Draw();
	UIelement* Add_UIelement(TYPE_UI type, SLIDER_TYPE typeOfScroll, UIelement* parent, iPoint Position, int size, bool enabled, SDL_Rect section, iPoint PositionOffset, const char* text = nullptr, j1Module* mod = nullptr, bool CanDrag = false, SDL_Color color = { 0,0,0,255 });

public:

	int				pts;
	//All UI Elements List
	p2List<UIelement*>	UIelements;

	bool debug;
	SDL_Texture*	ui_tex = nullptr;

};

#endif

