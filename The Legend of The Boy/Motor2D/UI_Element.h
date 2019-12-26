#include "p2Point.h"
#include "j1UI.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;
struct SDL_Rect;

enum UI_TYPE	
{
	UNDEFINED = -1,
	BUTTON,
	LABEL,
	IMAGE,
	SLIDER
};

class UIElement
{
	UIElement();
	virtual ~UIElement();

	virtual void Update();
	virtual void Draw(SDL_Texture* texture);

	iPoint position;

	bool is_interactive;
	UI_TYPE UIelementType;

};
