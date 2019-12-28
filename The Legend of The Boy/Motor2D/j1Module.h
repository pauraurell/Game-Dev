// ----------------------------------------------------
// j1Module.h
// Interface for all engine modules
// ----------------------------------------------------

#ifndef __j1MODULE_H__
#define __j1MODULE_H__

#include "p2Defs.h"
#include "p2Point.h"
#include "p2SString.h"
#include "PugiXml\src\pugixml.hpp"

class j1App;
struct Collider;

enum class uiEvent
{
	EVENT_UNKOWNN = -1,
	EVENT_ONCLICK,
	EVENT_HOVER
};

class UIelement;

class j1Module
{
public:

	j1Module() : active(false)
	{}

	void Init(bool init)
	{
		to_enable = init;
		active = init;
	}

	// Called before render is available
	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool Load(pugi::xml_node&)
	{
		return true;
	}

	virtual bool Save(pugi::xml_node&) const
	{
		return true;
	}

	bool IsEnabled() const { return active; }

	void Enable()
	{
		if (active == false)
		{
			active = true;
			Start();
		}
	}

	void Disable()
	{
		if (active == true)
		{
			active = false;
			CleanUp();
		}
	}

	virtual void OnCollision(Collider*, Collider*) {}
	virtual void UIevents(uiEvent type, UIelement* element) {}

public:

	p2SString	name;
	bool		active;
	bool		to_enable;

};

#endif