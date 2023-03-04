#ifndef TOWER_HPP
#define TOWER_HPP

#include <glm/glm.hpp>
#include <iostream>
#include "model.h"
#include "enemy.h"
#include "projectile.h"

//class Enemy;
using namespace glm;
class Tower
{
public:
    FountainEmitter* f_emitter;
    Enemy* target_id;
    Drawable* towerModel;
    Projectile* projectile;
    Projectile* meteor;
    mat4 towerRotation;
    mat4 towerTranslation;
    mat4 towerScaling;
    mat4 towerVP;
    mat4 towerMVP;
    mat4 towerM;
    mat4 hp_pos;
    mat4 mana_pos;
    vec3 x;
    vec3 hitbox;
    float cooldown;
    float mana_cd;
    float mana_max = 6;
    //int id;
    char the_type;
    float hp;
    float current_hp;
    int damage;
    float range;
    bool attacking = false;
    bool hit = false;
    bool power_attack = false;
    bool shoot = false;
    bool particle_effect = false;
    //char towertype;//m==mage, a==archer, +more
    //Tower(mat4& vP, Drawable* model, char type, mat4& translation, mat4& rotation);
    //Tower(mat4& vP, Drawable* model, char type, vec3 pos, mat4& rotation);
    Tower::Tower(mat4& vP, Drawable* model, char type, vec3 pos, mat4& rotation, Drawable* particle);
    //void update();
    void update(float dt);


};



#endif