#ifndef __BUTTON__
#define __BUTTON__

#include "j1Module.h"
#include"UI_Element.h"

class Button : public UIelement
{
public:

	Button();
	~Button();

	bool Awake(pugi::xml_node&);
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

};

#endif 