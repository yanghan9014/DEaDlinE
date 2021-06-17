#include "weapon.h"

weapon::weapon()
{
    skin_path = NULL;
    proj_path = NULL;
    special_effect_path = NULL;
    range_of_swing[0] = range_of_swing[1] = 0;
    special_effect_clip_num = 0;
    down_swing_duration = 0;
    up_swing_duration = 0;
    reload_time = 0;
    acceleration = 0;

    repeat_fire_count = 1;
    fire_interval = -1;

    omega = 0;
    damage = 2;
}
void weapon::set_wp_ID(int ID)
{
    wp_ID = ID;
    switch (wp_ID)
    {
        case generic_pistol:
            skin_path = "skin_examples/pistol_small.png";
            proj_path = "skin_examples/pistol_bullet_small.png";
            proj_type = proj_bullet;
            wp_offset[0] = 45;
            wp_offset[1] = 28;
            proj_offset[0] = 30;
            proj_offset[1] = 20;
            wp_clip.x = wp_clip.y = 0;
            wp_clip.w = 20;
            wp_clip.h = 16;

            proj_clip.x = proj_clip.y = 0;
            proj_clip.w = 10;
            proj_clip.h = 10;
            proj_speed = 15;
            acceleration = 0;
            damage = 2;
            break;
        case generic_knife:
            skin_path = "skin_examples/knife_small.png";
            proj_path = "skin_examples/knife_small.png";
            special_effect_path = "skin_examples/sword slash.png";
            special_effect_clip_num = 6;

            proj_type = proj_none;
            wp_offset[0] = 45;
            wp_offset[1] = -3;
            SE_offset[0] = 25;
            SE_offset[1] = -5;
            wp_clip.x = wp_clip.y = 0;
            wp_clip.w = 40;
            wp_clip.h = 40;
            proj_speed = 0;
            acceleration = 0;

            SE_clip.x = SE_clip.y = 0;
            SE_clip.w = 70;
            SE_clip.h = 100;

            range_of_swing[0] = 60;
            range_of_swing[1] = 80;
            down_swing_duration = 100;
            up_swing_duration = 120;
            damage = 7;
            break;
        case wp_sniper:
            skin_path = "skin_examples/sniper rifle_small.png";
            proj_path = "skin_examples/sniper_bullet_small.png";
            special_effect_path = "skin_examples/muzzle flash_small.png";
            special_effect_clip_num = 1;

            proj_type = proj_accelerated_bullet;
            wp_offset[0] = 40;
            wp_offset[1] = 25;
            proj_offset[0] = 20;
            proj_offset[1] = 20;
            SE_offset[0] = 60;
            SE_offset[1] = 10;
            wp_clip.x = wp_clip.y = 0;
            wp_clip.w = 40;
            wp_clip.h = 12;

            proj_clip.x = proj_clip.y = 0;
            proj_clip.w = 40;
            proj_clip.h = 15;
            proj_speed = 7;

            SE_clip.x = SE_clip.y = 0;
            SE_clip.w = 45;
            SE_clip.h = 30;

            acceleration = 1.0;
            down_swing_duration = 20;
            up_swing_duration = 150;
            reload_time = 1500;
            damage = 30;
            break;
        case wp_assassin:
            skin_path = "skin_examples/assassin_wp_small.png";
            proj_path = "skin_examples/assassin_proj_small.png";
            special_effect_path = "skin_examples/assassin_se.png";
            special_effect_clip_num = 4;
            repeat_fire_count = 3;
            fire_interval = 10;
            omega = 7;

            proj_type = proj_bullet;
            wp_offset[0] = 37;
            wp_offset[1] = 27;
            proj_offset[0] = 5;
            proj_offset[1] = 20;
            SE_offset[0] = 40;
            SE_offset[1] = 10;
            wp_clip.x = wp_clip.y = 0;
            wp_clip.w = 23;
            wp_clip.h = 22;

            proj_clip.x = proj_clip.y = 0;
            proj_clip.w = 40;
            proj_clip.h = 15;
            proj_speed = 40;

            SE_clip.x = SE_clip.y = 0;
            SE_clip.w = 30;
            SE_clip.h = 30;

            down_swing_duration = 3000;
            up_swing_duration = 400;
            reload_time = 400;
            damage = 5;
            break;

        case wp_redCloud:
            //skin_path = "skin_examples/assassin_wp_small.png";
            proj_path = "skin_examples/red_cloud_small.png";
            //special_effect_path = "skin_examples/muzzle flash_small.png";
            //special_effect_clip_num = 1;
            repeat_fire_count = 10;
            fire_interval = 120;
            omega = 3.5;

            proj_type = proj_bullet;
            wp_offset[0] = 37;
            wp_offset[1] = 27;
            proj_offset[0] = 5;
            proj_offset[1] = 20;
            SE_offset[0] = 60;
            SE_offset[1] = 10;
            wp_clip.x = wp_clip.y = 0;
            wp_clip.w = 23;
            wp_clip.h = 22;

            proj_clip.x = proj_clip.y = 0;
            proj_clip.w = 40;
            proj_clip.h = 15;
            proj_speed = 20;

            SE_clip.x = SE_clip.y = 0;
            SE_clip.w = 45;
            SE_clip.h = 30;

            down_swing_duration = 20;
            up_swing_duration = 150;
            reload_time = 1500;
            damage = 4;
            break;

        case wp_ta:
            skin_path = "skin_examples/laser_gun_small.png";
            proj_path = "skin_examples/laser beam.png";
            //special_effect_path = "skin_examples/muzzle flash_small.png";
            //special_effect_clip_num = 1;
            repeat_fire_count = 3;
            fire_interval = 120;
            omega = 0;

            proj_type = proj_bullet;
            wp_offset[0] = 28;
            wp_offset[1] = 17;
            proj_offset[0] = 5;
            proj_offset[1] = 20;
            SE_offset[0] = 60;
            SE_offset[1] = 10;
            wp_clip.x = wp_clip.y = 0;
            wp_clip.w = 23;
            wp_clip.h = 22;

            proj_clip.x = proj_clip.y = 0;
            proj_clip.w = 40;
            proj_clip.h = 15;
            proj_speed = 10;

            SE_clip.x = SE_clip.y = 0;
            SE_clip.w = 45;
            SE_clip.h = 30;

            down_swing_duration = 20;
            up_swing_duration = 150;
            reload_time = 600;
            damage = 8;
            break;

        case wp_yeet:
            //skin_path = "skin_examples/laser_gun_small.png";
            proj_path = "skin_examples/hsieh_small.png";
            //special_effect_path = "skin_examples/muzzle flash_small.png";
            //special_effect_clip_num = 1;
            repeat_fire_count = 30;
            fire_interval = 200;
            omega = 7;

            proj_type = proj_throw_knife;
            wp_offset[0] = 37;
            wp_offset[1] = 20;
            proj_offset[0] = 5;
            proj_offset[1] = 20;
            SE_offset[0] = 60;
            SE_offset[1] = 10;
            wp_clip.x = wp_clip.y = 0;
            wp_clip.w = 23;
            wp_clip.h = 22;

            proj_clip.x = proj_clip.y = 0;
            proj_clip.w = 40;
            proj_clip.h = 15;
            proj_speed = 3;

            SE_clip.x = SE_clip.y = 0;
            SE_clip.w = 45;
            SE_clip.h = 30;

            down_swing_duration = 20;
            up_swing_duration = 150;
            reload_time = 3000;
            damage = 2;
            break;

        case wp_mrEdge:
            //skin_path = "skin_examples/sniper rifle_small.png";
            proj_path = "skin_examples/awkward.png";
            //special_effect_clip_num = 1;
            repeat_fire_count = 6;
            fire_interval = 20;
            omega = 60;

            proj_type = proj_accelerated_bullet;
            wp_offset[0] = 30;
            wp_offset[1] = 10;
            proj_offset[0] = 30;
            proj_offset[1] = 0;
            //SE_offset[0] = 60;
            //SE_offset[1] = 10;
            wp_clip.x = wp_clip.y = 0;
            wp_clip.w = 40;
            wp_clip.h = 12;

            proj_clip.x = proj_clip.y = 0;
            proj_clip.w = 32;
            proj_clip.h = 59;
            proj_speed = 8;

            SE_clip.x = SE_clip.y = 0;
            SE_clip.w = 45;
            SE_clip.h = 30;

            acceleration = -0.3;
            down_swing_duration = 20;
            up_swing_duration = 150;
            reload_time = 300;
            damage = 5;
            break;
    }
    special_effect_clip = new SDL_Rect [special_effect_clip_num];
    for(int i=0; i<special_effect_clip_num; i++){
        special_effect_clip[i].x = SE_clip.w * i;
        special_effect_clip[i].y = 0;
        special_effect_clip[i].w = SE_clip.w;
        special_effect_clip[i].h = SE_clip.h;
    }
}
/*
bool weapon::wp_attack()
{
    switch (wp_ID)
    {
        case generic_pistol:

            break;
        case generic_knife:

            break;
        case wp_sniper:

            break;
        case wp_grenader:

            break;
        case wp_gandalf:

            break;
    }
}
*/
