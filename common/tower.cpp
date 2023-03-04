#include "tower.h"
#include <glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <glm/gtx/string_cast.hpp>

using namespace glm;

//Tower::Tower(mat4& vP, Drawable* model, char type, mat4& translation, mat4& rotation):towerModel(model){
//Tower::Tower(mat4& vP, Drawable* model, char type, vec3 pos, mat4& rotation) :towerModel(model) {
Tower::Tower(mat4& vP, Drawable* model, char type, vec3 pos, mat4& rotation, Drawable* particle) :towerModel(model) {
        
        towerVP = vP;
        towerRotation = rotation;
        x = pos;
        towerTranslation = translate(mat4(), pos);
        cooldown = 0;
        the_type = type;
        mana_cd = 0;
        if (type == 'm') {
            hp = 1000;//1000;
            range = 7;
            towerScaling = scale(mat4(), vec3(0.7f, 0.7f, 0.7f));
            towerM = towerTranslation * towerScaling * towerRotation;
            //hp_pos = translate(mat4(), vec3(0, 6.6f, 0));
            hp_pos = translate(mat4(), vec3(0, 7.0f, 0));
            mana_pos = translate(mat4(), vec3(0, 6.0f, 0));
            damage = 40;
            vec3 particle_height = vec3(0, 2.8f, 0);
            hitbox = vec3(2.53f/2.0f, 6.31f, 2.47f/2.0f)*0.7f;
            f_emitter = new FountainEmitter(particle, 200, pos + particle_height, the_type);
            
        }
        else if (type == 'a') {
            hp = 800;
            range = 6;
            towerScaling = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));
            towerM = towerTranslation * towerScaling * towerRotation;
            //hp_pos = translate(mat4(), vec3(0, 3.6f, 0));
            hp_pos = translate(mat4(), vec3(0, 4.0f, 0));
            mana_pos = translate(mat4(), vec3(0, 3.0f, 0));
            damage = 20;
            hitbox = vec3(2.53f / 2.0f, 6.31f, 2.47f / 2.0f) * 0.7f;
            vec3 particle_height = vec3(0, 1.8f, 0);
            f_emitter = new FountainEmitter(particle, 200, pos + particle_height, the_type);
        }
        current_hp = hp;
        towerMVP = towerVP * towerM;
    

}
void Tower::update(float dt) {
    if (mana_cd<mana_max) {
        mana_cd += dt;
    }
    
    if (attacking) {
        cooldown += dt;
    }
}
//void Tower::update() {
//        towerMVP = towerVP * towerM;
//}