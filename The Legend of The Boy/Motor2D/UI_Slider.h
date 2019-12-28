#ifndef __SLIDER__
#define __SLIDER__

#include "j1Module.h"
#include "j1UI.h"
#include "UI_Element.h"


class Slider : public UIelement
{
public:

	Slider(SLIDER_TYPE typeOfScroll);
	~Slider();

	bool Awake(pugi::xml_node&);
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

	bool Load(pugi::xml_node&) { return true; };
	bool Save(pugi::xml_node&) const  const { return true; };

	void ScrollLimits();

public:

	UIelement* button = nullptr;
	float button_position;
	float pos_0;
	SLIDER_TYPE type_of_slider = SLIDER_TYPE::NOT_A_SLIDER;

};

#endif