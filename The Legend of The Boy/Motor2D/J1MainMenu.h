#ifndef __j1MAINMENU_H__
#define __j1MAINMENU_H__

#include "j1Module.h"
#include "p2DynArray.h"

struct SDL_Texture;
class UIelement;

class j1MainMenu : public j1Module
{
public:

	j1MainMenu();

	virtual ~j1MainMenu();

	bool Awake(pugi::xml_node& config);

	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

	bool Draw();

	SDL_Texture* texture1 = nullptr;
	SDL_Texture* texture2 = nullptr;
	SDL_Texture* texture3 = nullptr;
	SDL_Texture* title = nullptr;

	UIelement* testButton = nullptr;
	UIelement* ContinueButton = nullptr;
	UIelement* exitButton = nullptr;
	UIelement* creditsButton = nullptr;
	UIelement* settingsButton = nullptr;
	UIelement* SettingsImage = nullptr;
	UIelement* sliderFx = nullptr;
	UIelement* sliderMusic = nullptr;
	UIelement* settingsLabel = nullptr;
	UIelement* settingsButtonOk = nullptr;
	UIelement* sliderFxLabel = nullptr;
	UIelement* sliderMusicLabel = nullptr;

	bool SettingsMenuIsActive;

	void UIevents(uiEvent type, UIelement* element);

	int posX1, posX2, posX3; bool stop;

};

#endif // __j1INTROSCENE_H__