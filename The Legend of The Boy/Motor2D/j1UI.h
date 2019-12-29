#ifndef __J1UI__
#define __J1UI__

#include "j1Module.h"
#include "SDL/include/SDL.h"
#include "UI_Element.h"
#include "p2List.h"
#include "j1Timer.h"

struct SDL_Texture;

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

	UIelement* Add_UIelement(TYPE_UI type, SLIDER_TYPE typeOfScroll, UIelement* parent, iPoint Position, int size, bool enabled, SDL_Rect section, iPoint PositionOffset, const char* text = nullptr, j1Module* listener = nullptr, bool CanDrag = false);

	void CreateInGameUi();
	void CreateInGameMenuUi();

	void UIevents(uiEvent type, UIelement* element);

	void EnableAll();
	void DisableAll();

public:

	int				pLife;
	SDL_Rect		heart;
	SDL_Rect		emptHeart;
	p2List<UIelement*>	UIelements;

	//TIMER
	j1Timer		time;
	int			sec;
	int			min;
	p2SString* timer;

	//UI Elements
	UIelement* coin_image;
	UIelement* coin_label;
	UIelement* timer_image;
	UIelement* timer_label;
	UIelement* inGameMenu_label_settings;
	UIelement* inGameMenu_image;
	UIelement* inGameMenu_button_QuitToDesktop;
	UIelement* inGameMenu_button_Save;
	UIelement* inGameMenu_button_Load;
	UIelement* inGameMenu_button_MainMenu;
	UIelement* label_fx;
	UIelement* label_music;
	UIelement* SliderFx;
	UIelement* SliderMusic;
	UIelement* inGameMenu_button_Continue;
	UIelement* SettingsButton;

	uint clickFx;

	bool debug;
	bool InGameMenu;
	SDL_Texture*	ui_tex = nullptr;

};

#endif

