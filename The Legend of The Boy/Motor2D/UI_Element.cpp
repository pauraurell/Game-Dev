#include "UI_Element.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"

UIElement::UIElement() 
{

}

UIElement::~UIElement() 
{

}

void UIElement::Update()
{

}

void UIElement::Draw(SDL_Texture* texture) {

	switch (this->UIelementType)
	{
	case BUTTON:
		break;
	case LABEL:
		break;
	case IMAGE:
		break;
	case SLIDER:
		break;
	}
}
