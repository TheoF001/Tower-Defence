#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include <glm/glm.hpp>
#include <iostream>
#include "model.h"
#include "FollowerEmitter.h"
//#include "ExplosionEmitter.h"

class FollowerEmitter;
class ExplosionEmitter;

using namespace glm;
class Projectile
{
public:
    FollowerEmitter* trail;
    ExplosionEmitter* explosion;
    Drawable* proj_model;
    mat4 projectileRotation;
    mat4 projectileTranslation;
    mat4 projectileScaling;
    mat4 projectileVP;
    mat4 projectileMVP;
    mat4 projectileM;
    vec3 pos;
    vec3 target_pos;
    vec3 vel;
    vec3 accel;
    char the_type;
    bool exploding = false;
    bool aoe = false;
    Projectile(mat4& vP, Drawable* model,  vec3 proj_pos, vec3 target, char type);
    void update(float dt = 0);


};



#endif