//#include <cmath>
//#include "projectile_mechanics.h"
//
//
//projectile::projectile(int wp_index, int p_type, int x, int y, double vx, double vy, double angle, double a)
//    : weapon_index(wp_index), proj_type(p_type), acceleration(a)
//{
//    setCoordX(x);
//    setCoordY(y);
//    setVelocityX(0);
//    setVelocityY(0);
//    proj_angle = angle;
//    switch(proj_type)
//    {
//        case proj_none:
//            break;
//        case proj_bullet:
//            setVelocityX(vx);
//            setVelocityY(vy);
//            break;
//        case proj_accelerated_bullet:
//            setVelocityX(vx);
//            setVelocityY(vy);
//            break;
//        case proj_grenade:
//            launch_timer = SDL_GetTicks();
//
//            break;
//        case proj_throw_knife:
//            proj_angle += 30;
//            break;
//        case proj_laser:
//            break;
//        case proj_boomerang:
//            break;
///*optional XD*/case proj_redCloud:
//            break;
//    }
//}
//
//void projectile::proj_move()
//{
//    // called by "UpdatePosition" function in class "game"
//    // update position based on velocity
//    // we will check collision later in class "game"
//
//    // int coord[2] : x and y coordinate
//    // double velocity[2] : x and y velocity
//    // gravity is already defined (#define gravity 3.0)
//    switch(proj_type)
//    {
//        case proj_none:
//            break;
//        case proj_bullet:
//            break;
//        case proj_accelerated_bullet:
//            setVelocityX( getVelocityX() + acceleration * cos(proj_angle*M_PI/180));
//            setVelocityY( getVelocityY() + acceleration * sin(proj_angle*M_PI/180));
//            break;
//        case proj_grenade:
//            setVelocityY( getVelocityY() + GRAVITY );
//            break;
//        case proj_throw_knife:
//            proj_angle += 30;
//            break;
//        case proj_laser:
//            break;
//        case proj_boomerang:
//            break;
///*optional XD*/case proj_redCloud:
//            break;
//    }
//}
#include <cmath>
#include "projectile_mechanics.h"


projectile::projectile(int wp_index, int p_type, int x, int y, double vx, double vy, double angle, double a)
    : weapon_index(wp_index), proj_type(p_type), acceleration(a)
{
    setCoordX(x);
    setCoordY(y);
    setVelocityX(0);
    setVelocityY(0);
    proj_angle = angle;
    switch(proj_type)
    {
        case proj_none:
            break;
        case proj_bullet:
            setVelocityX(vx);
            setVelocityY(vy);
            break;
        case proj_accelerated_bullet:
            setVelocityX(vx);
            setVelocityY(vy);
            break;
        case proj_grenade:
            launch_timer = SDL_GetTicks();

            break;
        case proj_throw_knife:
            proj_angle += 1;
            break;
        case proj_laser:
            break;
        case proj_boomerang:
            break;
/*optional XD*/case proj_redCloud:
            break;
    }
}

void projectile::proj_move()
{
    // called by "UpdatePosition" function in class "game"
    // update position based on velocity
    // we will check collision later in class "game"

    // int coord[2] : x and y coordinate
    // double velocity[2] : x and y velocity
    // gravity is already defined (#define gravity 3.0)
    switch(proj_type)
    {
        case proj_none:
            break;
        case proj_bullet:
            break;
        case proj_accelerated_bullet:
            setVelocityX( getVelocityX() + acceleration * cos(proj_angle*M_PI/180));
            setVelocityY( getVelocityY() + acceleration * sin(proj_angle*M_PI/180));
            break;
        case proj_decelerated_bullet:
            setVelocityX( getVelocityX() + acceleration * cos(proj_angle*M_PI/180));
            setVelocityY( getVelocityY() + acceleration * sin(proj_angle*M_PI/180));
            break;
        case proj_grenade:
            setVelocityY( getVelocityY() + GRAVITY );
            break;
        case proj_throw_knife:
            srand(SDL_GetTicks() % 20);
            proj_angle += rand() % 10;
            break;
        case proj_laser:
            break;
        case proj_boomerang:
            break;
/*optional XD*/case proj_redCloud:
            break;
    }
}
