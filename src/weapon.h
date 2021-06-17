//#ifndef WEAPON_H
//#define WEAPON_H
//
//#include <SDL.h>
//#include "projectile_mechanics.h"
//
//
//enum wp_IDs
//{
//    generic_pistol = -2,
//    generic_knife = -1,
//    wp_sniper = 0,
//    wp_assassin,
//    wp_grenader,
//    wp_littleLee,
//    wp_gandalf,
//    wp_mrEdge,
//    wp_redCloud,
//};
//
//class weapon
//{
//    public:
//        friend class player;
//        friend class character;
//        weapon();
//        void set_wp_ID(int ID);
//        int get_wp_ID() { return wp_ID; }
//
//        int wp_offset[2];
//        int proj_offset[2];
//        int SE_offset[2];
//        const char* skin_path;
//        const char* proj_path;
//        const char* special_effect_path;
//        SDL_Rect wp_clip;
//        SDL_Rect proj_clip;
//        int proj_type;
//        bool wp_attack();
//        int range_of_swing[2];
//        // swing duration
//        int down_swing_duration;
//        int up_swing_duration;
//
//        int repeat_fire_count;
//        int fire_interval;
//        double omega;
//
//    private:
//        int special_effect_clip_num;
//        SDL_Rect SE_clip;
//        SDL_Rect *special_effect_clip;
//        double proj_speed;
//        double acceleration;
//        int wp_ID;
//};
//#endif // WEAPON_H
//
#ifndef WEAPON_H
#define WEAPON_H

#include <SDL.h>
#include "projectile_mechanics.h"

enum wp_IDs
{
    generic_pistol = -2,
    generic_knife = -1,
    wp_sniper = 0,
    wp_assassin,
    wp_redCloud,
    wp_ta,
    wp_yeet,
    wp_mrEdge,
};

class weapon
{
    public:
        friend class player;
        friend class character;
        weapon();
        void set_wp_ID(int ID);
        int get_wp_ID() { return wp_ID; }

        int wp_offset[2];
        int proj_offset[2];
        int SE_offset[2];
        const char* skin_path;
        const char* proj_path;
        const char* special_effect_path;
        SDL_Rect wp_clip;
        SDL_Rect proj_clip;
        int proj_type;
        bool wp_attack();
        int range_of_swing[2];
        // swing duration
        int down_swing_duration;
        int up_swing_duration;
        int reload_time;
        int damage;

        int repeat_fire_count;
        int fire_interval;
        double omega;

    private:
        int special_effect_clip_num;
        SDL_Rect SE_clip;
        SDL_Rect *special_effect_clip;
        double proj_speed;
        double acceleration;
        int wp_ID;
};
#endif // WEAPON_H

