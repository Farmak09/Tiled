#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

// TODO 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
// ----------------------------------------------------
struct TileSet
{
	uint firstgid			= 0;
	p2SString name;
	uint tilewidth			= 0;
	uint tileheight			= 0;
	uint spacing			= 0;
	uint margin				= 0;
	uint tilecount			= 0;
	uint columns			= 0;
	p2SString imagesource;
	uint imagewidth			= 0;
	uint imageheight		= 0;

};

enum orientation
{
	invalid = 0,
	orthogonal,
	isometric,
	staggered,
	hexagonal
};
enum renderorder
{
	invalid = 0,
	right_down,
	right_up,
	left_down,
	left_up
};

struct Map
{
	
	orientation orientation	= orientation::invalid;
	renderorder renderorder = renderorder::invalid;
	uint width				= 0;
	uint height				= 0;
	uint tilewidth			= 0;
	uint tileheight			= 0;
	uint nextobjectid		= 0;
};

// TODO 1: Create a struct needed to hold the information to Map node


// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

private:

	bool FillMapData(pugi::xml_node& node);

	bool FillTileSetData(pugi::xml_node& node);
public:

	Map map;

	p2List<TileSet> TileSets;
	// TODO 1: Add your struct for map info as public for now

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__