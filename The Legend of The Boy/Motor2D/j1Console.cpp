#include "j1Console.h"
#include "j1App.h"
#include "p2Log.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1Fonts.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1UI.h"
#include "j1Player.h"
#include "p2SString.h"


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

	inputString.create(" ");

	list.create("list ->");
	god_mode.create("god_mode ->");
	quit.create("quit ->");
	fps.create("FPS ->");
	map1.create("map FirstLevel.tmx ->");
	map2.create("map SecondLevel.tmx ->");
	photo_mode.create("photo_mode ->");

	lastInput = 'º';
	inputChar[0] = ' ';

	inputLabel = App->ui->Add_UIelement(TYPE_UI::UI_LABEL, SLIDER_TYPE::NOT_A_SLIDER, nullptr, { 2, 170 }, 16, false, { 0,0,0,0 }, { 0,0 }, " ", this, false, {255,255,255,255});

	return true;
}

bool j1Console::PreUpdate()
{
	bool ret = true;
	
	cons.x = App->render->camera.x * 1 / 1000;
	cons.y = App->render->camera.y * 1 / 1000;

	if (input == true)
	{
		ret = ConsoleInput();

		inputString.create("%s", inputChar);
		inputLabel->text = inputString.GetString();

		if (App->input->GetKey(SDL_SCANCODE_KP_ENTER) == KEY_DOWN && inputChar[0] != ' ')
		{
			LOG(true, "%s", inputString.GetString());
			ret = Commands(inputString);
			inputString.create(" ");
			EmptyBuffer();
		}
	}

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
		inputLabel->enabled = true;

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
		App->render->Blit(inputLabel->texture, inputLabel->Position.x + App->render->camera.x / -2, inputLabel->Position.y + App->render->camera.y / -2, nullptr);
		
	}
	else { inputLabel->enabled = false; }

	return true;
}

bool j1Console::CleanUp()
{
	for (p2List_item<SDL_Texture*>* item = text.start; item; item = item->next)
	{
		SDL_DestroyTexture(item->data);
		text.del(item);
	}
	text.clear();

	for (p2List_item<UIelement*>* item = App->ui->UIelements.start; item; item = item->next)
	{
		if (item->data == inputLabel)
		{
			item->data->CleanUp();
		}
	}

	for (p2List_item<char*>* item = consInput.start; item; item = item->next)
	{
		consInput.del(item);
	}
	consInput.clear();

	return true;
}

void j1Console::Log(const char* string)
{
	text.add(App->fonts->PrintTextBox(string, { 255, 255, 255, 255 }, App->fonts->default, cons.w, 16));
}

bool j1Console::ConsoleInput()
{
	bool ret = true;
	
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	{
		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN) { lastInput = 'Q';/*consInput.add("q");*/ }
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) { lastInput = 'W';/*consInput.add("w");*/ }
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) { lastInput = 'E';/*consInput.add("e");*/ }
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) { lastInput = 'R';/*consInput.add("r");*/ }
		if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN) { lastInput = 'T';/*consInput.add("t");*/ }
		if (App->input->GetKey(SDL_SCANCODE_Y) == KEY_DOWN) { lastInput = 'Y';/*consInput.add("y");*/ }
		if (App->input->GetKey(SDL_SCANCODE_U) == KEY_DOWN) { lastInput = 'U';/*consInput.add("u");*/ }
		if (App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN) { lastInput = 'I';/*consInput.add("i");*/ }
		if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN) { lastInput = 'O';/*consInput.add("o");*/ }
		if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) { lastInput = 'P';/*consInput.add("p");*/ }
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) { lastInput = 'A';/*consInput.add("a");*/ }
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) { lastInput = 'S';/*consInput.add("s");*/ }
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) { lastInput = 'D';/*consInput.add("d");*/ }
		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) { lastInput = 'F';/*consInput.add("f");*/ }
		if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN) { lastInput = 'G';/*consInput.add("g");*/ }
		if (App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN) { lastInput = 'H';/*consInput.add("h");*/ }
		if (App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN) { lastInput = 'J';/*consInput.add("j");*/ }
		if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN) { lastInput = 'K';/*consInput.add("k");*/ }
		if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN) { lastInput = 'L';/*consInput.add("l");*/ }
		if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN) { lastInput = 'Z';/*consInput.add("z");*/ }
		if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN) { lastInput = 'X';/*consInput.add("x");*/ }
		if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN) { lastInput = 'C';/*consInput.add("c");*/ }
		if (App->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN) { lastInput = 'V';/*consInput.add("v");*/ }
		if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN) { lastInput = 'B';/*consInput.add("b");*/ }
		if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN) { lastInput = 'N';/*consInput.add("n");*/ }
		if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN) { lastInput = 'M';/*consInput.add("m");*/ }
	}
	else
	{
		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN) { lastInput = 'q';/*consInput.add("q");*/ }
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) { lastInput = 'w';/*consInput.add("w");*/ }
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) { lastInput = 'e';/*consInput.add("e");*/ }
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) { lastInput = 'r';/*consInput.add("r");*/ }
		if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN) { lastInput = 't';/*consInput.add("t");*/ }
		if (App->input->GetKey(SDL_SCANCODE_Y) == KEY_DOWN) { lastInput = 'y';/*consInput.add("y");*/ }
		if (App->input->GetKey(SDL_SCANCODE_U) == KEY_DOWN) { lastInput = 'u';/*consInput.add("u");*/ }
		if (App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN) { lastInput = 'i';/*consInput.add("i");*/ }
		if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN) { lastInput = 'o';/*consInput.add("o");*/ }
		if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) { lastInput = 'p';/*consInput.add("p");*/ }
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) { lastInput = 'a';/*consInput.add("a");*/ }
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) { lastInput = 's';/*consInput.add("s");*/ }
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) { lastInput = 'd';/*consInput.add("d");*/ }
		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) { lastInput = 'f';/*consInput.add("f");*/ }
		if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN) { lastInput = 'g';/*consInput.add("g");*/ }
		if (App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN) { lastInput = 'h';/*consInput.add("h");*/ }
		if (App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN) { lastInput = 'j';/*consInput.add("j");*/ }
		if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN) { lastInput = 'k';/*consInput.add("k");*/ }
		if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN) { lastInput = 'l';/*consInput.add("l");*/ }
		if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN) { lastInput = 'z';/*consInput.add("z");*/ }
		if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN) { lastInput = 'x';/*consInput.add("x");*/ }
		if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN) { lastInput = 'c';/*consInput.add("c");*/ }
		if (App->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN) { lastInput = 'v';/*consInput.add("v");*/ }
		if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN) { lastInput = 'b';/*consInput.add("b");*/ }
		if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN) { lastInput = 'n';/*consInput.add("n");*/ }
		if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN) { lastInput = 'm';/*consInput.add("m");*/ }
	}
	
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) { lastInput = '1';/*consInput.add("1");*/ }
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) { lastInput = '2';/*consInput.add("2");*/ }
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN) { lastInput = '3';/*consInput.add("3");*/ }
	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN) { lastInput = '4';/*consInput.add("4");*/ }
	if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN) { lastInput = '5';/*consInput.add("5");*/ }
	if (App->input->GetKey(SDL_SCANCODE_6) == KEY_DOWN) { lastInput = '6';/*consInput.add("6");*/ }
	if (App->input->GetKey(SDL_SCANCODE_7) == KEY_DOWN) { lastInput = '7';/*consInput.add("7");*/ }
	if (App->input->GetKey(SDL_SCANCODE_8) == KEY_DOWN) { lastInput = '8';/*consInput.add("8");*/ }
	if (App->input->GetKey(SDL_SCANCODE_9) == KEY_DOWN) { lastInput = '9';/*consInput.add("9");*/ }
	if (App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN) { lastInput = '0';/*consInput.add("0");*/ }
	if (App->input->GetKey(SDL_SCANCODE_KP_1) == KEY_DOWN) { lastInput = '1';/*consInput.add("1");*/ }
	if (App->input->GetKey(SDL_SCANCODE_KP_2) == KEY_DOWN) { lastInput = '2';/*consInput.add("2");*/ }
	if (App->input->GetKey(SDL_SCANCODE_KP_3) == KEY_DOWN) { lastInput = '3';/*consInput.add("3");*/ }
	if (App->input->GetKey(SDL_SCANCODE_KP_4) == KEY_DOWN) { lastInput = '4';/*consInput.add("4");*/ }
	if (App->input->GetKey(SDL_SCANCODE_KP_5) == KEY_DOWN) { lastInput = '5';/*consInput.add("5");*/ }
	if (App->input->GetKey(SDL_SCANCODE_KP_6) == KEY_DOWN) { lastInput = '6';/*consInput.add("6");*/ }
	if (App->input->GetKey(SDL_SCANCODE_KP_7) == KEY_DOWN) { lastInput = '7';/*consInput.add("7");*/ }
	if (App->input->GetKey(SDL_SCANCODE_KP_8) == KEY_DOWN) { lastInput = '8';/*consInput.add("8");*/ }
	if (App->input->GetKey(SDL_SCANCODE_KP_9) == KEY_DOWN) { lastInput = '9';/*consInput.add("9");*/ }
	if (App->input->GetKey(SDL_SCANCODE_KP_0) == KEY_DOWN) { lastInput = '0';/*consInput.add("0");*/ }

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RSHIFT) == KEY_REPEAT)
	{ 
		if (App->input->GetKey(SDL_SCANCODE_SLASH) == KEY_DOWN) { lastInput = '_';/*consInput.add("_");*/ }
	}
	if (App->input->GetKey(SDL_SCANCODE_PERIOD) == KEY_DOWN) { lastInput = '.';/*consInput.add(".");*/ }
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) { lastInput = ' ';/*consInput.add(" ");*/ }
	if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN)
	{
		ret = false;
		for (int i = 0; i < MAX_INPUT && ret == false; i++)
		{
			if (inputChar[i] == ' ' && i == 0) { ret = true; }
			else if (inputChar[i + 3] == '>')
			{
				if (i > 0)
				{
					inputChar[i] = ' ';
					inputChar[i + 1] = '-';
					inputChar[i + 2] = '>';
					inputChar[i + 3] = ' ';
					ret = true;
				}
				else
				{
					inputChar[i] = ' ';
					inputChar[i + 1] = ' ';
					inputChar[i + 2] = ' ';
					inputChar[i + 3] = ' ';
					ret = true;
				}
			}
		}
	}

	if (lastInput != 'º')
	{
		ret = false;
		for (int i = 0; i < MAX_INPUT && ret == false; i++)
		{
			if (inputChar[0] == ' ' && i == 0)
			{
				if (lastInput == ' ') { ret = true; }
				else
				{
					inputChar[i] = lastInput;
					inputChar[i + 1] = ' ';
					inputChar[i + 2] = '-';
					inputChar[i + 3] = '>';
					ret = true;
				}
			}
			else if (inputChar[i + 2] == '>')
			{
				inputChar[i] = lastInput;
				inputChar[i + 1] = ' ';
				inputChar[i + 2] = '-';
				inputChar[i + 3] = '>';
				ret = true;
			}
		}
	}
	lastInput = 'º';

	return ret;
}

bool j1Console::Commands(p2SString string)
{	
	bool ret = true;

	if (string == list)
	{
		LOG(true, "Command List:");
		LOG(true, "god_mode -> Activates/deactivates God Mode");
		LOG(true, "quit -> Quits game");
		LOG(true, "FPS <number> -> Changer FPS cap");
		LOG(true, "map <map_name> -> Changes current map");
		LOG(true, "photo_mode -> Activates/deactivates Photo Mode");
	}

	else if (string == god_mode)
	{
		if (App->scene->player->godMode == true) { App->scene->player->godMode = false; LOG(true, "God Mode deactivated"); }
		else { App->scene->player->godMode = true; LOG(true, "God Mode activated"); }
	}

	else if (string == quit)
	{
		ret = false;
	}

	else if (string == fps)
	{
		if (App->framerate == 60) { App->framerate = 30; }
		else { App->framerate = 60; }
	}

	else if (string == map1)
	{
		App->scene->StartFirstLevel();
	}
	else if (string == map2)
	{
		App->scene->StartSecondLevel();
	}

	else if (string == photo_mode)
	{

	}

	else { LOG(true, "The command was not on the command list"); }
	
	return ret;
}

void j1Console::EmptyBuffer()
{
	bool r = false;
	for (int i = 0; i < MAX_INPUT && r == false; i++)
	{
		if (inputChar[0] == ' ' && i == 0) { r = true; }
		if (inputChar[i] == '>') { inputChar[i] = ' '; r = true; }
		else { inputChar[i] = ' '; }
	}
}