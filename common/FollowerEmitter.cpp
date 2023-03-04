#include "FollowerEmitter.h"
#include <iostream>
#include <algorithm>


FollowerEmitter::FollowerEmitter(Drawable *_model, int number, glm::vec3 pos, Projectile* energy_ball) : IntParticleEmitter(_model, number, pos) {
    leader = energy_ball;
}
//FountainEmitter::FountainEmitter(Drawable* _model, int number) : IntParticleEmitter(_model, number) {}
//void FountainEmitter::updateParticles(float time, float dt, glm::vec3 camera_pos) {
void FollowerEmitter::updateParticles(float time, float dt, glm::vec3 camera_pos, glm::vec3 new_emitter_pos) {

    //This is for the fountain to slowly increase the number of its particles to the max amount
    //instead of shooting all the particles at once
    emitter_pos = leader->pos;
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
        

        particle.life -= dt*10;
        //std::cout << particle.life << std::endl;
        if (particle.life < 0.0f) {           
            createNewParticle(i);
            
        }
        particle.position += particle.velocity * dt;
    }
}

void FollowerEmitter::createNewParticle(int index){
    particleAttributes & particle = p_attributes[index];

    particle.position = emitter_pos + glm::vec3(0.1 - RAND * 0.2, 0, 0.1 - RAND * 0.2);
    particle.velocity = leader->vel * 0.2f;
     
    particle.mass = RAND*0.02 + 0.03f;
    particle.rot_axis = glm::normalize(glm::vec3(1 - 2*RAND, 1 - 2*RAND, 1 - 2*RAND));
    particle.rot_angle = RAND*360;
    particle.life = 1.0f; //mark it alive
}
