#include "j1UI.h"
#include "UI_Element.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Window.h"


UIelement::UIelement()
{
	bool enabled = false;
	bool interactable = false;

	bool X_drag = false;
	bool Y_drag = false;

	bool above = false;
	bool focus = false;

	rect = { 0,0,0,0 };
	globalPosition = { 0,0 };
	localPosition = { 0,0 };
}

UIelement::~UIelement()
{

}

bool UIelement::Start()
{


	return true;
}

bool UIelement::PreUpdate()
{
	return true;
}

bool UIelement::Update(float dt)
{
	return true;
}

bool UIelement::PostUpdate()
{
	return true;
}

bool UIelement::CleanUp()
{
	return true;
}


void UIelement::Draw()
{
	if ((above == true) && (this->type == TYPE_UI::UI_BUTTON))
	{
		SDL_SetTextureColorMod(texture, 255, 190, 170);
		SDL_SetTextureAlphaMod(texture, 220);
	}
	else
	{
		SDL_SetTextureColorMod(texture, 255, 255, 255);
		SDL_SetTextureAlphaMod(texture, 255);
	}

	App->render->Blit(texture, globalPosition.x, globalPosition.y, &rect);

	App->render->DrawQuad({ globalPosition.x*SCALE, globalPosition.y*SCALE, rect.w*SCALE, rect.h*SCALE }, 255, 0, 255, 255, false, false);
}


bool UIelement::Is_above()
{
	bool ret = false;
	
	SDL_Point mouse;
	SDL_Point mousePos;
	App->input->GetMousePosition(mouse.x, mouse.y);
	mouse.x = mouse.x / SCALE;
	mouse.y = mouse.y / SCALE;

	SDL_Rect intersect = { globalPosition.x , globalPosition.y, rect.w, rect.h };
	//LOG(true, "%i, %i, %i, %i", intersect.x, intersect.y, intersect.w, intersect.h);

	if (SDL_PointInRect(&mouse, &intersect)) {
		//LOG(true, "ABOVE");
		if (listener != nullptr)
		{
			this->listener->UIevents(uiEvent::EVENT_HOVER, this);
		}
		ret = true;
	}
	//else { LOG(true, "NO ABOVE"); }
	//LOG(true, "%i, %i", mouse.x, mouse.y);
	
	return ret;
}


void UIelement::Click()
{
	if (listener != nullptr)
	{
		this->listener->UIevents(uiEvent::EVENT_ONCLICK, this);
	}
}
