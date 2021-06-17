#include <stdio.h>
#include <sstream>

#include "gameplay_maps.h"
#include "scene.h"
#include "overlay.h"


GameplayMapManager::GameplayMapManager(const char* dataFilepath)//TODO: error msg
	: mMap(nullptr), mForeground(nullptr), mBackground(nullptr)
{
	int i = 0;
	for (; dataFilepath[i] != '/' ; i++)
	{
		mRootFolderPath[i] = dataFilepath[i];
	}
	mRootFolderPath[i] = '\0';//make sure to null terminate :P

	//Select a txt file
	std::ifstream ifs(dataFilepath);
	if (ifs.is_open() == false)
	{
		printf("[GameplayMapManager::loadFromFiles] Opening file %s failed!\n", dataFilepath);
		return;
	}

	while (ifs.good())//TODO: optimize?
	{
		mNames.push_back(std::string());

		ifs >> mNames.back();
	}

	printf("[GameplayMapManager] %d map(s) detected.\n", mNames.size());
}
GameplayMapManager::~GameplayMapManager()
{
	free();//msg
}
void GameplayMapManager::loadMap(const int map, Scene* scene)
{
	if (map > mNames.size() - 1)
		printf("[GameplayMapManager::loadMap] map index(%d) out of bounds(total: %d)!\n", map, mNames.size());

	std::stringstream background;
	background << mRootFolderPath.c_str() << "/" << mNames[map] << "/background.png";
	std::stringstream foreground;
	foreground << mRootFolderPath.c_str() << "/" << mNames[map] << "/foreground.png";
	std::stringstream data;
	data << mRootFolderPath.c_str() << "/" << mNames[map] << "/data.dat";
	std::stringstream texture;
	texture << mRootFolderPath.c_str() << "/" << mNames[map] << "/texture.png";


	free();

	mBackground = new Overlay(scene, background.str().c_str(), Overlay::NONE, 6.0f, 300, 300);
	mForeground = new Overlay(scene, foreground.str().c_str(), Overlay::NONE, 4.0f, 300, 300);

	mMap = new GameplayMap(scene);
	mMap->loadFromFiles(data.str().c_str(), texture.str().c_str());
}

void GameplayMapManager::free()
{
	delete mMap;
	delete mForeground;
	delete mBackground;
}
