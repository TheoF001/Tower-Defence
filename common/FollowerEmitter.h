#ifndef FOLLOWEREMITTER_H
#define FOLLOWEREMITTER_H
#include "IntParticleEmitter.h"
#include "projectile.h"

class Projectile;
class FollowerEmitter : public IntParticleEmitter {
    public:
        FollowerEmitter(Drawable* _model, int number, glm::vec3 pos, Projectile* energy_ball);
        //FountainEmitter(Drawable* _model, int number);
        Projectile* leader;
        char type;
        int dead_particles = 0;
        int active_particles = 0; //number of particles that have been instantiated
        void createNewParticle(int index) override;
        //void updateParticles(float time, float dt, glm::vec3 camera_pos = glm::vec3(0, 0, 0)) override;
        void updateParticles(float time, float dt, glm::vec3 camera_pos, glm::vec3 new_emitter_pos) override;
};


#endif //VVR_OGL_LABORATORY_FOUNTAINEMITTER_H
