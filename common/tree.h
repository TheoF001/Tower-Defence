#ifndef TREE_HPP
#define TREE_HPP

#include <glm/glm.hpp>
#include <iostream>
#include "model.h"

#define RAND ((float) rand()) / (float) RAND_MAX

using namespace glm;

class Tree
{
public:
    Drawable* treeModel;
    Drawable* leavesModel;
    vec3 x;
    float theta;
    mat4 translationMatrix;
    mat4 rotationMatrix;
    mat4 scaleMatrix;
    mat4 modelM;
    Tree(Drawable* tree, Drawable* leaves, vec3 position, float angle);
    



};

#endif