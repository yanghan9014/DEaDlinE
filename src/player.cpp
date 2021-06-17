#include <cmath>

#include "player.h"
#include "scene.h"
#include "character.h"
#include <stdlib.h>//


const int PLAYER_WIDTH = 40;
const int PLAYER_HEIGHT = 60;

player::player(GameplayMap& gameplaymap, Scene* scene, Uint8 pl_index, Uint8 characterID)
	: SceneObject(scene),
	clip_timer(0),
	m_gameplaymap(gameplaymap),
	mWhich(pl_index),
    ID(characterID),
	P_pl_speed     (cha_List[ characterID ]->pl_speed),
    P_squat_speed  (cha_List[ characterID ]->pl_squat_speed),
    P_jump_velocity(cha_List[ characterID ]->pl_jump_velocity),
    P_full_health  (cha_List[ characterID ]->full_health),
    skinSprite(mLTexture)
{
	if (pl_index == 0)
	{
		setCoordX(m_gameplaymap.getSpawnX1() - PLAYER_WIDTH / 2);
		setCoordY(m_gameplaymap.getSpawnY1() - PLAYER_HEIGHT);
        player_direction = pl_right;
	}
	else
	{
		setCoordX(m_gameplaymap.getSpawnX2() - PLAYER_WIDTH / 2);
		setCoordY(m_gameplaymap.getSpawnY2() - PLAYER_HEIGHT);
        player_direction = pl_left;
	}
    you_failed = 0;
    death_start = 0;


    setVelocityX(0);
    setVelocityY(0);
    cur_health = P_full_health;
    dropping = 0;
    airborne = 1;
    first_shot = last_shot = 0;
    current_wp = 0;
    switch_pending = 0;
    attacking = 0;
    reloading = 0;
    damage_dealt = 0;
    attack_timer = 0;
    P_fire_count = 1;
    P_fire_interval = 0;
    P_omega = 0;

    visible = 1;
    visible_timer = SDL_GetTicks();
    visible_delay = 200;

    P_original_clip = cha_List[ characterID ]->original_clip;
    P_c_box = cha_List[ characterID ]->c_box;
    P_death_clip = cha_List[ characterID ]->death_clip;

    for(int i=0; i<3; i++){
        P_weapons[i] = cha_List[ characterID ]->cha_wps[i];
        if(cha_List[characterID]->wp_skin_path[i] != NULL){
            if( !wp_sprite[i].loadFromFile( cha_List[characterID]->wp_skin_path[i] ) ){
                printf( "Failed to load sprite sheet !\n" );
            }
        }
        if(cha_List[characterID]->proj_skin_path[i] != NULL){
            if( !proj_sprite[i].loadFromFile( cha_List[characterID]->proj_skin_path[i] ) ){
                printf( "Failed to load sprite sheet !\n" );
            }
        }
        if(cha_List[characterID]->SE_path[i] != NULL){
            if( !special_effect_sprite[i].loadFromFile( cha_List[characterID]->SE_path[i] ) ){
                printf( "Failed to load sprite sheet !\n" );
            }
            Special_effect_clips[i] = new SDL_Rect [P_weapons[i].special_effect_clip_num];
            for(int j=0; j<P_weapons[i].special_effect_clip_num; j++){
                Special_effect_clips[i][j] = P_weapons[i].special_effect_clip[j];
            }
        }
    }

    for(int i=0; i<MAX_BULLET_COUNT; i++){
        proj_live[i] = 0;
    }
    for(int i=0; i<MAX_REPEAT_FIRE; i++){
        fired[i] = 0;
    }

    for(int j=0; j<PLAYER_CLIP_NUM; j++){
        skin_clips[ j ].x = P_original_clip.w * j;
        skin_clips[ j ].y = P_original_clip.y;
        skin_clips[ j ].w = P_original_clip.w;
        skin_clips[ j ].h = P_original_clip.h;
    }

    for(int i=PLAYER_CLIP_NUM; i < PLAYER_CLIP_NUM + DEATH_CLIP_NUM; i++){
        skin_clips[i].x = P_original_clip.w * PLAYER_CLIP_NUM + P_death_clip.w * (i-PLAYER_CLIP_NUM);
        skin_clips[i].y = P_death_clip.y;
        skin_clips[i].w = P_death_clip.w;
        skin_clips[i].h = P_death_clip.h;
    }

    reset_wp_aim_angle();

    for(int i=0; i< GAMEPAD_BUTTON_COUNT; i++){
        gamePadbuttonState[i] = SDL_JoystickGetButton(gamePad[ pl_index ], RightTrigger2);
    }

    //if( !skinSprite.loadFromFile( cha_List[characterID]->skin_path.c_str() ) ){
	if( !skinSprite.loadFromFile( cha_List[characterID]->skin_path ) ){
        printf( "Failed to load sprite sheet !\n" );
    }

    switch (characterID)
    {
	case Sniper: mMainWeapon.loadWav("audio/sniper_shot.wav"); break;
    case Assassin: mMainWeapon.loadWav("audio/throwing_knife.wav"); break;
    case RedCloud: mMainWeapon.loadWav("audio/throwing_knife.wav"); break;
    case TA: mMainWeapon.loadWav("audio/laser_blast.wav"); break;
    case Yeet: mMainWeapon.loadWav("audio/throwing_knife.wav"); break;
    case MrEdge: mMainWeapon.loadWav("audio/throwing_knife.wav"); break;
    }
    mSideWeapon.loadWav("audio/pistol_shot.wav");
    mMeleeWeapon.loadWav("audio/sword_slash.wav");
    mDownDown.loadWav("audio/down_down.wav");
    mDamaged.loadWav("audio/damaged.wav");
}
void player::reset()
{
	if (mWhich == 0)
	{
		setCoordX(m_gameplaymap.getSpawnX1() - PLAYER_WIDTH / 2);
		setCoordY(m_gameplaymap.getSpawnY1() - PLAYER_HEIGHT);
	}
	else
	{
		setCoordX(m_gameplaymap.getSpawnX2() - PLAYER_WIDTH / 2);
		setCoordY(m_gameplaymap.getSpawnY2() - PLAYER_HEIGHT);
	}
	cur_health = P_full_health;
	death_start = 0;
	you_failed = 0;
	for(int i=0; i<MAX_BULLET_COUNT; i++){
        destroy_proj(i);
	}
}

void player::reset_wp_aim_angle()
{
    switch (P_weapons[current_wp].get_wp_ID())
        {
            case generic_pistol:
                if(player_direction == pl_left){
                    wp_angle = aim_angle = 180;
                    pl_flip = SDL_FLIP_HORIZONTAL;
                    wp_flip = SDL_FLIP_VERTICAL;
                }
                else if(player_direction == pl_right){
                    wp_angle = aim_angle = 0;
                    pl_flip = SDL_FLIP_NONE;
                    wp_flip = SDL_FLIP_NONE;
                }
                break;
            case generic_knife:
                if(player_direction == pl_left){
                    wp_angle = aim_angle = 180;
                    pl_flip = SDL_FLIP_HORIZONTAL;
                    wp_flip = SDL_FLIP_VERTICAL;
                }
                else if(player_direction == pl_right){
                    wp_angle = aim_angle = 0;
                    pl_flip = SDL_FLIP_NONE;
                    wp_flip = SDL_FLIP_NONE;
                }
                break;
            case wp_sniper:
                if(player_direction == pl_left){
                    wp_angle = aim_angle = 180;
                    pl_flip = SDL_FLIP_HORIZONTAL;
                    wp_flip = SDL_FLIP_VERTICAL;
                }
                else if(player_direction == pl_right){
                    wp_angle = aim_angle = 0;
                    pl_flip = SDL_FLIP_NONE;
                    wp_flip = SDL_FLIP_NONE;
                }
                break;
            case wp_assassin:
                if(player_direction == pl_left){
                    wp_angle = aim_angle = 180;
                    pl_flip = SDL_FLIP_HORIZONTAL;
                    wp_flip = SDL_FLIP_VERTICAL;
                }
                else if(player_direction == pl_right){
                    wp_angle = aim_angle = 0;
                    pl_flip = SDL_FLIP_NONE;
                    wp_flip = SDL_FLIP_NONE;
                }
                break;
            case wp_redCloud:
                if(player_direction == pl_left){
                    wp_angle = 180;
                    aim_angle = 180;
                    pl_flip = SDL_FLIP_HORIZONTAL;
                    wp_flip = SDL_FLIP_VERTICAL;
                }
                else if(player_direction == pl_right){
                    wp_angle = 0;
                    aim_angle = 0;
                    pl_flip = SDL_FLIP_NONE;
                    wp_flip = SDL_FLIP_NONE;
                }
                break;
            case wp_ta:
                if(player_direction == pl_left){
                    wp_angle = 180;
                    aim_angle = 180;
                    pl_flip = SDL_FLIP_HORIZONTAL;
                    wp_flip = SDL_FLIP_VERTICAL;
                }
                else if(player_direction == pl_right){
                    wp_angle = 0;
                    aim_angle = 0;
                    pl_flip = SDL_FLIP_NONE;
                    wp_flip = SDL_FLIP_NONE;
                }
                break;
            case wp_yeet:
                if(player_direction == pl_left){
                    wp_angle = 180;
                    aim_angle = 180;
                    pl_flip = SDL_FLIP_HORIZONTAL;
                    wp_flip = SDL_FLIP_VERTICAL;
                }
                else if(player_direction == pl_right){
                    wp_angle = 0;
                    aim_angle = 0;
                    pl_flip = SDL_FLIP_NONE;
                    wp_flip = SDL_FLIP_NONE;
                }
                break;
            case wp_mrEdge:
                if(player_direction == pl_left){
                    wp_angle = 180;
                    aim_angle = 180;
                    pl_flip = SDL_FLIP_HORIZONTAL;
                    wp_flip = SDL_FLIP_VERTICAL;
                }
                else if(player_direction == pl_right){
                    wp_angle = 0;
                    aim_angle = 0;
                    pl_flip = SDL_FLIP_NONE;
                    wp_flip = SDL_FLIP_NONE;
                }
                break;
        }
}

void player::fire()
{
    //P_omega = P_weapons[current_wp].omega;
    //P_fire_count = P_weapons[current_wp].repeat_fire_count;
    if(attacking == 0 && reloading == 0){
        attacking = 1;
        reloading = 1;
        attack_timer = SDL_GetTicks();
        printf("[player::fire]\n");
        for(int i=0; i<MAX_REPEAT_FIRE; i++){
            fired[i] = 0;
        }
        switch (P_weapons[current_wp].wp_ID)
        {
            case generic_pistol:
				mSideWeapon.play();
                break;
            case generic_knife:
				mMeleeWeapon.play();
                break;
            case wp_sniper:
				mMainWeapon.play();
                break;

            case wp_assassin:
				mMainWeapon.play();
                if(player_direction == pl_left){
                    aim_angle = -180 + P_omega;
                }
                else if(player_direction == pl_right){
                    aim_angle = 0 - P_omega;
                }
                break;

            case wp_redCloud:
                if(player_direction == pl_left){
                    aim_angle = -180 + 5*P_omega;
                }
                else if(player_direction == pl_right){
                    aim_angle = 0 - 5*P_omega;
                }
                break;

            case wp_ta:
				mMainWeapon.play();
                break;

            case wp_yeet:
                break;

            case wp_mrEdge:
                if(player_direction == pl_left){
                    aim_angle = 180;
                }
                else if(player_direction == pl_right){
                    aim_angle = 0;
                }
                break;
        }
    }

}

void player::switchWeapon()
{
    switch_pending = 1;
}

void player::pl_wp_move(bool jump, bool squat, Uint8 dir)
{
    if(you_failed == 1){
        return;
    }
    if(jump == 1 && airborne == 0){
//        setVelocityY( getVelocityY() - JUMP_VELOCITY );
        setVelocityY( getVelocityY() - get_jump_v() );
        airborne = 1;
    }
    if(squat == 0){
        if(dir == dir_left){
            setVelocityX( -P_pl_speed );
        }
        else if(dir == dir_right){
            setVelocityX( P_pl_speed );
        }
        else if(dir == dir_none){
            setVelocityX( 0 );
        }
    }
    else if(squat){
        if(dir == dir_left){
            setVelocityX( -P_squat_speed );
        }
        else if(dir == dir_right){
            setVelocityX( P_squat_speed );
        }
        else if(dir == dir_none){
            setVelocityX( 0 );
        }
    }
}
//
void player::drop()
{
    if(you_failed == 1){
        return;
    }
    int floor_TileCoord[2] = { getCoordX() , getCoordY() + skin_clips[0].h + TILE_SIZE - 1 };
    // bottom left tile
	int floor_TileIndex[2];
    // find nearest floor tile's coord (bottom left)
	floor_TileIndex[1] = floor_TileCoord[1] / TILE_SIZE;
	floor_TileIndex[0] = floor_TileCoord[0] / TILE_SIZE;

    bool drop_thru = 1;
    while( floor_TileIndex[0] * TILE_SIZE < getCoordX() + skin_clips[0].w ){
        int drop_dist;
        int temp_floor_TileIndexY = floor_TileIndex[1];

        while(m_gameplaymap.get_mTileMap()[ temp_floor_TileIndexY ][ floor_TileIndex[0] ] == 0 && temp_floor_TileIndexY < m_gameplaymap.get_mMapHeight() - 1){
            temp_floor_TileIndexY ++;
        }
        //printf("tile type: %d", (m_gameplaymap.get_mTileMap()[ temp_floor_TileIndexY ][ floor_TileIndex[0] ]));
        drop_dist = temp_floor_TileIndexY * TILE_SIZE - (getCoordY() + skin_clips[0].h);
        if(drop_dist != 0 && drop_dist < skin_clips[0].h){
            drop_thru = 0;
        }
        else if( m_gameplaymap.get_mTileMap()[ (temp_floor_TileIndexY + 1 < m_gameplaymap.get_mMapHeight())? temp_floor_TileIndexY + 1 : m_gameplaymap.get_mMapHeight() - 1 ][ floor_TileIndex[0] ] != 0 && drop_dist < skin_clips[0].h){
            drop_thru = 0;
        }
        else{
            int drop_dist_2;
            int original_floor = floor_TileIndex[1];
            temp_floor_TileIndexY = (original_floor + 1 < m_gameplaymap.get_mMapHeight())? original_floor + 1 : original_floor;
            while(m_gameplaymap.get_mTileMap()[ temp_floor_TileIndexY ][ floor_TileIndex[0] ] == 0 && temp_floor_TileIndexY < m_gameplaymap.get_mMapHeight() - 1){
                temp_floor_TileIndexY ++;
            }
            drop_dist_2 = (temp_floor_TileIndexY - original_floor - 1) * TILE_SIZE;
            if(drop_dist_2 < skin_clips[0].h){
                drop_thru = 0;
            }
        }
        floor_TileIndex[0] ++;
    }
    if( drop_thru ){
        setCoordY( getCoordY() + 1 );
        dropping = 1;//
        airborne = 1;
        printf("Dropping\n");

        mDownDown.play();
    }
}

void player::render()
{
    SDL_Rect curClip;
    if(you_failed == 1 && death_start == 0){
        death_start = 1;
        clip_timer = SDL_GetTicks();
    }
    if(death_start){
        if(SDL_GetTicks() - clip_timer > DEATH_CLIP_NUM * 200){
            curClip = skin_clips[PLAYER_CLIP_NUM + DEATH_CLIP_NUM - 1];
        }
        else{
            curClip = skin_clips[ ( (SDL_GetTicks() - clip_timer) / 200) % DEATH_CLIP_NUM + PLAYER_CLIP_NUM ];
        }
        skinSprite.renderEx(getCoordX() - mSubscribedScene->mCamera.x,
                            getCoordY() - mSubscribedScene->mCamera.y,
                            &curClip, 0, NULL);
    }
    else{
        if(dropping){
            curClip = skin_clips[PLAYER_CLIP_NUM - 1];
            visible = 1;
            visible_timer = SDL_GetTicks();
        }
        else if(velocity[0] != 0){
            curClip = skin_clips[ ( ( (int)(SDL_GetTicks() - clip_timer) * abs((int)velocity[0]) ) / 700) % (PLAYER_CLIP_NUM - 2) + 1 ];
            visible = 1;
            visible_timer = SDL_GetTicks();
        }
        else{
            if(getID() == MrEdge){
                if(SDL_GetTicks() - visible_timer > visible_delay){
                    visible = 0;
                    curClip = skin_clips[0];
                }
                else{
                    curClip = skin_clips[PLAYER_CLIP_NUM - 1];
                }
            }
            else{
                curClip = skin_clips[0];
            }
            clip_timer = SDL_GetTicks();
        }
        // render non-active weapons behind player
        /*
        for(int i=0; i<3; i++){
            if(i != current_wp){
                wp_sprite[i].renderEx(getCoordX() - mSubscribedScene->mCamera.x,
                            getCoordY() - mSubscribedScene->mCamera.y ,
                            NULL, 0, NULL, wp_flip);
            }
        }
        */
        skinSprite.renderEx(getCoordX() - mSubscribedScene->mCamera.x,
                            getCoordY() - mSubscribedScene->mCamera.y,
                            &curClip, 0, NULL, pl_flip);

        SDL_Point rotationPoint_cur_wp;
        rotationPoint_cur_wp.x = P_original_clip.w / 2 - P_weapons[current_wp].wp_offset[0];
        rotationPoint_cur_wp.y = P_weapons[current_wp].wp_clip.h / 2;

        if(visible){
            wp_sprite[ current_wp ].renderEx(getCoordX() - mSubscribedScene->mCamera.x + P_weapons[current_wp].wp_offset[0],
                                            getCoordY() - mSubscribedScene->mCamera.y + P_weapons[current_wp].wp_offset[1],
                                            NULL, wp_angle, &rotationPoint_cur_wp, wp_flip);
        }

        for(int bIndex = 0; bIndex < MAX_BULLET_COUNT; bIndex++){
            if( proj_live[bIndex] ){
                proj_sprite[proj_list[bIndex]->weapon_index].renderEx(proj_list[bIndex]->getCoordX() - mSubscribedScene->mCamera.x,
                                                                        proj_list[bIndex]->getCoordY() - mSubscribedScene->mCamera.y,
                                                                        NULL, proj_list[bIndex]->proj_angle);
            }
        }
        // render special effects
        if(attacking){
            for(int i=0; i<P_weapons[current_wp].special_effect_clip_num; i++){
                    printf("%d\n",P_weapons[current_wp].special_effect_clip_num);
                if(SDL_GetTicks() - attack_timer < P_weapons[current_wp].down_swing_duration / P_weapons[current_wp].special_effect_clip_num * (i+1)){
                    if(player_direction == pl_left){
                        special_effect_sprite[current_wp].renderEx( getCoordX() - mSubscribedScene->mCamera.x + skin_clips[0].w - P_weapons[current_wp].SE_offset[0] - Special_effect_clips[current_wp][0].w,
                                                                    getCoordY() - mSubscribedScene->mCamera.y + P_weapons[current_wp].SE_offset[1],
                                                                    &Special_effect_clips[current_wp][i], 0, &rotationPoint_cur_wp, SDL_FLIP_HORIZONTAL);
                        break;
                    }
                    else if(player_direction == pl_right){
                        special_effect_sprite[current_wp].renderEx( getCoordX() - mSubscribedScene->mCamera.x + P_weapons[current_wp].SE_offset[0],
                                                                    getCoordY() - mSubscribedScene->mCamera.y + P_weapons[current_wp].SE_offset[1],
                                                                    &Special_effect_clips[current_wp][i], 0, &rotationPoint_cur_wp, SDL_FLIP_NONE);
                        break;
                    }
                }
            }
        }
    }
}

void player::updatePosition()
{
    if(cur_health <= 0){
        you_failed = 1;
    }
    // handle switching of weapon
    if(switch_pending && attacking == 0){
        printf("SWITCH!!!\n");
        current_wp = (current_wp + 1) % 3;
        switch_pending = 0;
        reset_wp_aim_angle();
        reloading = 0;
    }

    // handle projectile creation for multiple repeat rounds
    if(attacking == 1){
        P_omega = P_weapons[current_wp].omega;
        P_fire_count = P_weapons[current_wp].repeat_fire_count;
        P_fire_interval = P_weapons[current_wp].fire_interval;

        int proj_repeatNum = (P_fire_interval == -1)? 0 : (SDL_GetTicks() - attack_timer) / P_fire_interval ;
        proj_repeatNum = (proj_repeatNum >= P_fire_count)? P_fire_count-1 : proj_repeatNum;
        //printf("%d\n", proj_repeatNum);
        switch (P_weapons[current_wp].proj_type)
        {
            case proj_none:
                break;
            case proj_bullet:
                if(proj_repeatNum < P_fire_count){
                    if(fired[ proj_repeatNum ] == 0){
                        fired[ proj_repeatNum ] = 1;

                        if(player_direction == pl_left){
                            proj_list[ last_shot ] = new projectile(current_wp, P_weapons[current_wp].proj_type, getCoordX() + skin_clips[0].w - P_weapons[current_wp].proj_offset[0] - P_weapons[current_wp].proj_clip.w, getCoordY() + P_weapons[current_wp].proj_offset[1], P_weapons[current_wp].proj_speed * cos(aim_angle*M_PI/180), P_weapons[current_wp].proj_speed * sin(aim_angle*M_PI/180), aim_angle, P_weapons[current_wp].acceleration);
                        }
                        else if(player_direction == pl_right){
                            proj_list[ last_shot ] = new projectile(current_wp, P_weapons[current_wp].proj_type, getCoordX() + P_weapons[current_wp].proj_offset[0], getCoordY() + P_weapons[current_wp].proj_offset[1], P_weapons[current_wp].proj_speed * cos(aim_angle*M_PI/180), P_weapons[current_wp].proj_speed * sin(aim_angle*M_PI/180), aim_angle, P_weapons[current_wp].acceleration);
                        }
                        proj_live[ last_shot ] = 1;
                        last_shot = (last_shot + 1) % MAX_BULLET_COUNT;

                        if(getID() == Assassin || getID() == RedCloud || getID() == TA){
                            if(player_direction == pl_left){
                                aim_angle -= P_omega;
                            }
                            else if(player_direction == pl_right){
                                aim_angle += P_omega;
                            }
                        }
                    }
                }
                break;
            case proj_accelerated_bullet:
                if(proj_repeatNum < P_fire_count){
                    if(fired[ proj_repeatNum ] == 0){
                        fired[ proj_repeatNum ] = 1;

                        if(player_direction == pl_left){
                            proj_list[ last_shot ] = new projectile(current_wp, P_weapons[current_wp].proj_type, getCoordX() + skin_clips[0].w - P_weapons[current_wp].proj_offset[0] - P_weapons[current_wp].proj_clip.w, getCoordY() + P_weapons[current_wp].proj_offset[1], P_weapons[current_wp].proj_speed * cos(aim_angle*M_PI/180), P_weapons[current_wp].proj_speed * sin(aim_angle*M_PI/180), aim_angle, P_weapons[current_wp].acceleration);
                        }
                        else if(player_direction == pl_right){
                            proj_list[ last_shot ] = new projectile(current_wp, P_weapons[current_wp].proj_type, getCoordX() + P_weapons[current_wp].proj_offset[0], getCoordY() + P_weapons[current_wp].proj_offset[1], P_weapons[current_wp].proj_speed * cos(aim_angle*M_PI/180), P_weapons[current_wp].proj_speed * sin(aim_angle*M_PI/180), aim_angle, P_weapons[current_wp].acceleration);
                        }
                        proj_live[ last_shot ] = 1;
                        last_shot = (last_shot + 1) % MAX_BULLET_COUNT;

                        if(getID() == MrEdge){
                            if(player_direction == pl_left){
                                aim_angle -= P_omega;
                            }
                            else if(player_direction == pl_right){
                                aim_angle += P_omega;
                            }
                        }
                    }
                }
                break;
            case proj_throw_knife:
                if(proj_repeatNum < P_fire_count){
                    if(fired[ proj_repeatNum ] == 0){

                        srand(SDL_GetTicks() % 20);
                        aim_angle = rand() % 360;

                        fired[ proj_repeatNum ] = 1;

                        if(player_direction == pl_left){
                            proj_list[ last_shot ] = new projectile(current_wp, P_weapons[current_wp].proj_type, getCoordX() + skin_clips[0].w - P_weapons[current_wp].proj_offset[0] - P_weapons[current_wp].proj_clip.w, getCoordY() + P_weapons[current_wp].proj_offset[1], P_weapons[current_wp].proj_speed * cos(aim_angle*M_PI/180), P_weapons[current_wp].proj_speed * sin(aim_angle*M_PI/180), aim_angle, P_weapons[current_wp].acceleration);
                        }
                        else if(player_direction == pl_right){
                            proj_list[ last_shot ] = new projectile(current_wp, P_weapons[current_wp].proj_type, getCoordX() + P_weapons[current_wp].proj_offset[0], getCoordY() + P_weapons[current_wp].proj_offset[1], P_weapons[current_wp].proj_speed * cos(aim_angle*M_PI/180), P_weapons[current_wp].proj_speed * sin(aim_angle*M_PI/180), aim_angle, P_weapons[current_wp].acceleration);
                        }
                        proj_live[ last_shot ] = 1;
                        last_shot = (last_shot + 1) % MAX_BULLET_COUNT;
                    }
                }
                break;
        }
    }

    // handle reload and sword swing
    if(attacking || reloading){
        switch (P_weapons[current_wp].wp_ID)
        {
            case generic_pistol:
                attacking = 0;
                reloading = 0;
                break;
            case generic_knife:
                if(SDL_GetTicks() - attack_timer < P_weapons[current_wp].down_swing_duration){
                    //wp_angle += 130.0 * P_weapons[current_wp].down_swing_duration/1000 / 60;
                    if(player_direction == pl_left){
                        wp_angle -= 110.0 * 1000/P_weapons[current_wp].down_swing_duration / 60;
                    }
                    else if(player_direction == pl_right){
                        wp_angle += 110.0 * 1000/P_weapons[current_wp].down_swing_duration / 60;
                    }
                }
                else if(SDL_GetTicks() - attack_timer < P_weapons[current_wp].down_swing_duration + P_weapons[current_wp].up_swing_duration){
                    //wp_angle -= 130 * P_weapons[current_wp].up_swing_duration/1000 / 60;
                    if(player_direction == pl_left){
                        wp_angle += 110.0 * 1000/P_weapons[current_wp].up_swing_duration / 60;
                    }
                    else if(player_direction == pl_right){
                        wp_angle -= 110.0 * 1000/P_weapons[current_wp].up_swing_duration / 60;
                    }
                }
                else{
                    attacking = 0;
                }
                if(SDL_GetTicks() - attack_timer > P_weapons[current_wp].reload_time){
                    reloading = 0;
                }
                break;

            case wp_sniper:
                if(SDL_GetTicks() - attack_timer < P_weapons[current_wp].down_swing_duration){
                    //wp_angle += 130.0 * P_weapons[current_wp].down_swing_duration/1000 / 60;
                    if(player_direction == pl_left){
                        wp_angle += 15.0 * 1000/P_weapons[current_wp].down_swing_duration / 60;
                    }
                    else if(player_direction == pl_right){
                        wp_angle -= 15.0 * 1000/P_weapons[current_wp].down_swing_duration / 60;
                    }
                }
                else if(SDL_GetTicks() - attack_timer < P_weapons[current_wp].down_swing_duration + P_weapons[current_wp].up_swing_duration){
                    //wp_angle -= 130 * P_weapons[current_wp].up_swing_duration/1000 / 60;
                    if(player_direction == pl_left){
                        wp_angle -= 15.0 * 1000/P_weapons[current_wp].up_swing_duration / 60;
                    }
                    else if(player_direction == pl_right){
                        wp_angle += 15.0 * 1000/P_weapons[current_wp].up_swing_duration / 60;
                    }
                }
                else{
                    attacking = 0;
                }
                if(SDL_GetTicks() - attack_timer > P_weapons[current_wp].reload_time){
                    reloading = 0;
                }
                break;
            case wp_assassin:
                if(SDL_GetTicks() - attack_timer > P_fire_count * P_fire_interval){
                    attacking = 0;
                }
                if(SDL_GetTicks() - attack_timer > P_weapons[current_wp].reload_time && SDL_GetTicks() - attack_timer > P_fire_count * P_fire_interval){
                    reloading = 0;
                }
                break;
            case wp_redCloud:
                if(SDL_GetTicks() - attack_timer > P_fire_count * P_fire_interval){
                    attacking = 0;
                }
                if(SDL_GetTicks() - attack_timer > P_weapons[current_wp].reload_time && SDL_GetTicks() - attack_timer > P_fire_count * P_fire_interval){
                    reloading = 0;
                }
                break;
            case wp_ta:
                if( int(SDL_GetTicks() - attack_timer) > P_fire_count * P_fire_interval){
                    attacking = 0;
                }
                if( int(SDL_GetTicks() - attack_timer) > P_weapons[current_wp].reload_time && int(SDL_GetTicks() - attack_timer) > P_fire_count * P_fire_interval){
                    reloading = 0;
                }
                break;
            case wp_yeet:
                if( int(SDL_GetTicks() - attack_timer) > P_fire_count * P_fire_interval){
                    attacking = 0;
                }
                if( int(SDL_GetTicks() - attack_timer) > P_weapons[current_wp].reload_time && int(SDL_GetTicks() - attack_timer) > P_fire_count * P_fire_interval){
                    reloading = 0;
                }
                break;
            case wp_mrEdge:
                if( int(SDL_GetTicks() - attack_timer) > P_fire_count * P_fire_interval){
                    attacking = 0;
                }
                if( int(SDL_GetTicks() - attack_timer) > P_weapons[current_wp].reload_time && int(SDL_GetTicks() - attack_timer) > P_fire_count * P_fire_interval){
                    reloading = 0;
                }
                break;
        }
    }
    else{
        reset_wp_aim_angle();
    }
    for(int bIndex = 0; bIndex < MAX_BULLET_COUNT; bIndex++){
        if(proj_live[bIndex]){
            proj_list[bIndex]->proj_move();
            proj_list[bIndex]->baby_step();
        }
    }

    baby_step();
    if(airborne == 1){
        setVelocityY( getVelocityY() + GRAVITY );
    }
}

void player::checkCollision(player &opponent)
{
    // check collision when wielding a sword
    if(attacking && damage_dealt == 0){
        weapon& m_wp = P_weapons[current_wp];
        switch (m_wp.wp_ID)
        {
            case generic_pistol:

                break;
            case generic_knife:
                if(player_direction == pl_right){
                    if(( ( opponent.getCoordX() + opponent.P_c_box.x >= getCoordX() + P_original_clip.w/2 && opponent.getCoordX() + opponent.P_c_box.x <= getCoordX()+P_original_clip.w/2 + m_wp.range_of_swing[0] )  ||  ( opponent.getCoordX() + opponent.P_c_box.x + opponent.P_c_box.w >= getCoordX()+P_original_clip.w/2 && opponent.getCoordX() + opponent.P_c_box.x + opponent.P_c_box.w <= getCoordX()+P_original_clip.w/2 + m_wp.range_of_swing[0] ) )&& \
                       ( ( opponent.getCoordY() + opponent.P_c_box.y >= getCoordY() - (m_wp.range_of_swing[1]-P_original_clip.h) / 2  && opponent.getCoordY() + opponent.P_c_box.y <= getCoordY() + (m_wp.range_of_swing[1] + P_original_clip.h) / 2 )  ||  ( opponent.getCoordY() + opponent.P_c_box.y + opponent.P_original_clip.h >= getCoordY() - (m_wp.range_of_swing[1]-P_original_clip.h) / 2 && opponent.getCoordY() + opponent.P_c_box.y + opponent.P_original_clip.h <= getCoordY() + (m_wp.range_of_swing[1]+P_original_clip.h) / 2 ) ) )
                    {
                        opponent.damage(P_weapons[current_wp].damage);
                        mDamaged.play();
                        printf("Player health:%d\n", opponent.get_cur_health());
                        damage_dealt = 1;
                    }
                }
                else if(player_direction == pl_left){
                    if(( ( opponent.getCoordX() + opponent.P_c_box.x <= getCoordX() + P_original_clip.w/2 && opponent.getCoordX() + opponent.P_c_box.x >= getCoordX()+P_original_clip.w/2 - m_wp.range_of_swing[0] )  ||  ( opponent.getCoordX() + opponent.P_c_box.x + opponent.P_c_box.w <= getCoordX()+P_original_clip.w/2 && opponent.getCoordX() + opponent.P_c_box.x + opponent.P_c_box.w >= getCoordX()+P_original_clip.w/2 - m_wp.range_of_swing[0] ) )&& \
                       ( ( opponent.getCoordY() + opponent.P_c_box.y >= getCoordY() - (m_wp.range_of_swing[1]-P_original_clip.h) / 2  && opponent.getCoordY() + opponent.P_c_box.y <= getCoordY() + (m_wp.range_of_swing[1] + P_original_clip.h) / 2 )  ||  ( opponent.getCoordY() + opponent.P_c_box.y + opponent.P_original_clip.h >= getCoordY() - (m_wp.range_of_swing[1]-P_original_clip.h) / 2 && opponent.getCoordY() + opponent.P_c_box.y + opponent.P_original_clip.h <= getCoordY() + (m_wp.range_of_swing[1]+P_original_clip.h) / 2 ) ) )
                    {
                        opponent.damage(P_weapons[current_wp].damage);
                        mDamaged.play();
                        printf("Player health:%d\n", opponent.get_cur_health());
                        damage_dealt = 1;
                    }
                }
                break;
            case wp_sniper:

                break;
            case wp_redCloud:

                break;
            case wp_ta:

                break;
            case wp_yeet:

                break;
            case wp_mrEdge:

                break;
        }
    }
    else if(attacking == 0){
        damage_dealt = 0;
    }

    for(int bIndex = 0; bIndex < MAX_BULLET_COUNT; bIndex++){
		if ( proj_live[ bIndex ] ){
			///// start projectile collision detection ///////////////////////////////////////////////////////////////////
			//printf("%d ", bIndex);
			bool proj_collide = false;

			SDL_Rect proj_clip = P_weapons[current_wp].proj_clip;
			projectile *& mproj = proj_list[ bIndex ];

            if(ID != RedCloud){

                if ( mproj->getVelocityX() > 0)
                {
                    if ( m_gameplaymap.isColliding(mproj->getCoordX() + proj_clip.w, mproj->getCoordY()) ||
                         m_gameplaymap.isColliding(mproj->getCoordX() + proj_clip.w, mproj->getCoordY() + proj_clip.h) )
                    {
                        proj_collide = true;
                    }
                }
                else if ( mproj->getVelocityX() < 0)
                {
                    if ( m_gameplaymap.isColliding(mproj->getCoordX(), mproj->getCoordY()) ||
                         m_gameplaymap.isColliding(mproj->getCoordX(), mproj->getCoordY() + proj_clip.h) )
                    {
                        proj_collide = true;
                    }
                }
            }

			if(( ( mproj->getCoordX() >= opponent.getCoordX() && mproj->getCoordX() <= opponent.getCoordX() + opponent.skin_clips[0].w )  ||  ( mproj->getCoordX() + proj_clip.w >= opponent.getCoordX() && mproj->getCoordX() + proj_clip.w <= opponent.getCoordX() + opponent.skin_clips[0].w ) ) && \
               ( ( mproj->getCoordY() >= opponent.getCoordY() && mproj->getCoordY() <= opponent.getCoordY() + opponent.skin_clips[0].h )  ||  ( mproj->getCoordY() + proj_clip.h >= opponent.getCoordY() && mproj->getCoordY() + proj_clip.h <= opponent.getCoordY() + opponent.skin_clips[0].h ) ) )
            {
                opponent.damage(P_weapons[current_wp].damage);
				mDamaged.play();
                printf("Player health:%d\n", opponent.get_cur_health());
                proj_collide = true;
            }


			if( proj_collide ){
				delete proj_list[ bIndex ];
				proj_list[ bIndex ] = NULL;
				proj_live[ bIndex ] = 0;
			}
		}
	}

	int floor_TileCoord[2] = { getCoordX() + P_c_box.x                              , getCoordY() + P_c_box.y + P_c_box.h + TILE_SIZE - 1 };
	int right_TileCoord[2] = { getCoordX() + P_c_box.x + P_c_box.w + TILE_SIZE - 1  , getCoordY() + P_c_box.y + P_c_box.h - 1 };
	int left_TileCoord[2]  = { getCoordX() + P_c_box.x - TILE_SIZE + 1              , getCoordY() + P_c_box.y - 1 };
	int ceil_TileCoord[2]  = { getCoordX() + P_c_box.x                              , getCoordY() + P_c_box.y - TILE_SIZE};

	//  bottom left tile,   bottom right tile,  bottom left tile,  top left tile
    int floor_TileIndex[2], right_TileIndex[2], left_TileIndex[2], ceil_TileIndex[2];

	// distance in pixels
	int nearest_floor_dist = m_gameplaymap.get_mMapHeight() * TILE_SIZE - (getCoordY() + P_c_box.y + P_c_box.h) - TILE_SIZE;
	int nearest_ceil_dist = getCoordY() + P_c_box.y - TILE_SIZE;
	int nearest_right_dist = m_gameplaymap.get_mMapWidth() * TILE_SIZE - (getCoordX() + P_c_box.x + P_c_box.w);
	int nearest_left_dist = getCoordX() + P_c_box.x - TILE_SIZE;

	// find nearest floor tile's coord (bottom left)
	floor_TileIndex[1] = floor_TileCoord[1] / TILE_SIZE;
	floor_TileIndex[0] = floor_TileCoord[0] / TILE_SIZE;
	//printf("floor_tileIndex: %d,%d\n", floor_TileIndex[0], floor_TileIndex[1]);

	ceil_TileIndex[1] = ceil_TileCoord[1] / TILE_SIZE;
	ceil_TileIndex[0] = ceil_TileCoord[0] / TILE_SIZE;

	// find nearest right tile's coord (bottom right)
	right_TileIndex[1] = left_TileIndex[1] = right_TileCoord[1] / TILE_SIZE;
	right_TileIndex[0] = right_TileCoord[0] / TILE_SIZE;
	left_TileIndex[0] = left_TileCoord[0] / TILE_SIZE;

/////// check collision with floor /////////////////////////////////////////////////////////////////////

	// scan floor all tiles beneath the player and update the nearest tile
    while( floor_TileIndex[0] * TILE_SIZE < getCoordX() + P_c_box.x + P_c_box.w ){
        int drop_dist;
        int temp_floor_TileIndexY = floor_TileIndex[1];

        while(m_gameplaymap.get_mTileMap()[ temp_floor_TileIndexY ][ floor_TileIndex[0] ] == 0 && temp_floor_TileIndexY < m_gameplaymap.get_mMapHeight()){
            temp_floor_TileIndexY ++;
        }
        //printf("tile type: %d", (m_gameplaymap.get_mTileMap()[ temp_floor_TileIndexY ][ floor_TileIndex[0] ]));
        drop_dist = temp_floor_TileIndexY * TILE_SIZE - (getCoordY() + P_c_box.y + P_c_box.h);
        if(nearest_floor_dist > drop_dist ){
            nearest_floor_dist = drop_dist;
        }
        floor_TileIndex[0] ++;
    }
    if(nearest_floor_dist != 0){
        airborne = 1;
    }
    if( getVelocityY() > nearest_floor_dist){
        setCoordY( getCoordY() + nearest_floor_dist);
		setVelocityY(0);
		dropping = 0;
		airborne = 0;
    }
/////// end of check collision with floor /////////////////////////////////////////////////////////////////////



/////// check collision with ceiling /////////////////////////////////////////////////////////////////////

	// scan floor all tiles beneath the player and update the nearest tile
	int max_jump_dist = ( 2 * velocity[1] - GRAVITY * ((int)velocity[1] / (int)GRAVITY) ) * (int)ceil( velocity[1] / GRAVITY + 0.1) / 2 + 1;
	bool jump_thru = 1;
    while( ceil_TileIndex[0] * TILE_SIZE < getCoordX() + P_c_box.x + P_c_box.w ){
        int jump_dist;
        int temp_ceil_TileIndexY = ceil_TileIndex[1];

        while(m_gameplaymap.get_mTileMap()[ temp_ceil_TileIndexY ][ ceil_TileIndex[0] ] == 0 && temp_ceil_TileIndexY > 0){
            temp_ceil_TileIndexY --;
        }
        //printf("tile type: %d", (m_gameplaymap.get_mTileMap()[ temp_floor_TileIndexY ][ floor_TileIndex[0] ]));
        jump_dist = getCoordY() + P_c_box.y - (temp_ceil_TileIndexY + 1) * TILE_SIZE;
        if( max_jump_dist < jump_dist + TILE_SIZE + P_c_box.y + P_c_box.h && jump_dist < max_jump_dist){
            jump_thru = 0;
        }
        else if( m_gameplaymap.get_mTileMap()[ (temp_ceil_TileIndexY - 1 >= 0)? temp_ceil_TileIndexY - 1 : 0 ][ ceil_TileIndex[0] ] != 0 && jump_dist < max_jump_dist + TILE_SIZE){
            jump_thru = 0;
        }
        if(nearest_ceil_dist > jump_dist ){
            nearest_ceil_dist = jump_dist;
        }
        ceil_TileIndex[0] ++;
    }
    if( -getVelocityY() > nearest_ceil_dist && jump_thru == 0){
        setCoordY( getCoordY() - nearest_ceil_dist);
		setVelocityY(0);
    }
/////// end of check collision with ceiling /////////////////////////////////////////////////////////////////////

/////// check collision with right wall /////////////////////////////////////////////////////////////////////

	// scan floor all tiles on the right of the player and update the nearest tile
    while( right_TileIndex[1] * TILE_SIZE > getCoordY() + P_c_box.y - TILE_SIZE ){
        int right_dist;
        int temp_right_TileIndexX = right_TileIndex[0];

        while(m_gameplaymap.get_mTileMap()[ right_TileIndex[1] ][ temp_right_TileIndexX ] == 0 && temp_right_TileIndexX < m_gameplaymap.get_mMapWidth() + 1){
            temp_right_TileIndexX ++;
        }
        right_dist = temp_right_TileIndexX * TILE_SIZE - (getCoordX() + P_c_box.x + P_c_box.w);
        if(nearest_right_dist > right_dist && m_gameplaymap.get_mTileMap()[ right_TileIndex[1] ][ temp_right_TileIndexX ] != 0 ){
            nearest_right_dist = right_dist;
        }
        right_TileIndex[1] --;
    }
    if( getVelocityX() > nearest_right_dist){
        setCoordX( getCoordX() + nearest_right_dist - getVelocityX());
    }
/////// end of check collision with right wall /////////////////////////////////////////////////////////////////////

/////// check collision with left wall /////////////////////////////////////////////////////////////////////

	// scan floor all tiles on the left of the player and update the nearest tile
    while( left_TileIndex[1] * TILE_SIZE > getCoordY() + P_c_box.y - TILE_SIZE ){
        int left_dist;
        int temp_left_TileIndexX = left_TileIndex[0];

        while(m_gameplaymap.get_mTileMap()[ left_TileIndex[1] ][ temp_left_TileIndexX ] == 0 && temp_left_TileIndexX > 1){
            temp_left_TileIndexX --;
        }
        left_dist = getCoordX() + P_c_box.x - (temp_left_TileIndexX + 1) * TILE_SIZE;

        if(nearest_left_dist > left_dist && m_gameplaymap.get_mTileMap()[ left_TileIndex[1] ][ temp_left_TileIndexX ] != 0 ){
            nearest_left_dist = left_dist;
        }
        left_TileIndex[1] --;

    }
    if( -getVelocityX() > nearest_left_dist){
        setCoordX( getCoordX() - nearest_left_dist - getVelocityX());
    }
/////// end of check collision with right wall /////////////////////////////////////////////////////////////////////
}
