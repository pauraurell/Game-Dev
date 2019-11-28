#include "j1EntityManager.h"
#include "j1Entities.h"
#include "j1Player.h"
#include "j1Scene.h"
#include "Animation.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Particles.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Audio.h"
#include "PugiXml/src/pugixml.hpp"
#include "Brofiler/Brofiler.h"

j1EntityManager::j1EntityManager()
{
	
}

j1EntityManager::~j1EntityManager()
{}

bool j1EntityManager::Awake(pugi::xml_node& config) {
	node = config;

	LOG("Awake NODE %d", node.child("initialPosition").attribute("x").as_int());

	return true;
}