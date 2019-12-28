#ifndef __IMAGE__
#define __IMAGE__

#include "j1Module.h"
#include"UI_Element.h"
#include "j1UI.h"

struct SDL_Texture;

class Image : public UIelement
{
public:

	Image();
	~Image();

	bool Awake(pugi::xml_node&);

	bool PreUpdate();
	bool PostUpdate();

	bool CleanUp();

	bool Load(pugi::xml_node&) { return true; };
	bool Save(pugi::xml_node&) const  const { return true; };


};

#endif

