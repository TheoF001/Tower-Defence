#include "FountainEmitter.h"
#include <iostream>
#include <algorithm>

FountainEmitter::FountainEmitter(Drawable *_model, int number, glm::vec3 pos, char type) : IntParticleEmitter(_model, number, pos) {
    this->type = type;
}
//FountainEmitter::FountainEmitter(Drawable* _model, int number) : IntParticleEmitter(_model, number) {}
//void FountainEmitter::updateParticles(float time, float dt, glm::vec3 camera_pos) {
void FountainEmitter::updateParticles(float time, float dt, glm::vec3 camera_pos, glm::vec3 new_emitter_pos) {

    //This is for the fountain to slowly increase the number of its particles to the max amount
    //instead of shooting all the particles at once

    if (active_particles < number_of_particles) {
        int batch = 14;
        int limit = std::min(number_of_particles - active_particles, batch);
        for (int i = 0; i < limit; i++) {
            createNewParticle(active_particles);
            active_particles++;
        }
    }
    else {
        active_particles = number_of_particles; //In case we resized our ermitter to a smaller particle number
    }

    for(int i = 0; i < active_particles; i++){
        particleAttributes & particle = p_attributes[i];
        

       // if(particle.position.y < emitter_pos.y - 10.0f || particle.life == 0.0f){
        if (particle.position.y < -1.0f && particle.life == 1.0f) {
            //std::cout << particle.position.y << std::endl;
            particle.life = 0.0f;
            dead_particles++;
            //std::cout << active_particles << std::endl;
            //createNewParticle(i);
            
            
        }
        else if(dead_particles!=number_of_particles){
            particle.rot_angle += 90 * dt;
            glm::vec3 emitter_pos_diff = new_emitter_pos - emitter_pos;
            if (new_emitter_pos != emitter_pos) {
                emitter_pos = new_emitter_pos;

            }
            particle.position = particle.position + particle.velocity * dt + particle.accel * (dt * dt) * 0.5f + emitter_pos_diff;
            particle.velocity = particle.velocity + particle.accel * dt;


            //auto bill_rot = calculateBillboardRotationMatrix(particle.position, camera_pos);
            //particle.rot_axis = glm::vec3(bill_rot.x, bill_rot.y, bill_rot.z);
            //particle.rot_angle = glm::degrees(bill_rot.w);
            particle.dist_from_camera = length(particle.position - camera_pos);
        }


    }
}

void FountainEmitter::createNewParticle(int index){
    particleAttributes & particle = p_attributes[index];

    //particle.position = emitter_pos + glm::vec3(1 - RAND*2, RAND, 1 - RAND*2);
    if (type == 'm' || type == 'a') {
        particle.position = emitter_pos + glm::vec3(1 - RAND * 2, 0, 1 - RAND * 2);//for mage tower
    }
    else {
        particle.position = emitter_pos + glm::vec3(0.25 - RAND * 0.5, 0, 0.25 - RAND * 0.5);//for orcs
    }
    //particle.velocity = glm::vec3( 3 - RAND*6,0,3 - RAND*6);
    glm::vec3 direction = (particle.position - emitter_pos) / glm::length(particle.position - emitter_pos);
    //particle.velocity = glm::vec3(direction.x * (RAND * 3), 0, direction.z * ( RAND * 3));
    particle.velocity = glm::vec3(direction.x * (RAND * 1), 0, direction.z * (RAND * 1));
     
    particle.mass = RAND + 0.5f;
    particle.rot_axis = glm::normalize(glm::vec3(1 - 2*RAND, 1 - 2*RAND, 1 - 2*RAND));
    particle.accel = glm::vec3(0.0f, -9.8f, 0.0f); //gravity force
    particle.rot_angle = RAND*360;
    particle.life = 1.0f; //mark it alive
}
