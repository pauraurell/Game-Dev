#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "j1Window.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;
	

	folder.create(config.child("folder").child_value());

	return ret;
}

void j1Map::Draw()
{
	if (map_loaded == false)
		return;

	p2List_item<MapLayer*>* item_layer = data.layers.start;
	uint tile_id;

	while (item_layer != NULL)
	{
		MapLayer* l = item_layer->data;
		item_layer = item_layer->next;
		
		if (colliders == false)
		{
			if (l->draw == true)
			{
				for (int i = 0; i < l->width; i++)
				{
					for (int j = 0; j < l->height; j++)
					{
						tile_id = l->gid[l->Get(i, j)];

						if (tile_id != 0)
						{
							SDL_Texture* texture = data.tilesets.start->data->texture;
							iPoint position = MapToWorld(i, j);
							SDL_Rect* sect = &data.tilesets.start->data->getTileRect(tile_id);
							App->render->Blit(texture, position.x, position.y, sect, SDL_FLIP_NONE, l->speed);
						}
					}
				}
			}
		}
	}
}

iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret(0,0);
	
	// TODO 1: Add isometric map to world coordinates
	switch (data.type) {
	case MAPTYPE_ORTHOGONAL:
		ret.x = x * data.tile_width;
		ret.y = y * data.tile_height;
		break;
	case MAPTYPE_ISOMETRIC:
		ret.x = (x - y) * (data.tile_width * 0.5f);
		ret.y = (x + y) * (data.tile_height * 0.5f);
	}
	
	return ret;
}

iPoint j1Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);
	
	switch (data.type) {
	// TODO 2: Add orthographic world to map coordinates
	case MAPTYPE_ORTHOGONAL:
		ret.x = x / data.tile_width;
		ret.y = y / data.tile_height;
		break;
	// TODO 3: Add the case for isometric maps to WorldToMap
	case MAPTYPE_ISOMETRIC:
		ret.x = ((x / (data.tile_width * 0.5f)) + (y / (data.tile_height * 0.5f))) / 2;
		ret.y = ((y / (data.tile_height * 0.5f)) - (x / (data.tile_width * 0.5f))) / 2;
	}
	
	return ret;
}

SDL_Rect TileSet::getTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));

	return rect;
}


// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while (item != NULL)
	{
		App->tex->UnLoad(item->data->texture);
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// Remove all layers
	data.layers.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if (ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for (tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if (ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if (ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}

	// Load layer info ----------------------------------------------

	pugi::xml_node layer;
	for (layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* set1 = new MapLayer();

		if (ret == true)
		{
			ret = LoadLayer(layer, set1);
		}

		data.layers.add(set1);
	}

	pugi::xml_node object;
	p2SString object_name;
	for (object = map_file.child("map").child("objectgroup"); object && ret; object = object.next_sibling("objectgroup"))
	{
		object_name = object.attribute("name").as_string();
		if (object_name == "Colliders")
		{
			LoadColliders(object);
		}
	}


	if (ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while (item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		// Adapt this code with your own variables

		p2List_item<MapLayer*>* item_layer = data.layers.start;
		while (item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d layer speed: %f layer draw: %i collisions: %i", l->width, l->height, l->speed, l->draw, l->collision);
			item_layer = item_layer->next;
		}
	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if (bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if (orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if (orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if (orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if (offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if (set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if (set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;
	pugi::xml_node property;
	property = node.child("properties").child("property");

	layer->name.create(node.attribute("name").as_string());
	layer->width = node.attribute("width").as_uint();
	layer->height = node.attribute("height").as_uint();
	layer->speed = node.child("properties").child("property").attribute("value").as_float();
	layer->collision = property.attribute("value").as_bool();
	property = property.next_sibling("property");
	layer->draw = property.attribute("value").as_bool();
	property = property.next_sibling("property");
	layer->speed = property.attribute("value").as_float();

	layer->gid = new uint[layer->width * layer->height];
	memset(layer->gid, 0, layer->width * layer->height);

	pugi::xml_node tilesgid;
	int i = 0;


	for (tilesgid = node.child("data").child("tile"); tilesgid && ret; tilesgid = tilesgid.next_sibling("tile"))
	{
		layer->gid[i] = tilesgid.attribute("gid").as_uint();
		i++;
	}


	return ret;
}

void j1Map::drawColliders() {
	switch (colliders) {
	case true:
		colliders = false;
		break;
	case false:
		colliders = true;
		break;
	}
}

bool j1Map::LoadColliders(pugi::xml_node& node)
{
	bool ret = true;

	pugi::xml_node object;
	COLLIDER_TYPE collider_type;
	p2SString type;
	for (object = node.child("object"); object; object = object.next_sibling("object"))
	{
		collider_type = COLLIDER_WALL;

		SDL_Rect shape;
		shape.x = object.attribute("x").as_int();
		shape.y = object.attribute("y").as_int();
		shape.w = object.attribute("width").as_int();
		shape.h = object.attribute("height").as_int();

		App->col->AddCollider(shape, collider_type);
	}

	return ret;
}
