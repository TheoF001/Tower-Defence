#include "tree.h"
#include <glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <glm/gtx/string_cast.hpp>

using namespace glm;

Tree::Tree(Drawable* tree, Drawable* leaves, vec3 position, float angle): treeModel(tree), leavesModel(leaves) {
    #define RAND ((float) rand()) / (float) RAND_MAX
    x = position;
    theta = angle;
    translationMatrix = translate(mat4(), x);
    rotationMatrix = rotate(mat4(), theta, vec3(0,1,0));
    scaleMatrix = scale(mat4(), vec3(1,1,1)/(4.0f+RAND*1.0f));
    modelM = translationMatrix * rotationMatrix * scaleMatrix;
}

