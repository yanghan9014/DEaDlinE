#ifndef GAMEPLAY_MAPS_H
#define GAMEPLAY_MAPS_H

#include <vector>
#include <string>

#include "gameplay_map.h"
#include "l_texture.h"


class GameplayMap;
class Overlay;
class Scene;

class GameplayMapManager
{
	std::vector<std::string> mNames;
	std::string mRootFolderPath;
	GameplayMap* mMap;

	Overlay* mBackground;//during gameplay
	Overlay* mForeground;

public:
	GameplayMapManager(const char* dataFilepath);
	~GameplayMapManager();

	void loadMap(const int map, Scene* scene);
	void pushMapToSceneAgain() const { mMap->pushToSceneAgain(); }

	GameplayMap& getMap() const { return (*mMap); }

private:
	void free();
};

#endif // GAMEPLAY_MAPS_H
