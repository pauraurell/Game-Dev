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
	int				pLife;

	SDL_Rect		heart;
	SDL_Rect		emptHeart;
	j1Timer			time;

	UIelement* AddGUIelement(TYPE_UI type, UIelement* parent, iPoint globalPosition, iPoint localPosition, bool enabled, SDL_Rect section, char* text = nullptr, j1Module* listener = nullptr);
	SDL_Texture* j1UI::GetAtlasTexture() const;

	p2List<UIelement*>	UIelements;

private:

	SDL_Texture*	ui_tex = nullptr;
	p2SString atlasFile;
};

#endif

