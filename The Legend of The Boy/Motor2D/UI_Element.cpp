#include "j1UI.h"
#include "UI_Element.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Audio.h"

UIelement::UIelement()
{
	Position = { 0,0 };

	section = { 0,0,0,0 };
	bool enabled = false;
	posOffset = { 0,0 };
	played = false;
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
	if ((Is_above() == true) && (this->type == TYPE_UI::UI_BUTTON))
	{
		SDL_SetTextureColorMod(texture, 255, 190, 170);
		SDL_SetTextureAlphaMod(texture, 220);
	}
	else
	{
		SDL_SetTextureColorMod(texture, 255, 255, 255);
		SDL_SetTextureAlphaMod(texture, 255);
	}

	App->render->Blit(texture, Position.x + App->render->camera.x/-SCALE , Position.y + App->render->camera.y / -SCALE, &section);
	if (App->ui->debug) { App->render->DrawQuad({ Position.x*SCALE, Position.y*SCALE, section.w*SCALE, section.h*SCALE }, 255, 0, 255, 255, false, false); }
	
}


bool UIelement::Is_above()
{
	bool ret = false;
	
	SDL_Point mouse;
	SDL_Point mousePos;
	App->input->GetMousePosition(mouse.x, mouse.y);
	mouse.x = mouse.x / SCALE;
	mouse.y = mouse.y / SCALE;

	SDL_Rect intersect = { Position.x , Position.y, section.w, section.h };
	//LOG(true, "%i, %i, %i, %i", intersect.x, intersect.y, intersect.w, intersect.h);

	if (SDL_PointInRect(&mouse, &intersect)) {
		//LOG(true, "ABOVE");
		if (listener != nullptr)
		{
			if (played == false && this->enabled == true) { App->audio->PlayFx(above, App->audio->volume_fx, 0, 3); played = true; }
			this->listener->UIevents(uiEvent::EVENT_HOVER, this);
		}
		ret = true;
	}
	else { played = false; }

	//else { LOG(true, "NO ABOVE"); }
	//LOG(true, "%i, %i", mouse.x, mouse.y);
	
	return ret;
}

void UIelement::SetPos(int x, int y)
{
	this->Position.x = x * SCALE;
	this->Position.y = y * SCALE;
}


void UIelement::Click()
{
	if (listener != nullptr)
	{
		App->audio->PlayFx(click, App->audio->volume_fx, 0, 3);
		this->listener->UIevents(uiEvent::EVENT_ONCLICK, this);
	}
}
