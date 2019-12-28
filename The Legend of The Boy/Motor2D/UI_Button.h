#ifndef __J1GUIBUTTON__
#define __J1GUIBUTTON__

#include "j1Module.h"
#include "UI_Element.h"


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

	void Drag(float dt);

	bool CleanUp();

	bool Load(pugi::xml_node&) { return true; };
	bool Save(pugi::xml_node&) const  const { return true; };

private:

	UIelement* label = nullptr;
	int MouseMovement;

	bool drag;
};






#endif // !__J1GUIBUTTON__
