#include <glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>

using namespace glm;

Camera::Camera(GLFWwindow* window) : window(window)
{
    position = glm::vec3(5, 25, 5);
    //position = glm::vec3(0, 61.4, 0);//fixed camera
    horizontalAngle = 0.0f;
    //horizontalAngle = 0.0f;
    verticalAngle = 0.0f;
    //verticalAngle = -1.57f;//fixed camera
    FoV = 45.0f;
    //speed = 3.0f;
    speed = 10.0f;
    mouseSpeed = 0.001f;
    fovSpeed = 2.0f;
    tiltSpeed = 20.0f;
    tilt = 0.0f;
}

void Camera::update()
{
    // glfwGetTime is called only once, the first time this function is called
    static double lastTime = glfwGetTime();

    // Compute time difference between current and last frame
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    // Get mouse position
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);

    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // Reset mouse position for next frame
    if (active) {
        glfwSetCursorPos(window, width / 2, height / 2);
    }

    // Task 5.1: simple camera movement that moves in +-z and +-x axes
    
    // Move forward
    //if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    //    position.z -= deltaTime * speed;
    //}
    //// Move backward
    //if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    //    position.z += deltaTime * speed;
    //}
    //// Strafe right
    //if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    //    position.x += deltaTime * speed;
    //}
    //// Strafe left
    //if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    //    position.x -= deltaTime * speed;
    //}

    //// Task 5.2: update view matrix so it always looks at the origin
    //projectionMatrix = perspective(radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
    //viewMatrix = lookAt(
    //    position,
    //    vec3(5, 0, 5),
    //    vec3(0,1,0)
    //);
    ////

    // Task 5.3: Compute new horizontal and vertical angles, given windows size
    
    // and cursor position
    if (active) {
        horizontalAngle += mouseSpeed * (width / 2 - xPos);
        verticalAngle += mouseSpeed * (height / 2 - yPos);
    }

    // Task 5.4: right and up vectors of the camera coordinate system
    // use spherical coordinates
    vec3 direction(
        cos(verticalAngle)*sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle)*cos(horizontalAngle)
    );
    //std::cout << glm::to_string(direction) << std::endl;;
    // Right vector
    vec3 right(
        -cos(horizontalAngle),
        //sin(horizontalAngle),
        0,
        sin(horizontalAngle)
        //cos(horizontalAngle)
    );
    
    mat4 rotation= glm::rotate(mat4(), tilt / 180.0f * 3.14f, vec3(0.0f, 0.0f, 1.0f));;
    // Up vector
    vec3 up = cross(right, direction);
    
    
    // Task 5.5: update camera position using the direction/right vectors
    // Move forward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        position += direction * deltaTime * speed;
    }
    // Move backward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        position -= direction * deltaTime * speed;

    }
    // Strafe right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        position += right * deltaTime * speed;
    }
    // Strafe left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        position -= right * deltaTime * speed;
    }

    // Task 5.6: handle zoom in/out effects
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        FoV += fovSpeed * speed * deltaTime;
        if (FoV > 45.0f) {
            FoV = 45.0f;
        }
      
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        FoV -= fovSpeed * speed * deltaTime;
        if (FoV < 1.0f) {
            FoV = 1.0f;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        position += up * deltaTime * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        position -= up * deltaTime * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        if (tilt < tiltSpeed) {
        position += right * deltaTime * speed;
        tilt += tiltSpeed * deltaTime * speed;
        
        }
        //rotation = glm::rotate(mat4(), tilt / 180.0f * 3.14f, vec3(0.0f, 0.0f, 1.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        if (tilt > -tiltSpeed) {
        position -= right * deltaTime * speed;
        tilt -= tiltSpeed * deltaTime * speed;
        
            
        }
        //rotation = glm::rotate(mat4(), tilt / 180.0f * 3.14f, vec3(0.0f, 0.0f, 1.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_T) != GLFW_PRESS && glfwGetKey(window, GLFW_KEY_R) != GLFW_PRESS && tilt!=0) {
        if (tilt > 0) {
            tilt -= tiltSpeed * deltaTime * speed;
            position -= right * deltaTime * speed;
        }
        if(tilt<0) {
            tilt += tiltSpeed * deltaTime * speed;
            position += right * deltaTime * speed;
        }
        //position = oldp;
        //tilt = 0.0f;
        

    }

    //std::cout << glm::to_string(direction) << std::endl;;
    //std::cout << glm::to_string(position) << std::endl;
    rotation = glm::rotate(mat4(), tilt / 180.0f * 3.14f, vec3(0.0f, 0.0f, 1.0f));
    // Task 5.7: construct projection and view matrices
    projectionMatrix = perspective(radians(FoV), 4.0f / 3.0f, 0.1f, 1000.0f);
    //projectionMatrix = perspective(radians(FoV), 4.0f / 4.0f, 0.1f, 1000.0f);//->use this for the locked camera
    //projectionMatrix = ortho(-25.6f, 25.6f, -25.55f, 25.55f, 1.0f, 102.4f);
    viewMatrix = lookAt(
        position,
        //vec3(5, 25, 10),
        position+direction,
        //vec3(5, 0, 5),
        up
    );
    //*/
    viewMatrix = rotation * viewMatrix;
    // For the next frame, the "last time" will be "now"
    lastTime = currentTime;
}