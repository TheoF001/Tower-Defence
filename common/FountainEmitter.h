#ifndef VVR_OGL_LABORATORY_FOUNTAINEMITTER_H
#define VVR_OGL_LABORATORY_FOUNTAINEMITTER_H
#include "IntParticleEmitter.h"

class FountainEmitter : public IntParticleEmitter {
    public:
        FountainEmitter(Drawable* _model, int number, glm::vec3 pos, char type);
        //FountainEmitter(Drawable* _model, int number);
        char type;
        int dead_particles = 0;
        int active_particles = 0; //number of particles that have been instantiated
        void createNewParticle(int index) override;
        //void updateParticles(float time, float dt, glm::vec3 camera_pos = glm::vec3(0, 0, 0)) override;
        void updateParticles(float time, float dt, glm::vec3 camera_pos, glm::vec3 new_emitter_pos) override;
};


#endif //VVR_OGL_LABORATORY_FOUNTAINEMITTER_H
