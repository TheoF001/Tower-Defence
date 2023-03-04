#include "projectile.h"
#include <glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <glm/gtx/string_cast.hpp>

using namespace glm;

Projectile::Projectile(mat4& vP, Drawable* model, vec3 proj_pos, vec3 target, char type):proj_model(model){
    //arrow projectile look towards (0,1,0)
    //arrow center at (0,0.352,0)
    pos = proj_pos;//+vec3(0,1,0);
    projectileVP = vP;
    target_pos = target;
    the_type = type;
    //projectileRotation = rotation;
    vec3 direction = target_pos - pos;
    direction = normalize(direction);
    projectileRotation = rotate(mat4(), dot(vec3(0, 1, 0), direction), cross(vec3(0, 1, 0), direction));
    //projectileRotation = rotate(mat4(), dot(vec3(0,1,0), direction), cross(vec3(0, 1, 0), direction));
    projectileTranslation = translate(mat4(), pos);
    accel = vec3(0, -9.8, 0) / 2.0f;
    if (the_type == 'm') {
        //projectileScaling = scale(mat4(), vec3(2, 1.25, 2));
        //vel = direction * 40.0f;
        projectileScaling = scale(mat4(), vec3(1, 1, 1));
        vel = direction * 20.0f;
        accel = vec3(0, 0, 0);
    }
    else if(the_type == 'a'){
        projectileScaling = scale(mat4(), vec3(1, 1, 1));
        vel = direction * 10.0f;
    }
    else if (the_type == 'o') {
        projectileScaling = scale(mat4(), vec3(1, 1, 1)/6.0f);
        vel = direction * 9.0f;
        
    }
    else if (the_type == 'b') {
        projectileScaling = scale(mat4(), vec3(1, 1, 1) / 3.0f);
        vel = vec3(0,-1,0) * 20.0f;
        accel = vec3(0, 0, 0);
    }
    
    
    //accel = vec3(0, 0, 0);
    projectileM = projectileTranslation * projectileRotation* projectileScaling;
    //projectileMVP = projectileVP * projectileM;
    

}
void Projectile::update(float dt) {
    
    pos = pos + vel * dt + accel * (dt * dt) * 0.5f;
    vel = vel + accel * dt;
    vec3 direction = target_pos - pos;
    direction = normalize(direction);
    projectileTranslation = translate(mat4(), pos);
    if (the_type != 'b') {
        projectileRotation = rotate(mat4(), -dot(direction, vec3(0, 1, 0)), cross(direction, vec3(0, 1, 0)));
        
        projectileM = projectileTranslation * projectileRotation * projectileScaling;
    }
    else {
        
        projectileM = projectileTranslation * projectileScaling;
    }
   
    
   
}