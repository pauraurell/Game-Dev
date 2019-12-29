#ifndef __UI_LABEL_H__
#define __UI_LABEL_H__

#include "j1Module.h"
#include "UI_Element.h"
#include "j1UI.h"

struct SDL_Texture;

class Label : public UIelement
{
public:

	Label();
	~Label();

	bool Awake(pugi::xml_node&);
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();


	bool Load(pugi::xml_node&) { return true; };
	bool Save(pugi::xml_node&) const  const { return true; };

	void Print();
};

#endif

