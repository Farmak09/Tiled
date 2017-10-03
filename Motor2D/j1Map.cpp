#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"

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
	if(map_loaded == false)
		return;

	App->render->Blit(TileSets.start->data.texture, 0, 0);
	// TODO 6: Iterate all tilesets and draw all their 
	// images in 0,0 (you should have only one tileset for now)

}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// TODO 2: Make sure you clean up any memory allocated
	// from tilesets / map


	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	if(ret == true)
	{
		FillMapData(map_file.child("map"));

		pugi::xml_node tileset = map_file.child("map").child("tileset");

		for (tileset; tileset; tileset = tileset.next_sibling("tileset"))
			FillTileSetData(tileset);
		// TODO 3: Create and call a private function to load and fill
		// all your map data
	}

	// TODO 4: Create and call a private function to load a tileset
	// remember to support more any number of tilesets!
	

	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", map.width, map.height);
		LOG("tile_width: %d tile_height: %d", map.tilewidth, map.tileheight);

		for (int i = 0;TileSets.count() > i; i++)
		{
			LOG("Tileset --%i--", i+1);
			LOG("name: %s firstgid: %d", TileSets[i].name.GetString(), TileSets[i].firstgid);
			LOG("tile width: %d tile height: %d", TileSets[i].tilewidth, TileSets[i].tileheight);
			LOG("spacing: %d margin: %d", TileSets[i].spacing, TileSets[i].margin);
		}

		// TODO 5: LOG all the data loaded
		// iterate all tilesets and LOG everything
	}

	map_loaded = ret;

	return ret;
}

bool j1Map::FillMapData(pugi::xml_node& node)
{
	p2SString orientation_string = node.attribute("orientation").as_string();

	if (orientation_string		== "orthogonal")
		map.orientation		= orthogonal;
	else if (orientation_string == "isometric")
		map.orientation		= isometric;
	else if (orientation_string == "hexagonal")
		map.orientation		= hexagonal;
	else if (orientation_string == "staggered")
		map.orientation		= staggered;

	p2SString renderorder_string = node.attribute("renderorder").as_string();

	if (renderorder_string		== "right-down")
		map.renderorder		= right_down;
	else if (renderorder_string == "right-up")
		map.renderorder		= right_up;
	else if (renderorder_string == "left-down")
		map.renderorder		= left_down;
	else if (renderorder_string == "left-up")
		map.renderorder		= left_up;

	map.width				= node.attribute("width").as_uint();

	map.height				= node.attribute("height").as_uint();

	map.tilewidth			= node.attribute("tilewidth").as_uint();

	map.tileheight			= node.attribute("tileheight").as_uint();

	map.nextobjectid		= node.attribute("nextobjectid").as_uint();

	return true;
}
bool j1Map::FillTileSetData(pugi::xml_node& tileset)
{
	TileSet TileStruct;

	TileStruct.firstgid		= tileset.attribute("firstgid").as_uint();

	TileStruct.name			= tileset.attribute("name").as_string();

	TileStruct.tilewidth	= tileset.attribute("tilewidth").as_uint();

	TileStruct.tileheight	= tileset.attribute("tileheight").as_uint();

	TileStruct.spacing		= tileset.attribute("spacing").as_uint();

	TileStruct.margin		= tileset.attribute("margin").as_uint();

	TileStruct.tilecount	= tileset.attribute("tilecount").as_uint();

	TileStruct.columns		= tileset.attribute("columns").as_uint();


	TileStruct.imagesource	= tileset.child("image").attribute("source").as_string();

	TileStruct.imageheight	= tileset.child("image").attribute("height").as_uint();
	
	TileStruct.imagewidth	= tileset.child("image").attribute("width").as_uint();

	char image_directory[55] = "maps/";
	strcat(image_directory, tileset.child("image").attribute("source").as_string());
	TileStruct.texture = App->tex->Load(image_directory);

	TileSets.add(TileStruct);

	return true;
}
