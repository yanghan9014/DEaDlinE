#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <stdio.h>

#include "player.h"
#include "sprite.h"
#include "timer.h"


#define DOUBLE_TAP_TIME 250

class Scene;

class game
{
	private:
		Scene* mScene;
		bool mIsGamePadMode;

        player *players[2];
        Uint32 GameStartTime;
        const Uint8 *kState;

        Sprite mDeadline;
        Timer mDeadlineTimer;

    public:
        game(GameplayMap& gameplaymap ,Scene* scene, bool isGamePadMode, int p1ID, int p2ID, int mapID);
        ~game();

        void handleEvent();
        void UpdatePosition();
        void Check_Collision();

		void updateScene();
        void RenderAll();

        Scene* getScene() const { return mScene; }
        player* getPlayer(const int which) { return players[which]; }

        void restart();

	private:
	    // to detect double taps... not really ideal to place these here XP
        Uint32 doubleTap_timer[2];
        bool drop_activated[2];
        bool hatDown_Released[2];  // for gamepads only
        //
		inline float sigmoid(const float x);
};


#endif // GAME_H
