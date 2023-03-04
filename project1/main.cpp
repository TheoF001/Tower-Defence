#include <iostream>
#include <string>
#include <vector>//for glm::vector
#include <glm/glm.hpp>//---------
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>//----------------
#include <glm/gtc/type_ptr.hpp>//for gtm::value_ptr

// Include GLEW (always include first)
#include <GL/glew.h>

// Include GLFW to handle window and keyboard events
#include <glfw3.h>

// Shader loading utilities and other
#include <common/shader.h>
#include <common/util.h>
#include <common/camera.h>
#include <common/model.h>
#include <common/texture.h>
#include <common/light.h>
#include <common/tower.h>
#include <common/enemy.h>
#include <common/stb_image.h>
#include <common/SimplexNoise.h>
#include <common/FountainEmitter.h>
#include <common/ExplosionEmitter.h>
#include <common/tree.h>

//imgui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define SHADOW_WIDTH 10240//8192//20480//10240//4096//1024
#define SHADOW_HEIGHT 10240//8192//20480//10240//4096//1024

using namespace std;
using namespace glm;

// Function prototypes
void renderDepthMap();
void renderHelpingWindow();
void initialize();
void createContext();
void mainLoop();
void free();
void pollKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
void attack_handler(vector <Enemy*>& orcs, vector <Tower*>& mage);
void particle_effects(mat4  V, mat4 P, float currentTime, float dt, vector <Enemy*>& orcs, vector <Tower*>& mage);
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);

#define W_WIDTH 1024
#define W_HEIGHT 768
#define TITLE "Tower defence"
#define OFFSET 22//grid rows offset in vertices matrix

// Global variables
GLFWwindow* window;
Camera* camera;
Drawable* plane;
Drawable* magetowermodel;
Drawable* smithmodel;
Drawable* orcmodel;
Drawable* armor;
Drawable* quad;
Drawable* archertowermodel;
Drawable* arrow;//projectiles
GLuint arrowDiffuse, arrowSpecular, boulderDiffuse, energyDiffuse;//projectile texture
GLuint attack_range;
GLuint shaderProgram, depthProgram, miniMapProgram;//Shader Programms
GLuint gridVAO;//Vertex Array Object
GLuint myVBO;//Vertex Buffer Object
GLuint colorVBO;
GLuint MVPLocation;//MVP Locations on shaders
GLuint magetexture, orctexture, armortexture, archertexture;//textures
GLuint modeltextureSampler, ambientSampler, specularSampler;//textureSamplers
GLuint mageAOtexture, mageNormals, orcAOtexture, orcNormals, armorAOtexture, armorNormals, archerAOtexture, archerSpecular;
GLuint archerNormals, normalSampler;
GLuint comettexture;

//light and shadowing
Light* light;
GLuint LaLocation, LdLocation, LsLocation;
GLuint lightPositionLocation;
GLuint lightPowerLocation;
GLuint depthMapSampler;
GLuint lightVPLocation;
GLuint depthFrameBuffer, depthTexture;

GLuint shadowViewProjectionLocation, shadowModelLocation;
GLuint modelMatrixLocation;
//GLuint diffuseColorSampler;
GLuint specularColorSampler;
GLuint ambientColorSampler;
//materials
GLuint KaLocation, KdLocation, KsLocation, NsLocation, projectionMatrixLocation, viewMatrixLocation;
//minimap
GLuint quadTextureSamplerLocation;
//plane
GLuint heightmap, grasstexture, pathtexture;
GLuint draw_plane;
GLuint grasstextureSampler, pathtextureSampler, heightmapSampler, useTextureLocation;
// load height map texture
int hm_width, hm_height, hm_nChannels;
unsigned char* planedata;
vector<float> vertices;
//vector<vec3> vertices;
vector<unsigned int> indices;
GLuint terrainVAO, terrainVBO, terrainEBO, terrainNormalsVBO;
GLuint hpLocation, hpMatrixLocation;
unsigned int NUM_STRIPS ;
unsigned int NUM_VERTS_PER_STRIP ;
//vector <vec2> planeuvs;
vector <float> planeuvs;
vector <vec3> tower_pos;


//debugging
Drawable* light_orb;
GLuint suntexture;

//Instancing
vector <Tower* > mage = {};
vector <Enemy* > orcs = {};
//vector <Tower* > archer = {};
vector <Enemy* > orcs1 = {};
vector <Enemy* > orcs2 = {};
vector <Enemy* > orcs3 = {};
const GLfloat s = 10.0f;//grid's square side size magnification
const GLfloat grid_size = 10.0f;//number of boxes(or half the number of triangles)

//Trees
Drawable* tree;
Drawable* leaves;
GLuint tree_trunk;
GLuint tree_leaves;
vector <Tree*> trees;


//animation
//GLuint animationProgram, animeprojectionMatrixLocation, animeviewMatrixLocation, animemodelMatrixLocation,
//shoulder_right_dr_Location, elbow_right_dr_Location;

//game menu
bool pause = true;//false;
bool build = false;
int index=0;
float dt;


//particles
GLuint blood, debris;
GLuint particleShaderProgram, part_vMatrix, part_pMatrix, particleColorSampler;
Drawable* obj_particle;
//FountainEmitter f_emitter;



void renderHelpingWindow() {
    //static int counter = 0;

    ImGui::Begin("Helper Window");                          // Create a window called "Hello, world!" and append into it.

    if (build) {
        ImGui::Text("Allowed to build: Yes");// Display some text (you can use a format strings too)
    }
    else {
        ImGui::Text("Allowed to build: No");
    }
    ImGui::Text("Build on position: %d", index+1);
    /*ImGui::ColorEdit3("Background", &background_color[0]);

    ImGui::SliderFloat("x position", &slider_emitter_pos[0], -30.0f, 30.0f);
    ImGui::SliderFloat("y position", &slider_emitter_pos[1], -30.0f, 30.0f);
    ImGui::SliderFloat("z position", &slider_emitter_pos[2], -30.0f, 30.0f);
    ImGui::SliderInt("particles", &particles_slider, 0, 20000);*/


    //if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
    //    counter++;
    //ImGui::SameLine();
    //ImGui::Text("counter = %d", counter);

    /*ImGui::Checkbox("Use sorting", &use_sorting);
    ImGui::Checkbox("Use rotations", &use_rotations);*/

    ImGui::Text("Performance %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void renderDepthMap() {
    // using the correct shaders to visualize the depth texture on the quad
    glUseProgram(miniMapProgram);

    //enabling the texture - follow the aforementioned pipeline
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glUniform1i(quadTextureSamplerLocation, 0);

    // Drawing the quad
    quad->bind();
    quad->draw();
}


void uploadLight(const Light& light) {
    glUniform4f(LaLocation, light.La.r, light.La.g, light.La.b, light.La.a);
    glUniform4f(LdLocation, light.Ld.r, light.Ld.g, light.Ld.b, light.Ld.a);
    glUniform4f(LsLocation, light.Ls.r, light.Ls.g, light.Ls.b, light.Ls.a);
    glUniform3f(lightPositionLocation, light.lightPosition_worldspace.x,
        light.lightPosition_worldspace.y, light.lightPosition_worldspace.z);
    glUniform1f(lightPowerLocation, light.power);
}

void depth_pass(mat4 viewMatrix, mat4 projectionMatrix) {

    // Task 3.3
    //*/


    // Setting viewport to shadow map size
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    
    glBindFramebuffer(GL_FRAMEBUFFER, depthFrameBuffer);
    
    // Cleaning the framebuffer depth information (stored from the last render)
    glClear(GL_DEPTH_BUFFER_BIT);

    // Selecting the new shader program that will output the depth component
    glUseProgram(depthProgram);

    // sending the view and projection matrix to the shader
    mat4 view_projection = projectionMatrix * viewMatrix;
    glUniformMatrix4fv(shadowViewProjectionLocation, 1, GL_FALSE, &view_projection[0][0]);

    for (int i = 0; i < mage.size(); i++) {

        if (mage[i]->current_hp > 0.1f) {
            glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, &mage[i]->towerM[0][0]);

            mage[i]->towerModel->bind();
            mage[i]->towerModel->draw();
        }
        if (mage[i]->attacking) {
            if (mage[i]->target_id->hit && mage[i]->shoot && mage[i]->the_type=='a') {
                glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, &mage[i]->projectile->projectileM[0][0]);
                mage[i]->projectile->proj_model->bind();
                mage[i]->projectile->proj_model->draw();
            }

        }
        if (mage[i]->power_attack && orcs.size() > 0) {
            if (mage[i]->meteor->pos.y > 0.0f) {
                //mat4 M = translate(mat4(), mage[i]->meteor->pos) * scale(mat4(), vec3(0.5f, 0.5f, 0.5f));
                glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, &mage[i]->meteor->projectileM[0][0]);
                mage[i]->meteor->proj_model->bind();
                mage[i]->meteor->proj_model->draw();
                
            }
        }
        //cout << i << endl;
    }
   
    for (int i = 0; i < orcs.size(); i++) {
        if (orcs[i]->current_hp > 0.1f) {
            glEnable(GL_CULL_FACE);
            //glCullFace(GL_FRONT);
            mat4 M = orcs[i]->enemyM * translate(mat4(), vec3(-0.9, 0.6, -0.9));//to stop pete panning
            glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, &M[0][0]);
            //glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, &orcs[i]->enemyM[0][0]);
            //cout << orcs.size() << endl;
            orcs[i]->enemyModel->bind();
            orcs[i]->enemyModel->draw();
            orcs[i]->armorModel->bind();
            orcs[i]->armorModel->draw();
            //glCullFace(GL_BACK);
            glDisable(GL_CULL_FACE);
        }
        if (orcs[i]->attacking) {
            if (orcs[i]->target_id->hit && orcs[i]->shoot) {
                glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &orcs[i]->projectile->projectileM[0][0]);
                orcs[i]->projectile->proj_model->bind();
                orcs[i]->projectile->proj_model->draw();
            }

        }
        
       

    }
    //archer towers
    /*for (int i = 0; i < archer.size(); i++) {
        
        glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, &archer[i]->towerM[0][0]);
        archer[i]->towerModel->bind();
        archer[i]->towerModel->draw();
    }*/

    
    mat4 planeModelMatrix = scale(mat4(), 4.0f*vec3(0.05, 0.05, 0.05));
    glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, &planeModelMatrix[0][0]);
    glBindVertexArray(terrainVAO);

    // render the mesh triangle strip by triangle strip - each row at a time
    for (unsigned int strip = 0; strip < NUM_STRIPS; ++strip)
    {
        glDrawElements(GL_TRIANGLE_STRIP,   // primitive type
            NUM_VERTS_PER_STRIP, // number of indices to render
            GL_UNSIGNED_INT,     // index data type
            (void*)(sizeof(unsigned int)
                * NUM_VERTS_PER_STRIP
                * strip)); // offset to starting index
    }
    /*mat4 planeModelMatrix = scale(mat4(), vec3(2, 2, 2));
    glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, &planeModelMatrix[0][0]);
    plane->bind();
    plane->draw();*/
    for (int i = 0; i < trees.size(); i++) {
        mat4 M = trees[i]->modelM * translate(mat4(), vec3(-2, 0.9, -2));//to stop peter panning
        glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, &M[0][0]);
        //glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, &trees[i]->modelM[0][0]);
        trees[i]->treeModel->bind();
        trees[i]->treeModel->draw();

        trees[i]->leavesModel->bind();
        trees[i]->leavesModel->draw();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //*/
}

void lighting_pass(mat4 viewMatrix, mat4 projectionMatrix, vector <Enemy*>& orcs) {
    
    // Step 1: Binding a frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, W_WIDTH, W_HEIGHT);

    // Step 2: Clearing color and depth info
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Step 3: Selecting shader program
    glUseProgram(shaderProgram);

    // Making view and projection matrices uniform to the shader program
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

    // uploading the light parameters to the shader program

    uploadLight(*light);
    // Task 4.1 Display shadows on the scene
    //*/
    // Sending the shadow texture to the shaderProgram
    glActiveTexture(GL_TEXTURE0);								// Activate texture position
    glBindTexture(GL_TEXTURE_2D, depthTexture);			// Assign texture to position 
    glUniform1i(depthMapSampler, 0);

    glUniform1i(useTextureLocation, 0);
    // Sending the light View-Projection matrix to the shader program
    mat4 light_VP = light->lightVP();
    glUniformMatrix4fv(lightVPLocation, 1, GL_FALSE, &light_VP[0][0]);
    
    for (int i = 0; i < mage.size(); i++) {
        if (mage[i]->current_hp > 0.1) {
            glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &mage[i]->towerM[0][0]);

            if (mage[i]->the_type == 'm') {
                glUniform1i(useTextureLocation, 0);



                glActiveTexture(GL_TEXTURE1);								// Activate texture position
                glBindTexture(GL_TEXTURE_2D, magetexture);			// Assign texture to position 
                glUniform1i(modeltextureSampler, 1);

                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, mageAOtexture);
                glUniform1i(ambientSampler, 2);

                glActiveTexture(GL_TEXTURE3);
                glBindTexture(GL_TEXTURE_2D, mageNormals);
                glUniform1i(normalSampler, 3);

            }
            else {
                glUniform1i(useTextureLocation, 3);
                glActiveTexture(GL_TEXTURE1);								// Activate texture position
                glBindTexture(GL_TEXTURE_2D, archertexture);			// Assign texture to position 
                glUniform1i(modeltextureSampler, 1);

                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, archerAOtexture);
                glUniform1i(ambientSampler, 2);

                glActiveTexture(GL_TEXTURE3);
                glBindTexture(GL_TEXTURE_2D, archerSpecular);
                glUniform1i(specularSampler, 3);

                glActiveTexture(GL_TEXTURE4);
                glBindTexture(GL_TEXTURE_2D, archerNormals);
                glUniform1i(normalSampler, 4);

            }
            mage[i]->towerModel->bind();
            mage[i]->towerModel->draw();

            //tower's range
            glUniform1i(glGetUniformLocation(shaderProgram, "range"), 1);
            mat4 attack_rangeModel = mage[i]->towerTranslation * translate(mat4(), vec3(0, 0.3, 0)) * rotate(mat4(), 3.14f / 2.0f, vec3(1, 0, 0)) * scale(mat4(), vec3(mage[i]->range, mage[i]->range, mage[i]->range));
            glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &attack_rangeModel[0][0]);
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, attack_range);
            glUniform1i(glGetUniformLocation(shaderProgram, "tower_rangeSampler"), 4);
            obj_particle->bind();
            obj_particle->draw();
            glUniform1i(glGetUniformLocation(shaderProgram, "range"), 0);


            //tower's hp and mana
            mat4 hp_model_Matrix = mage[i]->towerTranslation * mage[i]->hp_pos;
            glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &hp_model_Matrix[0][0]);
            glUniform1i(hpLocation, 1);
            glUniform1f(glGetUniformLocation(shaderProgram, "max_hp"), mage[i]->hp);
            glUniform1f(glGetUniformLocation(shaderProgram, "cur_hp"), mage[i]->current_hp);
            //quad->bind();
            //quad->draw();
            //glUniform1i(hpLocation, 0);
            //tower's meteor strike

            //mat4 mana_model_Matrix = mage[i]->towerTranslation * mage[i]->mana_pos;
            //glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &mana_model_Matrix[0][0]);
            //glUniform1i(hpLocation, 1);
            glUniform1i(glGetUniformLocation(shaderProgram, "mana"), 1);
            glUniform1f(glGetUniformLocation(shaderProgram, "max_mana"), mage[i]->mana_max);
            glUniform1f(glGetUniformLocation(shaderProgram, "cur_mana"), mage[i]->mana_cd);
            quad->bind();
            quad->draw();
            glUniform1i(glGetUniformLocation(shaderProgram, "mana"), 0);
            glUniform1i(hpLocation, 0);
        }
        
        //projectile
        if (mage[i]->attacking) {
            if (mage[i]->target_id->hit && mage[i]->shoot) {
                if (mage[i]->the_type == 'm') {
                    //glUniform1i(useTextureLocation, 4);
                    glUniform1i(useTextureLocation, 2);
                    glActiveTexture(GL_TEXTURE1);
                    glBindTexture(GL_TEXTURE_2D, energyDiffuse);
                    //glBindTexture(GL_TEXTURE_2D, arrowDiffuse);
                    glUniform1i(modeltextureSampler, 1);
                    mat4 M = translate(mat4(), mage[i]->projectile->pos);
                    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &M[0][0]);
                }
                else {
                    glUniform1i(useTextureLocation, 4);
                    glActiveTexture(GL_TEXTURE1);
                    glBindTexture(GL_TEXTURE_2D, arrowDiffuse);
                    glUniform1i(modeltextureSampler, 1);
                    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &mage[i]->projectile->projectileM[0][0]);
                }
                glActiveTexture(GL_TEXTURE3);
                glBindTexture(GL_TEXTURE_2D, arrowSpecular);
                glUniform1i(specularSampler, 3);
                //cout << to_string(mage[i]->projectile->pos) << endl;
                
                
                mage[i]->projectile->proj_model->bind();
                mage[i]->projectile->proj_model->draw();

                
            }

        }
        if (mage[i]->power_attack && orcs.size()>0) {
            if (mage[i]->meteor->pos.y > 0.0f) {
                glUniform1i(useTextureLocation, 4);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, comettexture);
                glUniform1i(modeltextureSampler, 1);
                //mat4 M = translate(mat4(), mage[i]->meteor->pos);
                glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &mage[i]->meteor->projectileM[0][0]);
                mage[i]->meteor->proj_model->bind();
                mage[i]->meteor->proj_model->draw();

                //sglUseProgram(particleShaderProgram);
                //glUniform1i(glGetUniformLocation(particleShaderProgram, "trail"), 1);
                ////glUniform1f(glGetUniformLocation(particleShaderProgram, "life"), 1)
                //mage[i]->projectile->trail->updateParticles(0, dt, camera->position, vec3(0, 0, 0));
                //mage[i]->projectile->trail->renderParticles();
                //glUniform1i(glGetUniformLocation(particleShaderProgram, "trail"), 0);
                //glUseProgram(shaderProgram);
            }
        }
    
    }
    
    for (int i = 0; i < orcs.size(); i++) {
        if (orcs[i]->current_hp > 0.1f) {
            glUniform1i(glGetUniformLocation(shaderProgram, "orc"), 1);
            glUniform1i(useTextureLocation, 0);
            glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &orcs[i]->enemyM[0][0]);

            glActiveTexture(GL_TEXTURE1);								// Activate texture position
            glBindTexture(GL_TEXTURE_2D, orctexture);			// Assign texture to position 
            glUniform1i(modeltextureSampler, 1);

            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, orcAOtexture);
            glUniform1i(ambientSampler, 2);

            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, orcNormals);
            glUniform1i(normalSampler, 3);
            //glEnable(GL_CULL_FACE);
            orcs[i]->enemyModel->bind();
            orcs[i]->enemyModel->draw();


            glActiveTexture(GL_TEXTURE1);								// Activate texture position
            glBindTexture(GL_TEXTURE_2D, armortexture);			// Assign texture to position 
            glUniform1i(modeltextureSampler, 1);

            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, armorAOtexture);
            glUniform1i(ambientSampler, 2);

            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, armorNormals);
            glUniform1i(normalSampler, 3);

            orcs[i]->armorModel->bind();
            orcs[i]->armorModel->draw();
            //glDisable(GL_CULL_FACE);
            glUniform1i(glGetUniformLocation(shaderProgram, "orc"), 0);
            mat4 hp_model_Matrix = orcs[i]->enemyTranslation * orcs[i]->hp_pos;
            glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &hp_model_Matrix[0][0]);
            glUniform1i(hpLocation, 1);
            glUniform1f(glGetUniformLocation(shaderProgram, "max_hp"), orcs[i]->hp);
            glUniform1f(glGetUniformLocation(shaderProgram, "cur_hp"), orcs[i]->current_hp);
            quad->bind();
            quad->draw();
            glUniform1i(hpLocation, 0);
        }
        if (orcs[i]->attacking) {
            if (orcs[i]->target_id->hit && orcs[i]->shoot) {
                glUniform1i(useTextureLocation, 4);
                //glUniform1i(useTextureLocation, 2);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, boulderDiffuse);
                glUniform1i(modeltextureSampler, 1);

                glActiveTexture(GL_TEXTURE3);
                glBindTexture(GL_TEXTURE_2D, arrowSpecular);
                glUniform1i(specularSampler, 3);
                //cout << to_string(mage[i]->projectile->pos) << endl;
                //mat4 M = translate(mat4(), mage[i]->projectile->pos);
                //glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &M[0][0]);
                glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &orcs[i]->projectile->projectileM[0][0]);
                orcs[i]->projectile->proj_model->bind();
                orcs[i]->projectile->proj_model->draw();
            }

        }
        //cout << orcs[i]->f_emitter->dead_particles << endl;
        if (orcs[i]->current_hp < 0.1f && !orcs[i]->particle_effect) {
            orcs.erase(orcs.begin()+i);
        }
       
        
        

    }
   
    //mat4 planeModelMatrix = scale(mat4(), vec3(2, 2, 2));
    //glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &planeModelMatrix[0][0]);
    glActiveTexture(GL_TEXTURE1);								// Activate texture position
    glBindTexture(GL_TEXTURE_2D, grasstexture);			// Assign texture to position 
    glUniform1i(grasstextureSampler, 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, pathtexture);
    glUniform1i(pathtextureSampler, 2);
    glUniform1i(useTextureLocation, 1);
    //plane->bind();
    //plane->draw();
    // draw mesh
    //glUniform1i(useTextureLocation, 2);
    mat4 planeModelMatrix = scale(mat4(), 4.0f*vec3(0.05, 0.05, 0.05));
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &planeModelMatrix[0][0]);
    glBindVertexArray(terrainVAO);
    
    // render the mesh triangle strip by triangle strip - each row at a time
    //glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    for (unsigned int strip = 0; strip < NUM_STRIPS; ++strip)
    {
        glDrawElements(GL_TRIANGLE_STRIP,   // primitive type
            NUM_VERTS_PER_STRIP, // number of indices to render
            GL_UNSIGNED_INT,     // index data type
            (void*)(sizeof(unsigned int)
                * NUM_VERTS_PER_STRIP
                * strip)); // offset to starting index
    }

    glUniform1i(useTextureLocation, 4);
    for (int i = 0; i < trees.size(); i++) {
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &trees[i]->modelM[0][0]);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, tree_trunk);
        glUniform1i(glGetUniformLocation(shaderProgram, "diffuseColorSampler"), 4);
        trees[i]->treeModel->bind();
        trees[i]->treeModel->draw();

        glBindTexture(GL_TEXTURE_2D, tree_leaves);
        trees[i]->leavesModel->bind();
        trees[i]->leavesModel->draw();
    }


    glUniform1i(useTextureLocation, 0);
    mat4 orbTranslation = translate(mat4(), light->lightPosition_worldspace);
    mat4 orbSacling = scale(mat4(), vec3(0.5f, 0.5f, 0.5f));
    mat4 orbModelMatrix = orbTranslation * orbSacling;
    glActiveTexture(GL_TEXTURE1);								// Activate texture position
    glBindTexture(GL_TEXTURE_2D, suntexture);			// Assign texture to position 
    glUniform1i(modeltextureSampler, 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, suntexture);
    glUniform1i(ambientSampler, 2);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &orbModelMatrix[0][0]);
    light_orb->bind();
    light_orb->draw();

}



void createContext() {
    //particle shader program
    particleShaderProgram = loadShaders(
        "ParticleShader.vertexshader",
        "ParticleShader.fragmentshader");
    part_vMatrix = glGetUniformLocation(particleShaderProgram, "V");
    part_pMatrix = glGetUniformLocation(particleShaderProgram, "P");
    particleColorSampler = glGetUniformLocation(particleShaderProgram, "particle_texture");
    //main shader program
    shaderProgram = loadShaders("ShadowMapping.vertexshader", "ShadowMapping.fragmentshader");
    LaLocation = glGetUniformLocation(shaderProgram, "light.La");
    LdLocation = glGetUniformLocation(shaderProgram, "light.Ld");
    LsLocation = glGetUniformLocation(shaderProgram, "light.Ls");
    lightPositionLocation = glGetUniformLocation(shaderProgram, "light.lightPosition_worldspace");
    lightPowerLocation = glGetUniformLocation(shaderProgram, "light.power");
    projectionMatrixLocation = glGetUniformLocation(shaderProgram, "P");
    viewMatrixLocation = glGetUniformLocation(shaderProgram, "V");
    modelMatrixLocation = glGetUniformLocation(shaderProgram, "M");
    depthMapSampler = glGetUniformLocation(shaderProgram, "shadowMapSampler");
    lightVPLocation = glGetUniformLocation(shaderProgram, "lightVP");
    useTextureLocation = glGetUniformLocation(shaderProgram, "useTexture");
    hpLocation = glGetUniformLocation(shaderProgram, "hp");
    hpMatrixLocation = glGetUniformLocation(shaderProgram, "hpmodel_matrix");
    //depth texture program
    depthProgram = loadShaders("Depth.vertexshader", "Depth.fragmentshader");
    shadowViewProjectionLocation = glGetUniformLocation(depthProgram, "VP");
    shadowModelLocation = glGetUniformLocation(depthProgram, "M");
    glGenFramebuffers(1, &depthFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, depthFrameBuffer);
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0,
        GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        glfwTerminate();
        throw runtime_error("Frame buffer not initialized correctly");
    }

    // Binding the default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    //vec3 points[3] = { vec3(681 + 40 - 1024, 0, 886 + 40 - 1024) / 20.0f,     //point1
    //                   vec3(759 + 40 - 1024, 0, 760 + 40 - 1024) / 20.0f,     //point2
    //                   vec3(887 + 40 - 1024, 0, 687 + 40 - 1024) / 20.0f };   //point3
    vec3 point1 = vec3(681 + 40 - 1024, 0, 886 + 40 - 1024)/20.0f;
    vec3 point2 = vec3(759 + 40 - 1024, 0, 760 + 40 - 1024)/ 20.0f;
    vec3 point3 = vec3(887 + 40 - 1024, 0, 687 + 40 - 1024) / 20.0f;
    
    tower_pos.push_back(point1);
    tower_pos.push_back(point2);
    tower_pos.push_back(point3);

    tower_pos.push_back(vec3(-point3.x, point3.y, point3.z));
    tower_pos.push_back(vec3(-point2.x, point2.y, point2.z));
    tower_pos.push_back(vec3(-point1.x, point1.y, point1.z));

    tower_pos.push_back(vec3(-point1.x, point1.y, -point1.z));
    tower_pos.push_back(vec3(-point2.x, point2.y, -point2.z));
    tower_pos.push_back(vec3(-point3.x, point3.y, -point3.z));

    tower_pos.push_back(vec3(point3.x, point3.y, -point3.z));
    tower_pos.push_back(vec3(point2.x, point2.y, -point2.z));
    tower_pos.push_back(vec3(point1.x, point1.y, -point1.z));
    
    

      

    //creating models
    magetowermodel = new Drawable("Magic_tower.obj");
    archertowermodel = new Drawable("War_Tower.obj");
    //arrow = new Drawable("Arrow.obj");
    //arrow = new Drawable("arrow_centered.obj");
    arrow = new Drawable("arrow_down.obj");
    orcmodel = new Drawable("Orc2.obj");
    armor = new Drawable("armor.obj");
    tree = new Drawable("tree_trunk.obj");
    leaves = new Drawable("tree_leaves.obj");
    
    //attack range
    attack_range = loadSOIL("area.png");

    //creating textures
    modeltextureSampler = glGetUniformLocation(shaderProgram, "diffuseColorSampler");
    ambientSampler = glGetUniformLocation(shaderProgram, "ambientColorSampler");
    specularSampler = glGetUniformLocation(shaderProgram, "specularColorSampler");
    grasstextureSampler = glGetUniformLocation(shaderProgram, "grassColorSampler");
    pathtextureSampler = glGetUniformLocation(shaderProgram, "pathColorSampler");
    heightmapSampler = glGetUniformLocation(shaderProgram, "heightmap");
    normalSampler = glGetUniformLocation(shaderProgram, "normals_from_texture");
    comettexture = loadSOIL("comet.png");
    magetexture = loadSOIL("Magic_Tower_LP_BaseColor_v2.png");//mage tower model
    mageAOtexture = loadSOIL("Magic_Tower_LP_AO.png");
    mageNormals = loadSOIL("Magic_Tower_LP_Normal.png");
    energyDiffuse = loadSOIL("energy ball8.png");

    archertexture = loadSOIL("archer_DIffuse.jpg");
    archerAOtexture = loadSOIL("archer_AO.jpg");
    archerSpecular = loadSOIL("archer_Specular.jpg");
    archerNormals = loadSOIL("archer_Normal.jpg");
    arrowDiffuse = loadSOIL("Arrow_Diffuse.png");
    arrowSpecular = loadSOIL("Arrow_Specular.png");
    

    orctexture = loadSOIL("Orc_Body_Color.png");//orc model
    orcAOtexture = loadSOIL("Orc_Body_AO.png");
    orcNormals = loadSOIL("Orc_Body_Normal.png");
    armortexture = loadSOIL("Orc_Accesories_BaseColor.png");
    armorAOtexture = loadSOIL("Orc_Accesories_AO.png");
    armorNormals = loadSOIL("Orc_Accesories_Normal.png");
    boulderDiffuse = loadSOIL("boulder.png");

    tree_trunk = loadSOIL("tree wood.jpg");
    tree_leaves = loadSOIL("leaves.jpg");

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //creating the plane----------------------------------------------------
    float y = 0.0f; // offset to move the place up/down across the y axis
    vector<vec3> floorVertices = {
        vec3(-20.0f, y, -20.0f),
        vec3(-20.0f, y,  20.0f),
        vec3(20.0f,  y,  20.0f),
        vec3(20.0f,  y,  20.0f),
        vec3(20.0f,  y, -20.0f),
        vec3(-20.0f, y, -20.0f),

    };

    // plane normals
    vector<vec3> floorNormals = {
        vec3(0.0f, 1.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f)
    };

    // plane uvs
    vector<vec2> floorUVs = {
        vec2(0.0f, 0.0f),
        vec2(0.0f, 1.0f),
        vec2(1.0f, 1.0f),
        vec2(1.0f, 1.0f),
        vec2(1.0f, 0.0f),
        vec2(0.0f, 0.0f),
    };
    grasstexture = loadSOIL("grassv4.png");
    pathtexture = loadSOIL("path.jpg");
    
    //heightmap = loadSOIL("terrain.png");

    plane = new Drawable(floorVertices, floorUVs, floorNormals);
    //Heightmap
    //planedata = stbi_load("terrainv5_small.png",
    //planedata = stbi_load("terrain_new_small4.png",
   // planedata = stbi_load("final_terrain2_small.png",
    //planedata = stbi_load("game_terrain2.png",//square map
    planedata = stbi_load("game_terrain_circ3.png",
        &hm_width, &hm_height, &hm_nChannels,
        0);
    
    //float yScale = 64.0f / 256.0f, yShift = 0.0f;  // apply a scale+shift to the height data
    ///float yScale = 64.0f / 256.0f, yShift = 0.0f;
    SimplexNoise noise;
    //cout << hm_height << endl;
    for (int i = 0; i < hm_height; i++)
    {
        for (int j = 0; j < hm_width; j++)
        {
            // retrieve texel for (i,j) tex coord
            unsigned char* texel = planedata + (j + hm_width * i) * hm_nChannels;
            
            // raw height at coordinate
            unsigned char y = texel[0];
            float x = -hm_width / 2.0f + j;
            //cout << x << endl;
            float z = -hm_height / 2.0f + i;
            vertices.push_back(x);// v.x
            float y_cord = (float)y; //* yScale - yShift;
            if ((int) y == 0) {
                vertices.push_back(y_cord);
            }
            else {
              
                float  y_noise = (float)y / 255.0f * (noise.fractal(4, x, z, 254.0f) + 1.0f) / 2.0f * 80.0f;
                vertices.push_back(y_noise); // v.y
            }
            
            vertices.push_back(z);// v.z
            planeuvs.push_back((float)i / ((float)hm_height-1.0f));
            planeuvs.push_back((float)j / ((float)hm_width-1.0f));
        }
    }
    stbi_image_free(planedata);
    for (unsigned int i = 0; i < hm_height - 1; i++)       // for each row a.k.a. each strip
    {
        for (unsigned int j = 0; j < hm_width; j++)      // for each column
        {
            for (unsigned int k = 0; k < 2; k++)      // for each side of the strip
            {
                indices.push_back(j + hm_width * (i + k));
            }
        }
    }

    NUM_STRIPS = hm_height - 1;
    NUM_VERTS_PER_STRIP = hm_width * 2;



    // register VAO
    
    glGenVertexArrays(1, &terrainVAO);
    glBindVertexArray(terrainVAO);

    glGenBuffers(1, &terrainVBO);
    glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
    glBufferData(GL_ARRAY_BUFFER,
        vertices.size() * sizeof(float),       // size of vertices buffer
        &vertices[0],                          // pointer to first element
        GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
    
    //terrain uvs
    glGenBuffers(1, &myVBO);
    glBindBuffer(GL_ARRAY_BUFFER, myVBO);
    glBufferData(GL_ARRAY_BUFFER, planeuvs.size() * sizeof(float),
        &planeuvs[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(2);

    //terrain indices
    glGenBuffers(1, &terrainEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(unsigned int), // size of indices buffer
        &indices[0],                           // pointer to first element
        GL_STATIC_DRAW);
    glEnableVertexAttribArray(3);

    //Creating the quad for the miniMap-----------------------------------------------------------
    vector<vec3> quadVertices = {
    vec3(0.5, 0.5, 0.0),
    vec3(1.0, 0.5, 0.0),
    vec3(1.0, 1.0, 0.0),
    vec3(1.0, 1.0, 0.0),
    vec3(0.5, 1.0, 0.0),
    vec3(0.5, 0.5, 0.0)
    };

    vector<vec2> quadUVs = {
      vec2(0.0, 0.0),
      vec2(1.0, 0.0),
      vec2(1.0, 1.0),
      vec2(1.0, 1.0),
      vec2(0.0, 1.0),
      vec2(0.0, 0.0)
    };

    quad = new Drawable(quadVertices, quadUVs);
    miniMapProgram = loadShaders("SimpleTexture.vertexshader", "SimpleTexture.fragmentshader");
    quadTextureSamplerLocation = glGetUniformLocation(miniMapProgram, "textureSampler");

    //particles
    debris = loadSOIL("debris.png");
    blood = loadSOIL("blood.png");
    //blood = loadSOIL("lightning.jpg");
    obj_particle = new Drawable("quad.obj");

    //debug
    glfwSetKeyCallback(window, pollKeyboard);
    light_orb = new Drawable("earth.obj");
    //suntexture = loadSOIL("sun2.png");
    suntexture = loadSOIL("sauron.jpg");
}

void mainLoop() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    light->update();

    int wave = 0;
    
    //mat4 floorScaling = scale(mat4(), vec3(2, 2, 2));//Model matrices

    //mat4 mageTranslation = translate(mat4(), vec3(5.0f, 0.0f, 10.0f));
    mat4 mageRotation = rotate(mat4(), 3.14f, vec3(0.0f, 1.0f, 0.0f));

    //mat4 orcTranslation = translate(mat4(), vec3(5.0f, 0.7 * 1.160904, 0.0f));
    mat4 orcRotation = rotate(mat4(), 0.0f, vec3(0.0f, 1.0f, 0.0f));

    mat4 projection = camera->projectionMatrix;
    mat4 view = camera->viewMatrix;

    //FountainEmitter f_emitter = FountainEmitter(obj_particle, 100);
    mat4 VP = projection * view;
    //cout << tower_pos.size() << endl;
    //Instancing
    for (int i = 0; i < 6; i++) {

        mage.push_back(new Tower(VP, magetowermodel, 'm', tower_pos[i], mageRotation, obj_particle));
    }

    //wave 1
    for (int i = 0; i < 10; i++) {//10
        for (int j = 0; j < 2; j++) {//2
            orcs1.push_back(new Enemy(VP, orcmodel, armor, 'o',
                vec3((490.0f - 512.0f) * 0.05f + float(j) * 2.0f, 0.7 * 1.160904, -512.0f * 0.05f - (float)i),
                orcRotation, obj_particle));//orc's orientation


        }
    }
    //wave 2
    for (int i = 0; i < 15; i++) {//10
        for (int j = 0; j < 2; j++) {//2
            orcs2.push_back(new Enemy(VP, orcmodel, armor, 'o',
                //vec3((495.0f - 512.0f) * 0.05f + float(j) * 2.0f, 0.7 * 1.160904,  - 512.0f * 0.05f - (float)i), //orc's position
                vec3((490.0f - 512.0f) * 0.05f + float(j) * 2.0f, 0.7 * 1.160904, -512.0f * 0.05f - (float)i),
                orcRotation, obj_particle));//orc's orientation

            

        }
    }

    //wave 3
    for (int i = 0; i < 20; i++) {//10
        for (int j = 0; j < 2; j++) {//2
            orcs3.push_back(new Enemy(VP, orcmodel, armor, 'o',
                //vec3((495.0f - 512.0f) * 0.05f + float(j) * 2.0f, 0.7 * 1.160904,  - 512.0f * 0.05f - (float)i), //orc's position
                vec3((490.0f - 512.0f) * 0.05f + float(j) * 2.0f, 0.7 * 1.160904, -512.0f * 0.05f - (float)i),
                orcRotation, obj_particle));//orc's orientation

        }
    }

    orcs = orcs1;

    //mat4 archerTranslation;
    for (int i = 0; i < 6; i++) {
        mage.push_back(new Tower(VP, archertowermodel, 'a', tower_pos[i + 6], mageRotation, obj_particle));

    }

    //trees
    int tree_pos;
    vector <vec2> quarter = { vec2(0, 0), vec2(321,0), vec2(0,267), vec2(321, 267) };//z,x,offset
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 10; j++) {//vertices size 512*3*512 = 786432

            tree_pos = (rand() % 183 + (int)quarter[i].x) * 3 + 512 * (rand() % 245 + (int)quarter[i].y) * 3;
            //cout << tree_pos << endl;
            trees.push_back(new Tree(tree, leaves, vec3(vertices[tree_pos], vertices[tree_pos + 1], vertices[tree_pos + 2]) * 0.2f, 0));
        }
    }
    //test boom
    //ExplosionEmitter* test_emitter = new ExplosionEmitter(obj_particle, 8000, vec3(5,0,-10));//5000
    //float dt = 0.001;
    float t = glfwGetTime();
    //float previousTime = glfwGetTime();
    do {
        float currentTime = glfwGetTime();
        //cout << orcs.size() << endl;
        if (orcs.size() == 0) {
            wave += 1;
            

            if (wave == 1) {
                orcs = orcs2;
            }
            else if(wave<3){
                orcs = orcs3;
            }
            pause = true;
        }
        //float dt = currentTime - previousTime;
        //cout << dt << endl;
        dt = currentTime - t;
        //cout << dt << endl;
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        camera->update();
        mat4 projectionMatrix = camera->projectionMatrix;
        mat4 viewMatrix = camera->viewMatrix;

        light->update();
        //glCullFace(GL_FRONT);
        depth_pass(light->viewMatrix, light->projectionMatrix);
        //glCullFace(GL_BACK);
        mat4 light_proj = light->projectionMatrix;
        mat4 light_view = light->viewMatrix;

        if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
            lighting_pass(light_view, light_proj, orcs);
        }
        else {
            // Render the scene from camera's perspective
            lighting_pass(viewMatrix, projectionMatrix, orcs);
        }
        //float dt1 = 0.001f;
        //cout << pause << endl;
        //cout << dt << endl;
        if (pause == false) {
            attack_handler(orcs, mage);
            for (int i = 0; i < orcs.size(); i++) {
                //cout << to_string(orcs[1]->x) << endl;
                //cout << to_string(orcs[i]->v) << endl;;
                orcs[i]->update(dt);
                


            }
            for (int i = 0; i < mage.size(); i++) {
                mage[i]->update(dt);
            }
            particle_effects(camera->viewMatrix, camera->projectionMatrix, currentTime, dt, orcs, mage);
            /*glUniform1i(glGetUniformLocation(particleShaderProgram, "explosion"), 1);
            test_emitter->updateParticles(dt, dt, camera->position, vec3(0, 0, 0));
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(0,debris);
            glUniform1i(particleColorSampler, 0);
            test_emitter->renderParticles();
            glUniform1i(glGetUniformLocation(particleShaderProgram, "explosion"), 0);*/
        }
        

        //particles test
        //VP = camera->projectionMatrix * camera->viewMatrix;
        //glUseProgram(particleShaderProgram);
        //glUniformMatrix4fv(part_vpMatrix, 1, GL_FALSE, &VP[0][0]);

        ////*/ Use particle based drawing
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, debris);
        //glUniform1i(particleColorSampler, 0);
        //if (pause==false)
        //    f_emitter.updateParticles(currentTime, dt, camera->position);
        //    //o_emitter.updateParticles(currentTime, dt, camera->position);
        ///*if (pause == true) {
        //    cout << to_string(camera->position) << endl;
        //}*/
        //f_emitter.renderParticles();
        //cout << f_emitter.active_particles << endl;
        
        //particle_effects(camera->viewMatrix, camera->projectionMatrix, currentTime, dt, orcs, mage);
        renderHelpingWindow();
        renderDepthMap();
        // Swap buffers
        glfwSwapBuffers(window);

        // Events
        
        glfwPollEvents();
        //previousTime = glfwGetTime();
        t = currentTime;
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0);
}
void initialize() {
    // Initialize GLFW
    if (!glfwInit()) {
        throw runtime_error("Failed to initialize GLFW\n");
    }

    glfwWindowHint(GLFW_SAMPLES, 4);  // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(W_WIDTH, W_HEIGHT, TITLE, NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        throw runtime_error(string(string("Failed to open GLFW window.") +
            " If you have an Intel GPU, they are not 3.3 compatible." +
            "Try the 2.1 version.\n"));
    }
    glfwMakeContextCurrent(window);

    // Set Handler function for resizing the window
    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Start GLEW extension handler
    glewExperimental = GL_TRUE;

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        throw runtime_error("Failed to initialize GLEW\n");
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Hide the mouse and enable unlimited movement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, W_WIDTH / 2, W_HEIGHT / 2);

    // Set background color (gray) [r, g, b, a]
    //glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
    glClearColor(0.673f, 0.777f, 1.0f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    //glEnable(GL_CULL_FACE);

    //Blend must be enabled
    //*/
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //*/

    // Log
    logGLParameters();

    //Create Camera
    camera = new Camera(window);
    //Delete light
    light = new Light(window,
        vec4{ 0.3, 0.3, 0.3, 1 },
        vec4{ 1, 1, 1, 1 },
        vec4{ 1, 1, 1, 1 },
        //vec3{ 511 * 0.05, 10, 511 * 0.05 },
        //vec3{ -512*0.05, 30, -512*0.05 },
        vec3{ -1030 * 0.05, 40, -1030 * 0.05 },
        1.0f
    );
}

void free() {
    // Free allocated buffers
    glDeleteBuffers(1, &myVBO);
    //glDeleteBuffers(1, &colorsVBO);
    glDeleteVertexArrays(1, &gridVAO);
    glDeleteProgram(shaderProgram);
    glDeleteProgram(miniMapProgram);
    glDeleteProgram(depthProgram);
    glDeleteVertexArrays(1, &terrainVAO);
    glDeleteBuffers(1, &terrainVBO);
    glDeleteBuffers(1, &terrainEBO);
    glDeleteBuffers(1, &terrainNormalsVBO);
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
}

int main(void) {
    try {
        initialize();
        createContext();
        mainLoop();
        free();
    }
    catch (exception& ex) {
        cout << ex.what() << endl;
        getchar();
        free();
        return -1;
    }

    return 0;
}
void pollKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Task 2.1:
    

    if (key == GLFW_KEY_GRAVE_ACCENT && action == GLFW_PRESS) {
        if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            camera->active = true;
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            camera->active = false;
        }

    }
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        pause = !pause;
        if (!pause) {
            build = false;
        }

    }
    if (pause && key == GLFW_KEY_C && action == GLFW_PRESS) {
        index = 0;
        build = true;
    }
    if (pause && build && key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        if (index == 0) {
            index = (int)mage.size()-1;
        }
        else {
            index -= 1;
        }

    }
    if (pause && build && key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        index = (index + 1) % mage.size();
    }
    if (pause && build && key == GLFW_KEY_1 && action == GLFW_PRESS) {
        mage[index] = new Tower(mat4 (1), magetowermodel, 'm', tower_pos[index], rotate(mat4(), 3.14f, vec3(0.0f, 1.0f, 0.0f)), obj_particle);

    }
    if (pause && build && key == GLFW_KEY_2 && action == GLFW_PRESS) {
        mage[index] = new Tower(mat4(1), archertowermodel, 'a', tower_pos[index], rotate(mat4(), 3.14f, vec3(0.0f, 1.0f, 0.0f)), obj_particle);
    }
}

void attack_handler(vector <Enemy*>& orcs, vector <Tower*>& mage) {//who is attacking who and makes sure orcs don't collide
    
    for (int i = 0; i < mage.size(); i++) {//when the tower is attacking
        if (mage[i]->attacking) {
            if (mage[i]->target_id->current_hp < 0.1f) {
                //cout << mage[i]->target_id->current_hp << endl;
                for (int j = 0; j < mage[i]->target_id->hit_by.size(); j++) {
                    mage[i]->target_id->hit_by[j]->attacking = false;
                    
                }
                mage[i]->attacking = false;
                //mage[i]->target_id->dead = true;
                /*if (find(orcs.begin(), orcs.end(), mage[i]->target_id) <= orcs.end() && find(orcs.begin(), orcs.end(), mage[i]->target_id) - orcs.begin()<orcs.size()) {
                    orcs.erase(find(orcs.begin(), orcs.end(), mage[i]->target_id));
                }*/
                
            }

            else if (length(vec3(mage[i]->target_id->x.x, 0, mage[i]->target_id->x.z) - vec3(mage[i]->x.x, 0, mage[i]->x.z)) > mage[i]->range) {
                mage[i]->attacking = false;
                //cout << -1 << endl;<
                if (find(mage[i]->target_id->hit_by.begin(), mage[i]->target_id->hit_by.end(), mage[i]) - mage[i]->target_id->hit_by.begin() < mage[i]->target_id->hit_by.size())
                    mage[i]->target_id->hit_by.erase(find(mage[i]->target_id->hit_by.begin(), mage[i]->target_id->hit_by.end(), mage[i]));
                  
                    
            }
            else if (((mage[i]->cooldown > 3.0f && mage[i]->the_type == 'm')|| (mage[i]->cooldown > 2.0f && mage[i]->the_type == 'a')) && mage[i]->current_hp > 0.1f) {//&& mage[i]->current_hp > 0.1f
                //mage[i]->target_id->current_hp -= mage[i]->damage;
                mage[i]->target_id->hit = true;
                mage[i]->shoot = true;
                //projectile
                if (mage[i]->the_type == 'a') {
                    mage[i]->projectile = new Projectile(mage[i]->towerVP, arrow, mage[i]->x + vec3(0, 5, 0), mage[i]->target_id->x + vec3(0, 0.7, 0), mage[i]->the_type);
                }
                else {
                    mage[i]->projectile = new Projectile(mage[i]->towerVP, obj_particle, mage[i]->x + vec3(0, 5, 0), mage[i]->target_id->x + vec3(0, 0.7, 0), mage[i]->the_type);
                    mage[i]->projectile->trail = new FollowerEmitter(obj_particle, 100, mage[i]->projectile->pos, mage[i]->projectile);
                }
                mage[i]->cooldown = 0.0f;
                /*if (mage[i]->target_id->current_hp < 0.1f) {
                    mage[i]->target_id->current_hp = 0.05f;
                }*/
            }
            
            if (mage[i]->mana_cd > mage[i]->mana_max && mage[i]->current_hp>0.1f && mage[i]->attacking) {
                mage[i]->meteor = new Projectile(mage[i]->towerVP, light_orb, mage[i]->target_id->x + vec3(0, 10, 0), mage[i]->target_id->x, 'b');
                //mage[i]->meteor->trail = new FollowerEmitter(obj_particle, 100, mage[i]->projectile->pos, mage[i]->meteor);
                //cout << i << endl;
                mage[i]->power_attack = true;
                mage[i]->mana_cd = 0.0f;
            }
            
            
        }
        /*if (mage[i]->current_hp < 0.1f) {
            mage[i]->attacking = false;
            mage[i]->power_attack = false;
            mage[i]->shoot = false;
            mage[i]->hit = false;
            mage[i]->target_id->attacking = false;
        }*/
    }
    
    for (int i = 0; i < orcs.size(); i++) {//when the orc moves
        
            for (int j = 0; j < mage.size(); j++) {
                vec3 dist = mage[j]->x - orcs[i]->x;
                if (!mage[j]->attacking && length(dist) < mage[j]->range && orcs[i]->current_hp>0.1f && mage[j]->current_hp>0.1f) { //&& orcs[i]->x.z > mage[j]->x.z - orcs[i]->mov_speed) {
                    mage[j]->attacking = true;
                    mage[j]->target_id = orcs[i];
                    orcs[i]->hit_by.push_back(mage[j]);
                    if (!orcs[i]->attacking) {
                        orcs[i]->target_id = mage[j];
                        orcs[i]->attacking = true;
                        //orcs[i]->v = dist / length(dist) * orcs[i]->mov_speed;
                        orcs[i]->target = mage[j]->x;
                    }
                }
                //cout << mage[j]->meteor->exploding << endl;
                if (mage[j]->power_attack && mage[j]->meteor->exploding){
                    if(mage[j]->meteor->aoe && length(orcs[i]->x - mage[j]->meteor->pos) < 1.5f) {
                        orcs[i]->current_hp -= 50;
                        orcs[i]->meteored = true;
                        
                    }
                }
            }

            /*if (orcs[i]->attacking){
                if(orcs[i]->x.z > mage[j]->x.z - orcs[i]->mov_speed) {

            }*/
            if (orcs[i]->cooldown > 1.0f && orcs[i]->v == vec3(0, 0, 0) && orcs[i]->current_hp >0.1f) {
                //orcs[i]->target_id->current_hp -= orcs[i]->damage;
                orcs[i]->target_id->hit = true;
                orcs[i]->shoot = true;
                orcs[i]->projectile = new Projectile(orcs[i]->enemyVP, light_orb, orcs[i]->x + vec3(0, 1, 0), orcs[i]->target_id->x + vec3(0, 3, 0), orcs[i]->the_type);
                orcs[i]->cooldown = 0.0f;
                if (orcs[i]->target_id->current_hp < 0.1f) {
                    //orcs[i]->target_id->current_hp = 0.01f;
                    //orcs[i]->target_id->destroyed = true;
                    orcs[i]->attacking = false;
                    //orcs[i]->target_id->attacking = false;
                    if(find(orcs[i]->hit_by.begin(), orcs[i]->hit_by.end(), orcs[i]->target_id) < orcs[i]->hit_by.end())
                        orcs[i]->hit_by.erase(find(orcs[i]->hit_by.begin(), orcs[i]->hit_by.end(), orcs[i]->target_id));
                   /* mage.erase(find(mage.begin(), mage.end(), orcs[i]->target_id));*/

                }
            }
        
    }
}

void particle_effects(mat4  V, mat4 P, float currentTime,  float dt, vector <Enemy*>& orcs, vector <Tower*>& mage) {
    glUseProgram(particleShaderProgram);
    glUniformMatrix4fv(part_vMatrix, 1, GL_FALSE, &V[0][0]);
    glUniformMatrix4fv(part_pMatrix, 1, GL_FALSE, &P[0][0]);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(particleColorSampler, 0);
    

    glBindTexture(GL_TEXTURE_2D, blood);
    for (int i = 0; i <orcs.size(); i++) {//orcs blood effect
        if (orcs[i]->particle_effect || orcs[i]->meteored) {
            if (orcs[i]->first_hit || orcs[i]->meteored) {
                orcs[i]->meteored = false;
                orcs[i]->f_emitter = new FountainEmitter(obj_particle, 5, orcs[i]->x + vec3(0, 0, 0), orcs[i]->the_type);
                orcs[i]->first_hit = false;
            }
            orcs[i]->f_emitter->updateParticles(currentTime, dt, camera->position, orcs[i]->x+ vec3(0, 0, 0));
            orcs[i]->f_emitter->renderParticles();
        }
        if ((orcs[i]->f_emitter->number_of_particles == orcs[i]->f_emitter->dead_particles ) || orcs[i]->meteored) {//&& orcs[i]->current_hp>0.1f
            orcs[i]->particle_effect = false;
            orcs[i]->f_emitter = new FountainEmitter(obj_particle, 5, orcs[i]->x + vec3(0, 0, 0), orcs[i]->the_type);
            orcs[i]->meteored = false;
        }
        //projectile
        if (orcs[i]->attacking) {
            Tower* tower = orcs[i]->target_id;
            if (tower->hit && orcs[i]->shoot) {
                if (tower->hitbox.y + tower->x.y > orcs[i]->projectile->pos.y && (tower->x.x + tower->hitbox.x < orcs[i]->projectile->pos.x
                    || tower->x.x - tower->hitbox.x > orcs[i]->projectile->pos.x)) {
                    orcs[i]->projectile->update(dt);
                }

                else {
                    tower->current_hp -= orcs[i]->damage;
                    if (tower->current_hp < 0.1f) {
                        tower->current_hp = 0.05f;
                    }
                    orcs[i]->shoot = false;
                    tower->hit = false;
                    tower->particle_effect = true;
                }
            }

        }
    }
    glBindTexture(GL_TEXTURE_2D, debris);
    for (int i = 0; i < mage.size(); i++) {//tower debris effect
        if (mage[i]->particle_effect) {
            mage[i]->f_emitter->updateParticles(currentTime, dt, camera->position, mage[i]->x+ vec3(0, 2.8f, 0));
            mage[i]->f_emitter->renderParticles();
        }
        if (mage[i]->f_emitter->number_of_particles == mage[i]->f_emitter->dead_particles) {
            mage[i]->particle_effect = false;
            mage[i]->f_emitter = new FountainEmitter(obj_particle, 200, mage[i]->x + vec3(0, 1.8f, 0), mage[i]->the_type);
        }
        //projectile
        if (mage[i]->attacking) {
            Enemy* orc = mage[i]->target_id;
            if (orc->hit && mage[i]->shoot) {
                if (orc->hitbox.y + orc->x.y < mage[i]->projectile->pos.y && (orc->x.x + orc->hitbox.x < mage[i]->projectile->pos.x
                    || orc->x.x - orc->hitbox.x > mage[i]->projectile->pos.x)) {
                    mage[i]->projectile->update(dt);
                    if (mage[i]->the_type=='m') {
                        glUniform1i(glGetUniformLocation(particleShaderProgram, "trail"), 1);
                        //glUniform1f(glGetUniformLocation(particleShaderProgram, "life"), 1)
                        mage[i]->projectile->trail->updateParticles(0, dt, camera->position, vec3(0,0,0));
                        mage[i]->projectile->trail->renderParticles();
                        glUniform1i(glGetUniformLocation(particleShaderProgram, "trail"), 0);
                    }
                }

                else {
                    orc->current_hp -= mage[i]->damage;
                    if (orc->current_hp < 0.1f) {
                        orc->current_hp = 0.05f;
                    }
                    mage[i]->shoot = false;
                    orc->hit = false;
                    orc->particle_effect = true;
                    /*if (mage[i]->destroyed) {
                        mage.erase(find(mage.begin(), mage.end(), mage[i]));
                    }*/
                }
            }
            if (mage[i]->power_attack) {
                if (mage[i]->meteor->pos.y > 0.0f) {
                    mage[i]->meteor->update(dt);
                }
                else if(!mage[i]->meteor->exploding) {
                    mage[i]->meteor->exploding = true;
                    mage[i]->meteor->explosion = new ExplosionEmitter(obj_particle, 10000, vec3(mage[i]->meteor->pos.x, 0, mage[i]->meteor->pos.z));
                    mage[i]->meteor->aoe = true;
                }
                else {
                    mage[i]->meteor->aoe = false;
                    glUniform1i(glGetUniformLocation(particleShaderProgram, "explosion"), 1);
                    mage[i]->meteor->explosion->updateParticles(dt, dt, camera->position, vec3(0, 0, 0));
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(0, debris);
                    glUniform1i(particleColorSampler, 0);
                    mage[i]->meteor->explosion->renderParticles();
                    glUniform1i(glGetUniformLocation(particleShaderProgram, "explosion"), 0);
                    if (mage[i]->meteor->explosion->active_particles== mage[i]->meteor->explosion->dead_particles) {
                        mage[i]->meteor->exploding = false;
                        mage[i]->power_attack = false;
                    }

                }
            }
            
        }
    }
}
