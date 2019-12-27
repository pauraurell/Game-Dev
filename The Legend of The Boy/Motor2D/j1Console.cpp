#include "j1Console.h"
#include "j1App.h"
#include "p2Log.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1Fonts.h"
#include "j1Textures.h"

j1Console::j1Console()
{
	active = false;
}

j1Console::~j1Console()
{

}

bool j1Console::Start()
{
	cons.x = App->render->camera.x * OFFSET;
	cons.y = App->render->camera.y * 1 / 1000;
	cons.w = App->render->camera.w;
	cons.h = App->render->camera.h / 2;

	return true;
}

bool j1Console::PreUpdate()
{
	bool ret;
	
	cons.x = App->render->camera.x * 1 / 1000;
	cons.y = App->render->camera.y * 1 / 1000;

	if (input == true) { ret = ConsoleInput(); }
	else { ret = true; }

	return ret;
}

bool j1Console::Update(float dt)
{
	if (active == true) { input = true; }
	else if (active == false) { input = false; }

	return true;
}

bool j1Console::PostUpdate()
{
	if (active == true)
	{
		SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, 200);
		SDL_RenderFillRect(App->render->renderer, &cons);

		int offsetY = 100;
		int maxOffsetY = 100;
		p2List_item<SDL_Texture*>* item = text.end;
		while (item)
		{
			if (App->render->camera.y * OFFSET + offsetY > App->render->camera.y * OFFSET - 2 && App->render->camera.y * OFFSET + offsetY <= App->render->camera.y * OFFSET + maxOffsetY)
			{
				App->render->Blit(item->data, App->render->camera.x * OFFSET + 2, App->render->camera.y * OFFSET + offsetY);
			}
			offsetY -= 20;
			item = item->prev;
		}
		
	}

	return true;
}

bool j1Console::CleanUp()
{
	p2List_item<SDL_Texture*>* item = text.start;
	while (item)
	{
		SDL_DestroyTexture(item->data);
		text.del(item);
		item = item->next;
	}
	text.clear();

	return true;
}

void j1Console::Log(const char* string)
{
	text.add(App->fonts->PrintTextBox(string, { 255, 255, 255, 255 }, App->fonts->default, cons.w, 16));
}

bool j1Console::ConsoleInput()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYUP && event.key.repeat == 0)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:

				break;
			case SDLK_SPACE:

				break;
			case SDLK_a:

				break;
			case SDLK_d:

				break;
			}
		}

		if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_q:
				LOG(true, "q");
				break;
			case SDLK_w:
				LOG(true, "w");
				break;
			case SDLK_e:
				LOG(true, "e");
				break;
			case SDLK_r:
				LOG(true, "r");
				break;
			case SDLK_t:
				LOG(true, "t");
				break;
			case SDLK_y:
				LOG(true, "y");
				break;
			case SDLK_u:
				LOG(true, "u");
				break;
			case SDLK_i:
				LOG(true, "i");
				break;
			case SDLK_o:
				LOG(true, "o");
				break;
			case SDLK_p:
				LOG(true, "p");
				break;
			case SDLK_a:
				LOG(true, "a");
				break;
			case SDLK_s:
				LOG(true, "s");
				break;
			case SDLK_d:
				LOG(true, "d");
				break;
			case SDLK_f:
				LOG(true, "f");
				break;
			case SDLK_g:
				LOG(true, "g");
				break;
			case SDLK_h:
				LOG(true, "h");
				break;
			case SDLK_j:
				LOG(true, "j");
				break;
			case SDLK_k:
				LOG(true, "k");
				break;
			case SDLK_l:
				LOG(true, "l");
				break;
			case SDLK_z:
				LOG(true, "z");
				break;
			case SDLK_x:
				LOG(true, "x");
				break;
			case SDLK_c:
				LOG(true, "c");
				break;
			case SDLK_v:
				LOG(true, "v");
				break;
			case SDLK_b:
				LOG(true, "b");
				break;
			case SDLK_n:
				LOG(true, "n");
				break;
			case SDLK_m:
				LOG(true, "m");
				break;
			case SDLK_1:
				LOG(true, "1");
				break;
			case SDLK_2:
				LOG(true, "2");
				break;
			case SDLK_3:
				LOG(true, "3");
				break;
			case SDLK_4:
				LOG(true, "4");
				break;
			case SDLK_5:
				LOG(true, "5");
				break;
			case SDLK_6:
				LOG(true, "6");
				break;
			case SDLK_7:
				LOG(true, "7");
				break;
			case SDLK_8:
				LOG(true, "8");
				break;
			case SDLK_9:
				LOG(true, "9");
				break;
			case SDLK_0:
				LOG(true, "0");
				break;
			case SDLK_UNDERSCORE:
				LOG(true, "_");
				break;
			case SDLK_PERIOD:
				LOG(true, ".");
				break;
			case SDLK_BACKSPACE:
				LOG(true, "Backspace");
				break;
			case SDLK_KP_ENTER:
				LOG(true, "Enter");
				break;
			}
		}
	}
	return true;
}