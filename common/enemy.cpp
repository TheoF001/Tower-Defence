#include "enemy.h"
#include <glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <glm/gtx/string_cast.hpp>


using namespace glm;

//Enemy::Enemy(mat4& vP, Drawable* model, Drawable* armor, char type, mat4& translation, mat4& rotation) :enemyModel(model), armorModel(armor) {
Enemy::Enemy(mat4& vP, Drawable* model, Drawable* armor, char type, vec3 pos, mat4& rotation, Drawable* particle) :enemyModel(model), armorModel(armor) {
    f_emitter = new FountainEmitter(particle, 10, pos +vec3(0,1,0), the_type);
    cooldown = 0;
    enemyVP = vP;
    enemyRotation = rotation;
    x = pos;
    /*std::cout << x.x << std::endl;
    std::cout << x.y << std::endl;
    std::cout << x.z << std::endl;*/
    enemyTranslation = translate(mat4(), x);
    v = vec3(0, 0, 0.01);
    range = 7.0f;
    //v = vec3(0, 0, 0.01);
    //P = m * v;
    the_type = type;
    if (type == 'o') {
        //range = 5.0f;
        hp = 100;
        mov_speed = 1.0f/1.5f;
        v = vec3(0, 0, mov_speed);
        enemyScaling = scale(mat4(), vec3(0.7f, 0.7f, 0.7f));
        hp_pos = translate(mat4(), vec3(0, 1.5f, 0));
        damage = 50;//50;
        /*v = vec3(0, 0, 1);
        P = m * v;*/
        //hitbox = vec3(1.44f/2.0f, 2.15f/2.0f, 0.56f/2.0f)*0.7f;
        hitbox = vec3(0.1f / 2.0f, 0.7f, 0.4f / 2.0f) * 0.7f;

        //skeleton
        /*hand_left.parent = &elbow_left;
        hand_left.pos = vec3(0.64f, 0.17f, 0.1f);
        hand_left.t = translate(mat4(), vec3(0.18f, -0.08f, 0.1f));
        hand_left.children = false;

        hand_right.parent = &elbow_right;
        hand_right.pos = vec3(-0.64f, 0.17f, 0.1f);
        hand_right.t = translate(mat4(), vec3(-0.18f, -0.08f, 0.1f));
        hand_right.children = false;

        elbow_left.parent = &shoulder_left;
        elbow_left.children_nodes.push_back(&hand_left);
        elbow_left.pos = vec3(0.46f, 0.25f, 0);
        elbow_left.t = translate(mat4(), vec3(0.19f, -0.35f, 0.0f));

        elbow_right.parent = &shoulder_right;
        elbow_right.children_nodes.push_back(&hand_right);
        elbow_right.pos = vec3(-0.46f, 0.25f, 0);
        elbow_right.t = translate(mat4(), vec3(-0.19f, -0.35f, 0.0f));

        shoulder_left.parent = &neck;
        shoulder_left.children_nodes.push_back(&elbow_left);
        shoulder_left.pos = vec3(0.27f, 0, 0);
        shoulder_left.t = translate(mat4(), vec3(0.27f, 0, 0));
        
        shoulder_right.parent = &neck;
        shoulder_right.children_nodes.push_back(&elbow_right);
        shoulder_right.pos = vec3(-0.27f, 0, 0);
        shoulder_right.t = translate(mat4(), vec3(-0.27f, 0, 0));

        neck.parent = &key_bone;
        neck.children_nodes.push_back(&shoulder_left);
        neck.children_nodes.push_back(&shoulder_right);
        neck.children_nodes.push_back(&head);
        neck.pos = vec3(0, 0.6f, 0);
        neck.t = translate(mat4(), vec3(0, 0.6f, 0));

        key_bone.pos = vec3(0, 0, 0);
        key_bone.children_nodes.push_back(&waist_left);
        key_bone.children_nodes.push_back(&waist_right);
        key_bone.children_nodes.push_back(&neck);
        key_bone.root = true;

        head.parent = &neck;
        head.pos = vec3(0, 0.86f, 0);
        head.t = translate(mat4(), vec3(0, 0.86f - 0.6f, 0));
        head.children = false;

        waist_left.parent = &key_bone;
        waist_left.children_nodes.push_back(&knee_left);
        waist_left.pos= vec3(0.165f, 0, 0);
        waist_left.t = translate(mat4(), vec3(0.165f, 0, 0));

        waist_right.parent = &key_bone;
        waist_right.children_nodes.push_back(&knee_right);
        waist_right.pos = vec3(-0.165f, 0, 0);
        waist_right.t = translate(mat4(), vec3(-0.165f, 0, 0));

        knee_left.parent = &waist_left;
        knee_left.children_nodes.push_back(&foot_left);
        knee_left.pos = vec3(0.165f, -0.59f, 0);
        knee_left.t = translate(mat4(), vec3(0, -0.59f, 0));

        knee_right.parent = &waist_right;
        knee_right.children_nodes.push_back(&foot_right);
        knee_right.pos = vec3(-0.165f, -0.59f, 0);
        knee_right.t = translate(mat4(), vec3(0, -0.59f, 0));

        foot_left.parent = &knee_left;
        foot_left.pos = vec3(0.21f, -1.144f, 0);
        foot_left.t = translate(mat4(), vec3(0.045f, -0.554f, 0));
        foot_left.children = false;

        foot_right.parent = &knee_left;
        foot_right.pos = vec3(-0.21f, -1.144f, 0);
        foot_right.t = translate(mat4(), vec3(-0.045f, -0.554f, 0));
        foot_right.children = false;*/

    }
    else if (type == 'a') {
        hp = 50;
        mov_speed = 30;
        enemyScaling = scale(mat4(), vec3(0.5f, 0.5f, 0.5f));
    }
    enemyM = enemyTranslation * enemyScaling * enemyRotation;
    enemyMVP = enemyVP * enemyM;
    current_hp = hp;


}
//void Enemy::update() {
//
void Enemy::update(float dt) {
    
    move(dt);
    //std::cout << x.z << std::endl;
    //f_emitter->updateParticles(dt, dt, camera_pos);
    //advanceState(t, dt);
    enemyTranslation = translate(mat4(), x);
    enemyM = enemyTranslation * enemyScaling * enemyRotation;
    enemyMVP = enemyVP * enemyM;
   
}

void Enemy::move(float dt) {
    if (attacking == false || x.z < target.z - mov_speed) {
        if (x.z > -12.0f && x.z < 12.0) {
        
            //P = vec3(0, 0, 0);
            if (x.x > 0){ // old map
            //if (x.x > 0 && x.x < 10) {
                v = cross(x, vec3(0, 0.05f, 0));
                //v = vec3(mov_speed, 0, 0);
            }
            else if (x.x < 0){//old map
            //else if (x.x < 0 && x.x > -10) {
                v = cross(x, vec3(0, -0.05f, 0));
                //v = vec3(-mov_speed, 0, 0);
            }
            
            else {
                v = vec3(0, 0, mov_speed);
            }
        }
        else if (x.z > 11.9f && x.z < 26.6f && abs(x.x) < 1.4) {
            
            v = vec3(0, 0, mov_speed);
        }
        else if (x.z > 26.6f) {
            v = vec3(0, 0, 0);
        }
        else if (x.z < -12.0f) {
            v = vec3(0, 0, mov_speed);
        }
    }
    else if (x.z > target.z - mov_speed) {// old map
    //else{

        if (length(x - target) < range) {
            v = vec3(0, 0, 0);
            //std::cout << cooldown << std::endl;
            cooldown += dt;
        }
    }
    x += v*dt;
}