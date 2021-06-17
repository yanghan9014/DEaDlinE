//#ifndef PROJECTILE_MECHANICS_H
//#define PROJECTILE_MECHANICS_H
//
//#include <SDL.h>
//
//#include "entity.h"
//
//
//enum Projectile_Type
//{
//	proj_none = -1,
//    proj_bullet = 0,
//    proj_accelerated_bullet,
//    proj_grenade,
//    proj_throw_knife,
//    proj_laser,
//    proj_boomerang,
//    proj_redCloud
//};
//
//class projectile : public entity
//{
//    public:
//        projectile(int wp_index, int p_type, int x, int y, double vx, double vy, double angle, double a);
//        //void launch();
//        int weapon_index;
//        Uint32 launch_timer;
//
//        double proj_angle;
//        void proj_move();
//    private:
//        int proj_type;
//        double acceleration;
//};
//
//
//#endif // PROJECTILE_MECHANICS_H
#ifndef PROJECTILE_MECHANICS_H
#define PROJECTILE_MECHANICS_H

#include <SDL.h>

#include "entity.h"


enum Projectile_Type
{
	proj_none = -1,
    proj_bullet = 0,
    proj_accelerated_bullet,
    proj_decelerated_bullet,
    proj_grenade,
    proj_throw_knife,
    proj_laser,
    proj_boomerang,
    proj_redCloud
};

class projectile : public entity
{
    public:
        projectile(int wp_index, int p_type, int x, int y, double vx, double vy, double angle, double a);
        //void launch();
        int weapon_index;
        Uint32 launch_timer;

        double proj_angle;
        void proj_move();
    private:
        int proj_type;
        double acceleration;
};


#endif // PROJECTILE_MECHANICS_H
