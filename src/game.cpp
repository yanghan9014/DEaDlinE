#include <cmath>
#include <stdlib.h>

#include "game.h"
#include "scene.h"
#include "character.h"


game::game(GameplayMap& gameplaymap, Scene* scene, bool isGamePadMode, int p1ID, int p2ID, int mapID)
	: mDeadline(scene, "overlays/deadline.png", false)
{
	mScene = scene;

    doubleTap_timer[0] = SDL_GetTicks();
    doubleTap_timer[1] = SDL_GetTicks();
    drop_activated[0] = 0;
    drop_activated[1] = 0;

    mIsGamePadMode = isGamePadMode;
    players[0] = new player(gameplaymap, mScene, 0, p1ID);
    players[1] = new player(gameplaymap, mScene, 1, p2ID);

    scene->setCameraCenterPos(  (gameplaymap.getSpawnX1() + gameplaymap.getSpawnX2() ) / 2,
								(gameplaymap.getSpawnY1() + gameplaymap.getSpawnY2() ) / 2 );

    kState = SDL_GetKeyboardState( NULL );


	SDL_Rect clipQuads[4] = {
		{ 0, 0, 40, 800 },
		{ 40, 0, 40, 800 },
		{ 80, 0, 40, 800 },
		{ 120, 0, 40, 800 }
	};
	mDeadline.setClipQuads(4, clipQuads);

	int currentClips[2] = { 0, 0 };
	SDL_Rect destQuads[2] = {
		{ -340, 0, 40, 800 },
		{ 2700, 0, 40, 800 }
	};
	mDeadline.setDestQuads(2, currentClips, destQuads);
}
game::~game()
{
	delete players[0];
	delete players[1];
}

void game::handleEvent()
{
	bool handleEventLog = false;//printf on/off

    if (mIsGamePadMode)
	{
        for(int p=0; p<2; p++){

            // double tapping too slow
            if(SDL_GetTicks() - doubleTap_timer[p] > DOUBLE_TAP_TIME){
                drop_activated[p] = 0;
            }
            Uint8 move_dir = SDL_JoystickGetHat(gamePad[p], 0);
            switch( move_dir )
            {
            case SDL_HAT_LEFTUP:
                players[p]->pl_wp_move(1, 0, dir_left);
                hatDown_Released[p] = 1;
                break;
            case SDL_HAT_LEFT:
                players[p]->pl_wp_move(0, 0, dir_left);
                hatDown_Released[p] = 1;
                break;
            case SDL_HAT_LEFTDOWN:
                players[p]->pl_wp_move(0, 1, dir_left);
                if(drop_activated == 0){
                    drop_activated[p] = 1;
                    doubleTap_timer[p] = SDL_GetTicks();
                }
                else if(drop_activated[p] == 1 && hatDown_Released[p] == 1){
                    players[p]->drop();
                }
                hatDown_Released[p] = 0;
                break;
            case SDL_HAT_DOWN:
                players[p]->pl_wp_move(0, 1, dir_none);
                if(drop_activated == 0){
                    drop_activated[p] = 1;
                    doubleTap_timer[p] = SDL_GetTicks();
                }
                else if(drop_activated[p] == 1 && hatDown_Released[p] == 1){
                    players[p]->drop();
                }
                hatDown_Released[p] = 0;
                break;
            case SDL_HAT_RIGHTDOWN:
                players[p]->pl_wp_move(0, 1, dir_right);
                if(drop_activated == 0){
                    drop_activated[p] = 1;
                    doubleTap_timer[p] = SDL_GetTicks();
                }
                else if(drop_activated[p] == 1 && hatDown_Released[p] == 1){
                    players[p]->drop();
                }
                hatDown_Released[p] = 0;
                break;
            case SDL_HAT_RIGHT:
                players[p]->pl_wp_move(0, 0, dir_right);
                hatDown_Released[p] = 1;
                break;
            case SDL_HAT_RIGHTUP:
                players[p]->pl_wp_move(1, 0, dir_right);
                hatDown_Released[p] = 1;
                break;
            case SDL_HAT_UP:
                players[p]->pl_wp_move(1, 0, dir_none);
                hatDown_Released[p] = 1;
                break;
            case SDL_HAT_CENTERED:
                players[p]->pl_wp_move(0, 0, dir_none);
                hatDown_Released[p] = 1;
                break;
            default:
                players[p]->pl_wp_move(0, 0, dir_none);
                hatDown_Released[p] = 1;
                break;
            }
            Sint16 aim_dir[2];
            aim_dir[0] = SDL_JoystickGetAxis(gamePad[p], rightX);
            aim_dir[1] = SDL_JoystickGetAxis(gamePad[p], rightY);
            if(fabs(aim_dir[0]) < JOYSTICK_DEADZONE && fabs(aim_dir[1]) < JOYSTICK_DEADZONE){
                players[p]->set_wp_angle( atan2( (double)players[1-p]->getCoordY() - players[p]->getCoordY(), (double)players[1-p]->getCoordX() - players[p]->getCoordX() ) * ( 180.0 / M_PI ) );
            }
            else
                players[p]->set_wp_angle( atan2( (double)aim_dir[1], (double)aim_dir[0] ) * ( 180.0 / M_PI ) );

				//printf("aimDir: %.2f\n", mWeapon.weaponAngle);

            if( players[p]->get_wp_angle() < -90){
                players[p]->pl_flip = SDL_FLIP_HORIZONTAL;
                players[p]->wp_flip = SDL_FLIP_VERTICAL;
            }
            else if( players[p]->get_wp_angle() > 90){
                players[p]->pl_flip = SDL_FLIP_HORIZONTAL;
                players[p]->wp_flip = SDL_FLIP_VERTICAL;
            }
            else{
                players[p]->pl_flip = SDL_FLIP_NONE;
                players[p]->wp_flip = SDL_FLIP_NONE;
            }
            Uint8 lastState = players[p]->gamePadbuttonState[ RightTrigger2 ];
            players[p]->gamePadbuttonState[ RightTrigger2 ] = SDL_JoystickGetButton(gamePad[p], RightTrigger2);
            if(lastState == 0 && players[p]->gamePadbuttonState[ RightTrigger2 ] == 1){
                players[p]->fire();
            }
            Uint8 lastSwitch = players[p]->gamePadbuttonState[ LeftTrigger2 ];
            players[p]->gamePadbuttonState[ LeftTrigger2 ] = SDL_JoystickGetButton(gamePad[p], LeftTrigger2);
            if(lastSwitch == 0 && players[p]->keyBoardState[ key_switch ] == 1){
                players[p]->switchWeapon();
            }
        }
    }
	else
    {
        Uint8 lastState[2];
        lastState[0] = players[0]->keyBoardState[ key_fire ];
        lastState[1] = players[1]->keyBoardState[ key_fire ];
        Uint8 lastDrop[2];
        lastDrop[0] = players[0]->keyBoardState[ key_down ];
        lastDrop[1] = players[1]->keyBoardState[ key_down ];
        Uint8 lastSwitch[2];
        lastSwitch[0] = players[0]->keyBoardState[ key_switch ];
        lastSwitch[1] = players[1]->keyBoardState[ key_switch ];

//        players[1]->keyBoardState[ key_up ] = kState[ SDL_SCANCODE_UP ];
//        players[1]->keyBoardState[ key_left ] = kState[ SDL_SCANCODE_LEFT ];
//        players[1]->keyBoardState[ key_down ] = kState[ SDL_SCANCODE_DOWN ];
//        players[1]->keyBoardState[ key_right ] = kState[ SDL_SCANCODE_RIGHT ];
//        players[1]->keyBoardState[ key_fire ] = kState[ SDL_SCANCODE_RETURN ];
//        players[1]->keyBoardState[ key_switch ] = kState[ SDL_SCANCODE_RSHIFT ];
        players[1]->keyBoardState[ key_up ] = kState[ SDL_SCANCODE_I ];
        players[1]->keyBoardState[ key_left ] = kState[ SDL_SCANCODE_J ];
        players[1]->keyBoardState[ key_down ] = kState[ SDL_SCANCODE_K ];
        players[1]->keyBoardState[ key_right ] = kState[ SDL_SCANCODE_L ];
        players[1]->keyBoardState[ key_fire ] = kState[ SDL_SCANCODE_PERIOD ];
        players[1]->keyBoardState[ key_switch ] = kState[ SDL_SCANCODE_SLASH ];

        players[0]->keyBoardState[ key_up ] = kState[ SDL_SCANCODE_W ];
        players[0]->keyBoardState[ key_left ] = kState[ SDL_SCANCODE_A ];
        players[0]->keyBoardState[ key_down ] = kState[ SDL_SCANCODE_S ];
        players[0]->keyBoardState[ key_right ] = kState[ SDL_SCANCODE_D ];
        //players[0]->keyBoardState[ key_fire ] = kState[ SDL_SCANCODE_SPACE ];
        //players[0]->keyBoardState[ key_switch ] = kState[ SDL_SCANCODE_LSHIFT ];
        players[0]->keyBoardState[ key_fire ] = kState[ SDL_SCANCODE_C ];
        players[0]->keyBoardState[ key_switch ] = kState[ SDL_SCANCODE_V ];

        for(int p=0; p<2; p++){
            if(lastState[p] == 0 && players[p]->keyBoardState[ key_fire ] == 1){
                players[p]->fire();
            }
            if(lastSwitch[p] == 0 && players[p]->keyBoardState[ key_switch ] == 1){
                players[p]->switchWeapon();
            }

            // double tapping too slow
            if(SDL_GetTicks() - doubleTap_timer[p] > DOUBLE_TAP_TIME){
                drop_activated[p] = 0;
            }
            if(lastDrop[p] == 0 && players[p]->keyBoardState[ key_down ] == 1){
                if( drop_activated[p] == 0 ){
                    drop_activated[p] = 1;
                    doubleTap_timer[p] = SDL_GetTicks();
                }
                else{
                    players[p]->drop();
                    drop_activated[p] = 0;
                }
            }
            if( players[p]->keyBoardState[ key_up ] ){
                if( players[p]->keyBoardState[ key_left ] ||  players[p]->keyBoardState[ key_right ] ){

                    if( players[p]->keyBoardState[ key_left ] ){
                        players[p]->pl_wp_move(1, 0, dir_left);

                        if(players[p]->attacking == 0){
                            players[p]->set_pl_dir(pl_left);
                            players[p]->pl_flip = SDL_FLIP_HORIZONTAL;
                            players[p]->wp_flip = SDL_FLIP_VERTICAL;
                            switch(players[p]->get_cur_wp().proj_type)
                            {
                            case proj_none:
                                players[p]->set_wp_angle(180);
                                break;
                            case proj_bullet:
                                players[p]->set_wp_angle(180);
                                players[p]->set_aim_angle(180);
                                break;
                            case proj_accelerated_bullet:
                                players[p]->set_wp_angle(180);
                                players[p]->set_aim_angle(180);
                                break;
//                            case proj_grenade:
//                                players[p]->set_wp_angle(180);
//                                players[p]->set_aim_angle(-135);
//                                break;
                            }
                        }

						if (handleEventLog)
							printf("left jump\n");
                    }
                    if( players[p]->keyBoardState[ key_right ] ){
                        players[p]->pl_wp_move(1, 0, dir_right);

                        if(players[p]->attacking == 0){
                            players[p]->set_pl_dir(pl_right);
                            players[p]->pl_flip = SDL_FLIP_NONE;
                            players[p]->wp_flip = SDL_FLIP_NONE;
                            switch(players[p]->get_cur_wp().proj_type)
                            {
                            case proj_none:
                                players[p]->set_wp_angle(0);
                                break;
                            case proj_bullet:
                                players[p]->set_wp_angle(0);
                                players[p]->set_aim_angle(0);
                                break;
                            case proj_accelerated_bullet:
                                players[p]->set_wp_angle(0);
                                players[p]->set_aim_angle(0);
                                break;
//                            case proj_grenade:
//                                players[p]->set_wp_angle(0);
//                                players[p]->set_aim_angle(-45);
//                                break;
                            }
                        }


						if (handleEventLog)
							printf("right jump\n");
                    }
                }
                else{
                    players[p]->pl_wp_move(1, 0, dir_none);
                }
            }
            else if( players[p]->keyBoardState[ key_down ] ){
                if( players[p]->keyBoardState[ key_left ] ||  players[p]->keyBoardState[ key_right ] ){
                    players[p]->pl_wp_move(0, 1, dir_left);
                    if( players[p]->keyBoardState[ key_left ] ){
                        players[p]->set_pl_dir(pl_left);
                        players[p]->pl_flip = SDL_FLIP_HORIZONTAL;
                        players[p]->wp_flip = SDL_FLIP_VERTICAL;
                        switch(players[p]->get_cur_wp().proj_type)
                        {
                        case proj_none:
                            players[p]->set_wp_angle(180);
                            break;
                        case proj_bullet:
                            players[p]->set_wp_angle(180);
                            players[p]->set_aim_angle(180);
                            break;
                        case proj_accelerated_bullet:
                            players[p]->set_wp_angle(180);
                            players[p]->set_aim_angle(180);
                            break;
//                        case proj_grenade:
//                            players[p]->set_wp_angle(180);
//                            players[p]->set_aim_angle(-135);
//                            break;
                        }

						if (handleEventLog)
							printf("left squat\n");
                    }
                    if( players[p]->keyBoardState[ key_right ] ){
                        players[p]->pl_wp_move(0, 1, dir_right);
                        if(players[p]->attacking == 0){
                            players[p]->set_pl_dir(pl_right);
                            players[p]->pl_flip = SDL_FLIP_NONE;
                            players[p]->wp_flip = SDL_FLIP_NONE;
                            switch(players[p]->get_cur_wp().proj_type)
                            {
                            case proj_none:
                                players[p]->set_wp_angle(0);
                                break;
                            case proj_bullet:
                                players[p]->set_wp_angle(0);
                                players[p]->set_aim_angle(0);
                                break;
                            case proj_accelerated_bullet:
                                players[p]->set_wp_angle(0);
                                players[p]->set_aim_angle(0);
                                break;
//                            case proj_grenade:
//                                players[p]->set_wp_angle(0);
//                                players[p]->set_aim_angle(-45);
//                                break;
                            }
                        }

						if (handleEventLog)
							printf("right squat\n");
                    }
                }
                else{
                    players[p]->pl_wp_move(0, 1, dir_none);
                }
            }
            else {
                if( players[p]->keyBoardState[ key_left ] ||  players[p]->keyBoardState[ key_right ] ){

                    if( players[p]->keyBoardState[ key_left ] ){
                        players[p]->pl_wp_move(0, 0, dir_left);

                        if(players[p]->attacking == 0){
                            players[p]->set_pl_dir(pl_left);
                            players[p]->pl_flip = SDL_FLIP_HORIZONTAL;
                            players[p]->wp_flip = SDL_FLIP_VERTICAL;
                            switch(players[p]->get_cur_wp().proj_type)
                            {
                            case proj_none:
                                players[p]->set_wp_angle(180);
                                break;
                            case proj_bullet:
                                players[p]->set_wp_angle(180);
                                players[p]->set_aim_angle(180);
                                break;
                            case proj_accelerated_bullet:
                                players[p]->set_wp_angle(180);
                                players[p]->set_aim_angle(180);
                                break;
//                            case proj_grenade:
//                                players[p]->set_wp_angle(180);
//                                players[p]->set_aim_angle(-135);
//                                break;
                            }
                        }


						if (handleEventLog)
							printf("left\n");
                    }
                    if( players[p]->keyBoardState[ key_right ] ){
                        players[p]->pl_wp_move(0, 0, dir_right);

                        if(players[p]->attacking == 0){
                            players[p]->set_pl_dir(pl_right);
                            players[p]->pl_flip = SDL_FLIP_NONE;
                            players[p]->wp_flip = SDL_FLIP_NONE;
                            switch(players[p]->get_cur_wp().proj_type)
                            {
                            case proj_none:
                                players[p]->set_wp_angle(0);
                                break;
                            case proj_bullet:
                                players[p]->set_wp_angle(0);
                                players[p]->set_aim_angle(0);
                                break;
                            case proj_accelerated_bullet:
                                players[p]->set_wp_angle(0);
                                players[p]->set_aim_angle(0);
                                break;
//                            case proj_grenade:
//                                players[p]->set_wp_angle(0);
//                                players[p]->set_aim_angle(-45);
//                                break;
                            }
                        }


						if (handleEventLog)
							printf("right\n");
                    }
                }
                else{
                    players[p]->pl_wp_move(0, 0, dir_none);
                }
            }
        }
    }
}

void game::UpdatePosition()
{
	players[0]->updatePosition();
	players[1]->updatePosition();


	int time = mDeadlineTimer.getElapsed();

	int currentClips[2] = { (time % 1024) / 256, (time % 1024) / 256 };
	SDL_Rect destQuads[2] = {
		//left deadline; left boundary is ((time / 40) - 340)
		//width is 40
		{ (time / 40) - 340, 0, 40, 800 },
		//right deadline; left boundary is (2700 - (time / 40))
		{ 2700 - (time / 40), 0, 40, 800 }
	};
	mDeadline.setDestQuads(2, currentClips, destQuads);
}

//void game::Check_Collision()
//{
//	players[0]->checkCollision(*players[1]);
//	players[1]->checkCollision(*players[0]);
//}
void game::Check_Collision()
{
	int time = mDeadlineTimer.getElapsed();

	players[0]->checkCollision(*players[1]);
	players[1]->checkCollision(*players[0]);
	for(int i=0; i<2; i++){
        if(players[i]->getID() != RedCloud){
            if(players[i]->getCoordX() + players[i]->P_c_box.x <= ((time / 40) - 340) + 40 || players[i]->getCoordX() + players[i]->P_c_box.x + players[i]->P_c_box.w >= (2700 - (time / 40))){
                players[i]->setVelocityX(0);
                players[i]->set_cur_health(0);
            }
        }
	}
}

void game::updateScene()
{
	mScene->smoothUpdate(players[0]->getCoordX(), players[0]->getCoordY(),
						 players[1]->getCoordX(), players[1]->getCoordY());
}

void game::RenderAll()
{
    for(int p=0; p<2; p++){
		players[p]->render();
    }
}
void game::restart()
{
	players[0]->reset();
	players[1]->reset();

	mDeadlineTimer.restart();
}
