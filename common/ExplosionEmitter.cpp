#include "ExplosionEmitter.h"
#include <iostream>
#include <algorithm>


ExplosionEmitter::ExplosionEmitter(Drawable *_model, int number, glm::vec3 pos) : IntParticleEmitter(_model, number, pos) {}

void ExplosionEmitter::updateParticles(float time, float dt, glm::vec3 camera_pos, glm::vec3 new_emitter_pos) {

    //This is for the fountain to slowly increase the number of its particles to the max amount
    //instead of shooting all the particles at once
    
    if (active_particles < number_of_particles) {
        int batch = 1500;//500;//50;
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
        

        //particle.life -= dt/3;
        particle.life = (1-particle.dist/3);
        //std::cout << particle.life << std::endl;
        if (particle.life < 0.0f) {           
            //createNewParticle(i);
            dead_particles++;
            
        }
        
        //if (particle.life > 0.0f) {
            particle.position += particle.velocity * dt; //+ particle.accel * dt *dt *0.5f;
            if (particle.dist > 0.3) {
                particle.position += particle.velocity * dt + particle.accel * dt * dt * 0.5f;
            }
            if (particle.mass>0.01f) {
                particle.mass -= 0.0005f;
            }
            //particle.velocity += particle.accel*dt;
            /*if (particle.position.y > 0.5f) {
                particle.accel = particle.accel = -9.8f * (particle.position - emitter_pos);
                particle.accel.y = 9.8;
            }*/
            particle.dist = length(particle.position - emitter_pos);
       //}
        
    }
}

void ExplosionEmitter::createNewParticle(int index){
    particleAttributes & particle = p_attributes[index];

    particle.position = emitter_pos + glm::vec3(0.1 - RAND * 0.2, RAND*0.3, 0.1 - RAND * 0.2)*0.1f;//*0.001
    particle.dist = length(particle.position - emitter_pos);
    glm::vec3 direction = normalize(particle.position- emitter_pos);
    particle.velocity = direction*2.0f;
    particle.accel = glm::vec3(2- RAND * 4, 2 - RAND * 4, 2 - RAND * 4);
    //particle.accel.y = 9.8f;

    //particle.mass = RAND*0.02 + 0.03f;
    particle.mass = RAND * 0.002 + 0.07f;
    
    particle.life = 1.0f; //mark it alive
}
