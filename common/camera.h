#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <iostream>

class Camera
{
public:
    GLFWwindow* window;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    // Initial position : on +Z
    glm::vec3 position;
    // Initial horizontal angle : toward -Z
    float horizontalAngle;
    // Initial vertical angle : none
    float verticalAngle;
    // Field of View
    float FoV;

    float speed; // units / second
    float mouseSpeed;
    float fovSpeed;
    //My additions
    float tiltSpeed;
    float tilt;
    bool active = true;

    Camera(GLFWwindow* window);

    void update();
};

#endif