//#ifndef CHARACTER_H
//#define CHARACTER_H
//
//#include <SDL.h>
//#include "weapon.h"
//
//
//#define CHARACTER_COUNT 6
//
//class character;
//extern character *cha_List[ CHARACTER_COUNT ];
//
//enum characterIDs
//{
//    Sniper = 0,
//    Assassin,
//    Grenader,
//    Gandalf,
//    RedCloud,
//    MrEdge
//};
//
//class character
//{
//	public:
//        //character(int ID, const char* path, int offset, double pl_v, double pl_sv, double jump_v, int HP, double reload_t, double proj_v);
//        character(int ID, const char* path, int c_boxX, int c_boxY, double pl_v, double pl_sv, double jump_v, int HP);
//        const int chaID;
//        const char* skin_path;
//        const char* wp_skin_path[3];
//        const char* proj_skin_path[3];
//        const char* SE_path[3];
//        weapon cha_wps[3];
//
//        //const int proj_pos_offsetY;
//        SDL_Rect c_box;
//        SDL_Rect original_clip;
//        SDL_Rect death_clip;
//
//        const double pl_speed;
//        const double pl_squat_speed;
//        const double pl_jump_velocity;
//        const int full_health;
//};
//
//
//#endif // CHARACTER_H
#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL.h>
#include "weapon.h"


#define CHARACTER_COUNT 6

class character;
extern character *cha_List[ CHARACTER_COUNT ];

enum characterIDs
{
    Sniper = 0,
    Assassin,
    RedCloud,
    TA,
    Yeet,
    MrEdge
};

class character
{
	public:
        //character(int ID, const char* path, int offset, double pl_v, double pl_sv, double jump_v, int HP, double reload_t, double proj_v);
        character(int ID, const char* path, int c_boxX, int c_boxY, double pl_v, double pl_sv, double jump_v, int HP);
        const int chaID;
        const char* skin_path;
        const char* wp_skin_path[3];
        const char* proj_skin_path[3];
        const char* SE_path[3];
        weapon cha_wps[3];

        //const int proj_pos_offsetY;
        SDL_Rect c_box;
        SDL_Rect original_clip;
        SDL_Rect death_clip;

        const double pl_speed;
        const double pl_squat_speed;
        const double pl_jump_velocity;
        const int full_health;
};


#endif // CHARACTER_H
