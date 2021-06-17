#include <iostream>
#include <iomanip>

#include "gameplay_map.h"
#include "scene.h"//for rendering

#define INVALID_ID -1	//-1
#define ANIMATE_TICK 15	//0.25 seconds
#define LOOP_FRAME 8	//8 frames, 2 seconds total


//TODO: map border

struct TileData
{
	ClipIdType clipId[LOOP_FRAME];

	TileIdType tileId;//
	TileIdType damagedTileId;//valid id to have 2hp; 0 to have 1 hp; -1 to be indestructible

	bool doCollision;
	bool isForeground;
};


GameplayMap::GameplayMap(Scene* scene)
	: SceneObject(scene)
{
	mTileDatas = nullptr;
	mTileNum = 0;

	mTileMap = nullptr;
	mMapWidth = 0;
	mMapHeight = 0;

	mClock = 0;//
	mAniFrame = 0;

	mRenderIterator = false;
}
GameplayMap::~GameplayMap()
{
	delete[] mTileDatas;

	for (int i = 0; i < mMapHeight; i++)
		delete[] mTileMap[i];
	delete[] mTileMap;
}

//void GameplayMap::writeToFile(const char* text, const char* filepath) const
//{
//	std::ofstream ofs;
//	ofs.open(filepath, std::ofstream::out | std::ofstream::trunc);//std::ofstream::app; append or overwrite
//	if (ofs.is_open() == false)
//	{
//		std::cout << "[GameplayMap::writeToFile] Opening file: " << filepath << " failed!\n";
//		return;
//	}
//
//	ofs << text;
//	ofs.close();
//}
//void GameplayMap::outputFromFile(const char* filepath) const
//{
//	std::ifstream ifs(filepath);
//	if (ifs.is_open() == false)
//	{
//		std::cout << "[GameplayMap::outputFromFile] failed to open file: " << filepath << " \n";
//		return;
//	}
//
//	int buffer_int;
//	while(ifs.good())
//	{
//		ifs >> buffer_int;
//		if (ifs.good())
//			std::cout << buffer_int << '\n';
//		else
//		{
//			if (ifs.eof())
//				break;
//
//			ifs.clear();
//			ifs.ignore(1000, '\n');
//		}
//	}
//	if (ifs.eof() == false)
//	{
//		std::cout << "[GameplayMap::outputFromFile] Finish reading from file: " << filepath << " failed!\n";
//	}
//}

//TODO: improve fstream usage
void GameplayMap::loadFromFiles(const char* dataFilepath, const char* textureFilepath)
{
	//Select a txt file
	std::ifstream ifs(dataFilepath);
	if (ifs.is_open() == false)
	{
		std::cout << "[GameplayMap::loadFromFiles] Opening file \"" << dataFilepath << "\" failed!\n";
		return;
	}

	//Start input
	mTileNum = getInt(ifs);
	mClipNum = getInt(ifs);

	if (!ifs.good())
	{
		std::cout << "[GameplayMap::loadFromFiles] fail1\n";
		return;
	}

	if (mTileDatas == nullptr)
	{
		mTileDatas = new TileData[mTileNum];
	}
	else//assume the data hasn't been modified, map dimension etc. stays the same; TODO: safety?
	{
		std::cout << "[GameplayMap::loadFromFiles] Overwriting data\n";//
	}
	for (int i = 0; i < mTileNum; i++)
	{
		mTileDatas[i].doCollision = getInt(ifs);
		mTileDatas[i].isForeground = getInt(ifs);
		mTileDatas[i].damagedTileId = getInt(ifs);

		for (int j = 0; j < LOOP_FRAME; j++)
		{
			mTileDatas[i].clipId[j] = getInt(ifs);
		}

		if (!ifs.good())
		{
			std::cout << "[GameplayMap::loadFromFiles] fail\n";
			return;
		}
	}

	if (mTextureClips == nullptr)
	{
		mTextureClips = new SDL_Rect[mClipNum];
	}

	int bufferXCoord, bufferYcoord;
	for (int i = 0; i < mClipNum; i++)
	{
		bufferXCoord = getInt(ifs);
		bufferYcoord = getInt(ifs);
		mTextureClips[i] = {bufferXCoord * TILE_SIZE, bufferYcoord * TILE_SIZE, TILE_SIZE, TILE_SIZE};

		if (!ifs.good())
		{
			std::cout << "[GameplayMap::loadFromFiles] fail\n";
			return;
		}
	}

	mMapWidth = getInt(ifs);
	mMapHeight = getInt(ifs);

	mSpawnX1 = getInt(ifs);
	mSpawnY1 = getInt(ifs);
	mSpawnX2 = getInt(ifs);
	mSpawnY2 = getInt(ifs);

	if (!ifs.good())
	{
		std::cout << "[GameplayMap::loadFromFiles] fail\n";
		return;
	}

	if (mTileMap == nullptr)
	{
		mTileMap = new TileIdType*[mMapHeight];
		for (int i = 0; i < mMapHeight; i++)
		{
			mTileMap[i] = nullptr;
		}
	}//TODO: read char/string rather than individual int?
	for (int i = 0; i < mMapHeight; i++)
	{
		if (mTileMap[i] == nullptr)
		{
			mTileMap[i] = new TileIdType[mMapWidth];
		}
		for (int j = 0; j < mMapWidth; j++)
		{
			mTileMap[i][j] = getInt(ifs);

			if (i == mMapHeight - 1 && j == mMapWidth - 1)
				break;
			if (!ifs.good())
			{
				std::cout << "[GameplayMap::loadFromFiles] fail\n";
				return;
			}
		}
	}
	if (ifs.eof() == false)
	{
		//std::cout << "Finish reading from file: " << dataFilepath << " failed!\n";
	}
//	if (mTileMap[mMapHeight - 1][mMapWidth - 1] == INVALID_ID)
//	{
//		std::cout << "Reading all data failed! Some data fields are missing?(Check your txt file)\n";
//	}

	//Load tile textures
	mLTexture.loadFromFile(textureFilepath);
}
int GameplayMap::getInt(std::ifstream& ifs) const//TODO: move to a utility class?
{
	//TODO: more error detection
	int buffer_int;
	while (true)
	{
		ifs >> buffer_int;
		if (ifs.good())
		{
			return buffer_int;
		}
		else
		{
			ifs.clear();
			ifs.ignore(1000, '\n');//TODO: do not use an arbitrary number
		}
	}
}
void GameplayMap::saveToFile(const char* dataFilepath)
{
	std::ofstream ofs;
	ofs.open(dataFilepath, std::ofstream::out | std::ofstream::trunc);//std::ofstream::appstd::ofstream::trunc; append or overwrite
	if (ofs.is_open() == false)
	{
		std::cout << "[GameplayMap::saveToFile] Opening file \"" << dataFilepath << "\" failed!\n";
		return;
	}
	ofs <<	"Note:\n"
			"Non-number characters have no effect\n"
			"Empty lines have no effect\n"
			"\n"
			"Separate values with whitespaces:\n"
			"(spaces, tabs, etc.)\n"
			"\n"
			"==================================================\n"
			"\n"
			"Number of tiles:\n";
	ofs << mTileNum << '\n';

	ofs <<	"\n"
			"Number of clips:\n";
	ofs << mClipNum << '\n';

	ofs <<	"\n"
			"Tile data:\n"
			"doCollision\tisForeground\tdamaged tile ID"
			" (valid id to have 2hp; 0 to have 1 hp; -1 to be indestructible)\n"
			"clip ID (" << LOOP_FRAME << " frames per loop)\n";
	for (int i = 0; i < mTileNum; i++)
	{
		ofs << mTileDatas[i].doCollision << '\t';
		ofs << mTileDatas[i].isForeground << '\t';
		ofs << (int)(mTileDatas[i].damagedTileId) << '\n';

		for (int j = 0; j < LOOP_FRAME; j++)
		{
			ofs << (int)(mTileDatas[i].clipId[j]) << ' ';
		}
		ofs << "\n\n";
	}

	ofs <<	"Clip data: (x, y) in tiles"
			" (tiles are " << TILE_SIZE << " * " << TILE_SIZE << " pixels in size)\n";
	for (int i = 0; i < mClipNum; i++)
	{
		ofs << (mTextureClips[i].x / TILE_SIZE) << '\t' << (mTextureClips[i].y / TILE_SIZE) << '\n';
	}

	ofs <<	"\n"
			"Map layout: (width, height) in tiles\n";
	ofs << mMapWidth << '\t' << mMapHeight << '\n';

	ofs <<	"\n"
			"Spawn point: (width, height) in tiles\n"
			"Player 1:\n";
	ofs << mSpawnX1 << '\t' << mSpawnY1 << '\n';

	ofs <<	"Player 2:\n";
	ofs << mSpawnX2 << '\t' << mSpawnY2 << '\n';

	ofs <<	"\n"
			"Tile ID:\n"
			"(" << INVALID_ID << " is invalid)\n";
	for (int i = 0; i < mMapHeight; i++)
	{
		for (int j = 0; j < mMapWidth; j++)
		{
			ofs /*<< std::setw(2) << std::setfill('0') */<< (int)(mTileMap[i][j]);
			if (j < mMapWidth - 1)
				ofs << ' ';
		}
		ofs << '\n';
	}

	ofs <<	"\n"
			"==================================================\n"
			"\n"
			"(End of file)";

	ofs.close();
}

int GameplayMap::modifyMap(const int x, const int y, TileIdType newTileId)
{
	if (y < 0 || y >= mMapHeight ||
		x < 0 || x >= mMapWidth)
	{
		std::cout << "[GameplayMap::modifyMap] Coordinate of modified tile is out of bound!\n";
		return -1;
	}
	if (newTileId > mClipNum)
	{
		std::cout << "[GameplayMap::modifyMap] No such tile ID! (ID ranging from 0 to " << mClipNum << ")\n";
		return -1;
	}
	mTileMap[y][x] = newTileId;
	return 0;
}

void GameplayMap::render()
{
	SDL_Rect& cam = mSubscribedScene->mCamera;

	if (!mRenderIterator)
	{
		for (int i = 0; i < mMapHeight; i++)//y
		{
			for (int j = 0; j < mMapWidth; j++)//x
			{
				if ((j + 1) * TILE_SIZE >= cam.x && j * TILE_SIZE <= cam.x + cam.w &&
					(i + 1) * TILE_SIZE >= cam.y && i * TILE_SIZE <= cam.y + cam.h)
				{
					if (!mTileDatas[ mTileMap[i][j] ].isForeground)//background
					{
						mLTexture.render( j * TILE_SIZE - cam.x, i * TILE_SIZE - cam.y,
										&( mTextureClips[mTileDatas[mTileMap[i][j]].clipId[mAniFrame]] ) );
					}
				}
			}
		}
		mRenderIterator = true;//switch to foreground rendering
	}
	else
	{
		for (int i = 0; i < mMapHeight; i++)//y
		{
			for (int j = 0; j < mMapWidth; j++)//x
			{
				if ((j + 1) * TILE_SIZE >= cam.x && j * TILE_SIZE <= cam.x + cam.w &&
					(i + 1) * TILE_SIZE >= cam.y && i * TILE_SIZE <= cam.y + cam.h)
				{
					if (mTileDatas[ mTileMap[i][j] ].isForeground)//foreground
					{
						mLTexture.render( j * TILE_SIZE - cam.x, i * TILE_SIZE - cam.y,
										&( mTextureClips[mTileDatas[mTileMap[i][j]].clipId[mAniFrame]] ) );
					}
				}
			}
		}
		mRenderIterator = false;//switch to background rendering
	}
}
void GameplayMap::renderTextureSheet()
{
	mLTexture.render(0, 0);
}

void GameplayMap::tick()//
{
	mClock++;
	mClock %= (ANIMATE_TICK * LOOP_FRAME);

	mAniFrame = mClock / ANIMATE_TICK;
}

bool GameplayMap::isColliding(const int x, const int y)
{
	int tileX = x / TILE_SIZE;
	int tileY = y / TILE_SIZE;

	if (tileX < 0 || tileX > mMapWidth - 1 ||
		tileY < 0 || tileY > mMapHeight - 1)
	{
		return true;
	}
	else
	{
		return ( mTileDatas[ mTileMap[ tileY ][ tileX ] ].doCollision );
	}
}
