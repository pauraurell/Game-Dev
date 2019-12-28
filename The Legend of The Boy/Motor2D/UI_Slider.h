#ifndef __SLIDER__
#define __SLIDER__

#include "j1Module.h"
#include "j1UI.h"
#include "UI_Element.h"

class Slider : public UIelement
{
public:

	Slider();
	~Slider();

	bool Awake(pugi::xml_node&);
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

	bool Load(pugi::xml_node&) { return true; };
	bool Save(pugi::xml_node&) const  const { return true; };

};

#endif