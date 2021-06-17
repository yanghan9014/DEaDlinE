//#include "character.h"
//
//character::character(int ID, const char* path,
//					int c_boxX, int c_boxY, double pl_v, double pl_sv, double jump_v, int HP)
//: chaID(ID), skin_path(path), pl_speed(pl_v), pl_squat_speed(pl_sv), pl_jump_velocity(jump_v), full_health(HP)
//{
//    original_clip.x = original_clip.y = 0;
//    original_clip.w = 60;
//    original_clip.h = 60;
//
//    c_box.w = c_boxX;
//    c_box.h = c_boxY;
//    c_box.x = ( original_clip.w - c_box.w ) / 2;
//    c_box.y = ( original_clip.h - c_box.h ) / 2;
//
//    death_clip.x = death_clip.y = 0;
//    death_clip.w = 60;
//    death_clip.h = 60;
//
//    cha_wps[0].set_wp_ID(chaID);
//    wp_skin_path[0] = cha_wps[0].skin_path;
//    proj_skin_path[0] = cha_wps[0].proj_path;
//    SE_path[0] = cha_wps[0].special_effect_path;
//    for(int i=1; i<3; i++){
//        cha_wps[i].set_wp_ID(i - 3);
//        wp_skin_path[i] = cha_wps[i].skin_path;
//        proj_skin_path[i] = cha_wps[i].proj_path;
//        SE_path[i] = cha_wps[i].special_effect_path;
//    }
//}
#include "character.h"

character::character(int ID, const char* path,
					int c_boxX, int c_boxY, double pl_v, double pl_sv, double jump_v, int HP)
: chaID(ID), skin_path(path), pl_speed(pl_v), pl_squat_speed(pl_sv), pl_jump_velocity(jump_v), full_health(HP)
{
    original_clip.x = original_clip.y = 0;
    original_clip.w = 60;
    original_clip.h = 60;

    c_box.w = c_boxX;
    c_box.h = c_boxY;
    c_box.x = ( original_clip.w - c_box.w ) / 2;
    c_box.y = ( original_clip.h - c_box.h ) / 2;

    death_clip.x = death_clip.y = 0;
    death_clip.w = 60;
    death_clip.h = 60;

    cha_wps[0].set_wp_ID(chaID);
    wp_skin_path[0] = cha_wps[0].skin_path;
    proj_skin_path[0] = cha_wps[0].proj_path;
    SE_path[0] = cha_wps[0].special_effect_path;

    for(int i=1; i<3; i++){
        cha_wps[i].set_wp_ID(i - 3);
        wp_skin_path[i] = cha_wps[i].skin_path;
        proj_skin_path[i] = cha_wps[i].proj_path;
        SE_path[i] = cha_wps[i].special_effect_path;
    }
}
