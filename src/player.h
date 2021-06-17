//#ifndef PLAYER_H
//#define PLAYER_H
//
//#include <vector>
//#include <SDL.h>
//
//#include "projectile_mechanics.h"
//#include "scene_object.h"
//#include "gameplay_map.h"
//#include "weapon.h"//
//#include "l_mixerchunk.h"
//
//#define PLAYER_CLIP_NUM 6
//#define DEATH_CLIP_NUM 11
//
//#define MAX_BULLET_COUNT 10
//#define MAX_REPEAT_FIRE 100//
//
//extern SDL_Joystick* gamePad[2];
//
//#define GAMEPAD_BUTTON_COUNT 12
//#define KEYBOARD_BUTTON_COUNT 6
//#define JOYSTICK_DEADZONE 1000
//
//enum gamepadButtons
//{
//    Button1 = 0,
//    Button2,
//    Button3,
//    Button4,
//    LeftTrigger1,
//    RightTrigger1,
//    LeftTrigger2,
//    RightTrigger2,
//    Button_Select,
//    Button_Start,
//    LeftAxisClick,
//    RightAxisClick
//};
//enum keyBoardButtons
//{
//    key_up = 0,
//    key_left,
//    key_down,
//    key_right,
//    key_fire,
//    key_switch
//};
//enum direction
//{
//	dir_none = 0,
//	dir_left,
//	dir_right
//};
//enum joystickAxis
//{
//    leftX = 0,
//    leftY = 1,
//    rightX = 3,
//    rightY = 4
//};
//enum pl_dir
//{
//    pl_left = 0,
//    pl_right
//};
//
//class player : public entity, public SceneObject
//{
//	public:
//        player(GameplayMap& gameplaymap, Scene* scene, Uint8 pl_index, Uint8 characterID);
//        void reset();//
//
//        int getID() { return ID; }//
//        bool you_failed;
//        bool death_start;
//
//        int get_full_health() { return P_full_health; }
//        int get_cur_health() { return cur_health; }
//        void set_cur_health(int hp) { cur_health = hp; }
//        void damage(int damagePt) { cur_health -= damagePt; }
//
//		int get_pl_dir() { return player_direction; }
//        void set_pl_dir(int p_dir) { player_direction = p_dir; }
//        double get_wp_angle() { return wp_angle; }
//        void set_wp_angle(double wpAngle) { wp_angle = wpAngle; }
//        double get_aim_angle() { return aim_angle; }
//        void set_aim_angle(double a_angle) { aim_angle = a_angle; }
//
//        double get_jump_v() { return P_jump_velocity; }
//
//        weapon get_cur_wp() { return P_weapons[current_wp]; }
//        void reset_wp_aim_angle();//
//
//        void fire();
//        bool fired[MAX_REPEAT_FIRE];//
//        int P_fire_count;//
//        int P_fire_interval;//
//        double P_omega;//
//
//        int current_wp;
//        void switchWeapon();
//        //int switch_pending;
//        bool switch_pending;
//        int attacking;
//        bool damage_dealt;//
//        Uint32 attack_timer;
//
//        void pl_wp_move(bool jump, bool squat, Uint8 dir);
//        void drop();
//        int dropping;//
//
//        bool airborne;
//        bool visible;//
//        Uint32 visible_timer;
//        int visible_delay;
//
//        Uint8 gamePadbuttonState[ GAMEPAD_BUTTON_COUNT ];
//        Uint8 keyBoardState[ KEYBOARD_BUTTON_COUNT ];
//
//        SDL_RendererFlip pl_flip;
//        SDL_RendererFlip wp_flip;
//
//        //std::vector<projectile> projs;
//        projectile *proj_list[ MAX_BULLET_COUNT ];
//        int first_shot;
//        int last_shot;
//        void destroy_proj(int bIndex)
//        {
//            delete proj_list[ bIndex ];
//            proj_list[ bIndex ] = NULL;
//            proj_live[ bIndex ] = 0;
//        }
//
//        void render() override;
//        void updatePosition();
//        void checkCollision(player &opponent);
//        SDL_Rect P_c_box;//
//        SDL_Rect P_original_clip;//
//        SDL_Rect P_death_clip;
//
//    private:
//        bool proj_live[MAX_BULLET_COUNT];
//
//        Uint32 clip_timer;
//        GameplayMap& m_gameplaymap;
//        int cur_health;
//        int player_direction;
//        double wp_angle;
//        double aim_angle;
//        weapon P_weapons[3];
//
//        const int mWhich;//
//        const int  ID;
//        const double P_pl_speed;
//        const double P_squat_speed;
//        const double P_jump_velocity;
//        const int P_full_health;
//
//        SDL_Rect skin_clips[ PLAYER_CLIP_NUM + DEATH_CLIP_NUM ];
//        LTexture& skinSprite;
//        LTexture wp_sprite[3];
//        LTexture proj_sprite[3];
//        LTexture special_effect_sprite[3];
//        SDL_Rect range_of_swing[3][2];
//        SDL_Rect *Special_effect_clips[3];
//
//        LMixerChunk mMainWeapon;//
//        LMixerChunk mSideWeapon;//
//        LMixerChunk mMeleeWeapon;//
//};
//
//
//#endif // PLAYER_H
#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <SDL.h>

#include "projectile_mechanics.h"
#include "scene_object.h"
#include "gameplay_map.h"//TODO: pass in a pointer instead of an obj/ref to enable forward decl?
#include "weapon.h"//
#include "l_mixerchunk.h"

#define PLAYER_CLIP_NUM 6
#define DEATH_CLIP_NUM 11

#define MAX_BULLET_COUNT 100
#define MAX_REPEAT_FIRE 100//

extern SDL_Joystick* gamePad[2];

#define GAMEPAD_BUTTON_COUNT 12
#define KEYBOARD_BUTTON_COUNT 6
#define JOYSTICK_DEADZONE 1000

enum gamepadButtons
{
    Button1 = 0,
    Button2,
    Button3,
    Button4,
    LeftTrigger1,
    RightTrigger1,
    LeftTrigger2,
    RightTrigger2,
    Button_Select,
    Button_Start,
    LeftAxisClick,
    RightAxisClick
};
enum keyBoardButtons
{
    key_up = 0,
    key_left,
    key_down,
    key_right,
    key_fire,
    key_switch
};
enum direction
{
	dir_none = 0,
	dir_left,
	dir_right
};
enum joystickAxis
{
    leftX = 0,
    leftY = 1,
    rightX = 3,
    rightY = 4
};
enum pl_dir
{
    pl_left = 0,
    pl_right
};

class player : public entity, public SceneObject
{
	public:
        player(GameplayMap& gameplaymap, Scene* scene, Uint8 pl_index, Uint8 characterID);
        void reset();//

        int getID() { return ID; }//
        bool you_failed;
        bool death_start;

        int get_full_health() { return P_full_health; }
        int get_cur_health() { return cur_health; }
        void set_cur_health(int hp) { cur_health = hp; }
        void damage(int damagePt) { cur_health -= damagePt; }

		int get_pl_dir() { return player_direction; }
        void set_pl_dir(int p_dir) { player_direction = p_dir; }
        double get_wp_angle() { return wp_angle; }
        void set_wp_angle(double wpAngle) { wp_angle = wpAngle; }
        double get_aim_angle() { return aim_angle; }
        void set_aim_angle(double a_angle) { aim_angle = a_angle; }

        double get_jump_v() { return P_jump_velocity; }

        weapon get_cur_wp() { return P_weapons[current_wp]; }
        void reset_wp_aim_angle();//

        void fire();
        bool fired[MAX_REPEAT_FIRE];//
        int P_fire_count;//
        int P_fire_interval;//
        double P_omega;//

        int current_wp;
        void switchWeapon();
        //int switch_pending;
        bool switch_pending;
        bool attacking;
        bool reloading;
        bool damage_dealt;//
        Uint32 attack_timer;

        void pl_wp_move(bool jump, bool squat, Uint8 dir);
        void drop();
        int dropping;//

        bool airborne;
        bool visible;//
        Uint32 visible_timer;
        int visible_delay;

        Uint8 gamePadbuttonState[ GAMEPAD_BUTTON_COUNT ];
        Uint8 keyBoardState[ KEYBOARD_BUTTON_COUNT ];

        SDL_RendererFlip pl_flip;
        SDL_RendererFlip wp_flip;

        //std::vector<projectile> projs;
        projectile *proj_list[ MAX_BULLET_COUNT ];
        int first_shot;
        int last_shot;
        void destroy_proj(int bIndex)
        {
            if(proj_live[ bIndex ] != 0){
                delete proj_list[ bIndex ];
                proj_list[ bIndex ] = NULL;
                proj_live[ bIndex ] = 0;
            }
        }

        void render() override;
        void updatePosition();
        void checkCollision(player &opponent);
        SDL_Rect P_c_box;//
        SDL_Rect P_original_clip;//
        SDL_Rect P_death_clip;

    private:
        bool proj_live[MAX_BULLET_COUNT];

        Uint32 clip_timer;
        GameplayMap& m_gameplaymap;
        int cur_health;
        int player_direction;
        double wp_angle;
        double aim_angle;
        weapon P_weapons[3];

        const int mWhich;//
        const int  ID;
        const double P_pl_speed;
        const double P_squat_speed;
        const double P_jump_velocity;
        const int P_full_health;

        SDL_Rect skin_clips[ PLAYER_CLIP_NUM + DEATH_CLIP_NUM ];
        LTexture& skinSprite;
        LTexture wp_sprite[3];
        LTexture proj_sprite[3];
        LTexture special_effect_sprite[3];
        SDL_Rect range_of_swing[3][2];
        SDL_Rect *Special_effect_clips[3];

        LMixerChunk mMainWeapon;
        LMixerChunk mSideWeapon;
        LMixerChunk mMeleeWeapon;
        LMixerChunk mDownDown;
        LMixerChunk mDamaged;
};


#endif // PLAYER_H
