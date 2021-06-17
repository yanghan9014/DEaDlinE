#ifndef ENTITY_H
#define ENTITY_H

#include "l_texture.h"


#define PLAYER_CLIP_NUM 6
#define GRAVITY 1.0

class entity
{
	protected:
		int coord[2];
		double velocity[2];
    public:
		//SDL_Rect skin_clips[ PLAYER_CLIP_NUM ];

        void baby_step() { coord[0] += velocity[0];  coord[1] += velocity[1]; }
		int getCoordX() { return coord[0]; }
		int getCoordY() { return coord[1]; }
		double getVelocityX() { return velocity[0]; }
		double getVelocityY() { return velocity[1]; }

		void setCoordX(int x) { coord[0] = x; }
		void setCoordY(int y) { coord[1] = y; }
		void setVelocityX(double x) { velocity[0] = x; }
		void setVelocityY(double y) { velocity[1] = y; }
};


#endif // ENTITY_H
