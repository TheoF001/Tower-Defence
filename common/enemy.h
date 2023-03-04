#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "model.h"
//#include "RigidBody.h"

#include "FountainEmitter.h"
//#include "tower.h"
using namespace glm;

class Tower;
class Projectile;
class Enemy
{
public:
    Projectile* projectile;
    FountainEmitter* f_emitter;
    Drawable* enemyModel;
    Drawable* armorModel;
    mat4 enemyRotation;
    mat4 enemyTranslation;
    mat4 enemyScaling;
    mat4 enemyVP;
    mat4 enemyMVP;
    mat4 enemyM;
    mat4 hp_pos;
    vec3 x;//position in map
    vec3 v;
    char the_type;
    //vec3 omega;
    float cooldown;
    std::vector <Tower*> hit_by;
    vec3 target;
    Tower* target_id;
    float range;
    float current_hp;
    float hp;
    int damage;
    float mov_speed;
    char enemytype;//o==orc, g==goblin, +more
    bool attacking = false;
    bool hit = false;
    bool first_hit = true;
    bool particle_effect = false;
    bool shoot = false;
    bool meteored = false;
    vec3 hitbox;//dimensions of the hitbox measured from the center
    //Enemy(mat4& vP, Drawable* model, Drawable* armor, char type, mat4& translation, mat4& rotation);
    Enemy(mat4& vP, Drawable* model, Drawable* armor, char type, vec3 pos, mat4& rotation, Drawable* particle);
    //void update();
    //void update(float t = 0, float dt = 0);
    void update(float dt = 0);
    void move(float dt = 0);


};

#endif