#include <iostream> 

#include "p2Defs.h"
#include "p2Log.h"

#include "j1Window.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1App.h"
#include "j1Player.h"
#include "j1Bat.h"
#include "j1Skeleton.h"
#include "j1FadeToBlack.h"
#include "j1Collision.h"
#include "j1Particles.h"
#include "j1EntityManager.h"
#include "j1Pathfinding.h"
#include "j1Fonts.h"
#include "j1UI.h"
#include "j1Console.h"
#include "j1MainMenu.h"
#include "j1Credits.h"
#include "j1Score.h"


// Constructor
j1App::j1App(int argc, char* args[]) : argc(argc), args(args)
{
	frames = 0;

	want_to_save = want_to_load = false;

	input = new j1Input();
	win = new j1Window();
	render = new j1Render();
	tex = new j1Textures();
	audio = new j1Audio();
	scene = new j1Scene();
	map = new j1Map();
	fade = new j1FadeToBlack();
	col = new j1Collision();
	particles = new j1Particles();
	entManager = new j1EntityManager();
	pathfinding = new j1PathFinding();
	fonts = new j1Fonts();
	ui = new j1UI();
	cons = new j1Console();
	main_menu = new j1MainMenu();
	cred = new j1Credits();
	score = new j1Score();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);
	AddModule(map);
	AddModule(scene, false);
	AddModule(entManager);
	AddModule(particles);
	AddModule(col);
	AddModule(score, false);
	AddModule(ui);
	AddModule(cons, false);
	AddModule(main_menu);
	AddModule(cred, false);
	AddModule(fade);
	AddModule(pathfinding);
	AddModule(fonts);

	// render last to swap buffer
	AddModule(render);

}

// Destructor
j1App::~j1App()
{
	// release modules
	p2List_item<j1Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.clear();
}

void j1App::AddModule(j1Module* module, bool init)
{
	module->active = init;
	if (init == false) { module->to_enable = false; }
	modules.add(module);
}

// Called before render is available
bool j1App::Awake()
{
	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;

	bool ret = false;
		
	config = LoadConfig(config_file);

	if(config.empty() == false)
	{
		// self-config
		ret = true;
		app_config = config.child("app");
		title.create(app_config.child("title").child_value());
		organization.create(app_config.child("organization").child_value());
		frameratecap = app_config.child("framerateCap").attribute("fps").as_int();;
	}

	if(ret == true)
	{
		p2List_item<j1Module*>* item;
		item = modules.start;

		while(item != NULL && ret == true)
		{
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}

	return ret;
}

// Called before the first frame
bool j1App::Start()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		if (item->data->active == false) { item->data->CleanUp(); }
		item = item->next;
	}

	FrameCapEnabled = true;
	QuitToDesktop = false;

	return ret;
}

// Called each loop iteration
bool j1App::Update()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		if (item->data->to_enable == true)
		{
			item->data->Enable();
			item->data->to_enable = false;
		}
		item = item->next;
	}
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// ---------------------------------------------
pugi::xml_node j1App::LoadConfig(pugi::xml_document& config_file) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = config_file.load_file("config.xml");

	if(result == NULL)
		LOG(false, "Could not load map xml file config.xml. pugi error: %s", result.description());
	else
		ret = config_file.child("config");

	return ret;
}

// ---------------------------------------------
void j1App::PrepareUpdate()
{
	frame_count++;
	last_sec_frame_count++;

	// Let's pause the game.
	if (!pause)
		dt = frame_time.ReadSec();
	else
		dt = 0.0f;

	frame_time.Start();
}

// ---------------------------------------------
void j1App::FinishUpdate()
{
	if (want_to_save == true)
		SaveGameNow();

	if (want_to_load == true)
		LoadGameNow();

	// Framerate

	if (last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	avg_fps = float(frame_count) / startup_time.ReadSec();
	seconds_since_startup = startup_time.ReadSec();
	last_frame_ms = frame_time.Read();
	frames_on_last_update = prev_last_sec_frame_count;



	if (last_frame_ms < (1000 / frameratecap))
	{
		j1PerfTimer timer;
		SDL_Delay((1000 / frameratecap) - last_frame_ms);


	}

	if (App->FrameCapEnabled)
		capStr.create("Enabled");

	if (frameratecap == 30)
	{
		p2SString title("%s ||  Av. fps: %.2f  ||  Frames last second: %d  ||  Last Frame Ms: %u  ||  FrameCap: %s   || Vsync: Disabled",
		GetTitle(),  avg_fps, frames_on_last_update, last_frame_ms,  capStr.GetString());
		App->win->SetTitle(title.GetString());
		App->render->vsyncActive = false;
	}

	else if (frameratecap == 60)
	{
		p2SString title("%s ||  Av. fps: %.2f  ||  Frames last second: %d  ||  Last Frame Ms: %u  ||  FrameCap: Disabled   || Vsync: Enabled",
			GetTitle(), avg_fps, frames_on_last_update, last_frame_ms);
		App->win->SetTitle(title.GetString());
		App->render->vsyncActive = true;
	}

	Mix_VolumeMusic(App->audio->volume_music);
}

// Call modules before each loop iteration
bool j1App::PreUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool j1App::DoUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool j1App::PostUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	if (QuitToDesktop) { return false; }



	return ret;
}

// Called before quitting
bool j1App::CleanUp()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}
	return ret;
}

// ---------------------------------------
int j1App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* j1App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* j1App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* j1App::GetOrganization() const
{
	return organization.GetString();
}

// Load / Save
void j1App::LoadGame()
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list
	want_to_load = true;
}

// ---------------------------------------
void j1App::SaveGame() const
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list ... should we overwrite ?

	want_to_save = true;
}

// ---------------------------------------
void j1App::GetSaveGames(p2List<p2SString>& list_to_fill) const
{
	// need to add functionality to file_system module for this to work
}

bool j1App::LoadGameNow()
{
	bool ret = false;

	pugi::xml_document data;
	pugi::xml_node root;
	load_game.create("save_game.xml");

	pugi::xml_parse_result result = data.load_file(load_game.GetString());

	if(result != NULL)
	{
		LOG(true, "Loading new Game State from %s...", load_game.GetString());

		root = data.child("game_state");

		p2List_item<j1Module*>* item = modules.start;
		ret = true;

		while(item != NULL && ret == true)
		{
			ret = item->data->Load(root.child(item->data->name.GetString()));
			item = item->next;
		}

		data.reset();
		if(ret == true)
			LOG(true, "...finished loading");
		else
			LOG(true, "...loading process interrupted with error on module %s", (item != NULL) ? item->data->name.GetString() : "unknown");
	}
	else
		LOG(true, "Could not parse game state xml file %s. pugi error: %s", load_game.GetString(), result.description());

	want_to_load = false;
	return ret;
}

bool j1App::SaveGameNow() const
{
	bool ret = true;

	LOG(true, "Saving Game State to %s...", save_game.GetString());

	// xml object were we will store all data
	pugi::xml_document data;
	pugi::xml_node root;
	save_game.create("save_game.xml");
	
	root = data.append_child("game_state");

	p2List_item<j1Module*>* item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Save(root.append_child(item->data->name.GetString()));
		item = item->next;
	}

	if(ret == true)
	{
		data.save_file(save_game.GetString());
		LOG(true, "... finished saving", save_game.GetString());
	}
	else
		LOG(true, "Save process halted from an error in module %s", (item != NULL) ? item->data->name.GetString() : "unknown");

	data.reset();
	want_to_save = false;
	return ret;
}