#ifndef GAMEPLAY_MAP_H
#define GAMEPLAY_MAP_H

#include <SDL.h>

#include <fstream>

#include "scene_object.h"


#define TILE_SIZE 20 //20 pixels squared

typedef char TileIdType;
typedef unsigned char ClipIdType;

struct TileData;

class GameplayMap : public SceneObject
{
private:
	TileData* mTileDatas;
	int mTileNum;

	TileIdType** mTileMap;
	int mMapWidth, mMapHeight;//in number of tiles
	int mSpawnX1, mSpawnY1;//in number of tiles
	int mSpawnX2, mSpawnY2;

	unsigned char mClock;//temporary
	unsigned char mAniFrame;

	bool mRenderIterator;

public:
	GameplayMap(Scene* scene);
	GameplayMap(const GameplayMap&) = delete;
	~GameplayMap();

	//for player::checkCollision
	char** get_mTileMap() const { return mTileMap; }
	int get_mMapWidth() const { return mMapWidth; }
	int get_mMapHeight() const { return mMapHeight; }

	int getSpawnX1() const { return mSpawnX1 * TILE_SIZE + TILE_SIZE / 2; }
	int getSpawnY1() const { return (mSpawnY1 + 1) * TILE_SIZE; }
	int getSpawnX2() const { return mSpawnX2 * TILE_SIZE + TILE_SIZE / 2; }
	int getSpawnY2() const { return (mSpawnY2 + 1) * TILE_SIZE; }

//	void writeToFile(const char* text, const char* filepath) const;//test
//	void outputFromFile(const char* filepath) const;//test

	void loadFromFiles(const char* dataFilepath, const char* textureFilepath);//supports reload
	void saveToFile(const char* dataFilepath);

	int modifyMap(const int x, const int y, TileIdType newTileId);//

	void render() override;//need to be used in pairs!
	void renderTextureSheet();//for debug

	void tick();//TODO: universal clock

	bool isColliding(const int x, const int y);


private:
	int getInt(std::ifstream& ifs) const;
};


#endif // GAMEPLAY_MAP_H
