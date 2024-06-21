#include "MyCamera.hpp"

using namespace rendering;

//* ╔═══════════════════════════════╗
//* ║ Constructors & Deconstructors ║
//* ╚═══════════════════════════════╝
MyCamera::MyCamera(std::string name, glm::mat4 projection, glm::vec3 position, glm::vec3 viewCenter)
    : name(name),
      projection(projection),
      position(position),
      originalPosition(position),
      positionMatrix(glm::translate(glm::mat4(1.0f), position * -1.0f)),
      view(glm::mat4(1.0f)),
      viewCenter(viewCenter),
      originalViewCenter(viewCenter),
      moveSpeed(glm::vec3(1.0f)) {}

//* ╔═════════╗
//* ║ Methods ║
//* ╚═════════╝
void MyCamera::CameraMovement(int key, int action, int mods) {
    if (useInputs) {
        switch (key) {
            //* Reset
            case GLFW_KEY_R:
                if (action == GLFW_PRESS) {
                    this->position   = originalPosition;
                    this->viewCenter = originalViewCenter;
                }
                break;
            // //* Ascend
            // case GLFW_KEY_E:
            //     // if (action == GLFW_PRESS) {
            //     this->position.y += this->moveSpeed.y;
            //     this->viewCenter.y += this->moveSpeed.y;
            //     // }
            //     break;
            // //* Descend
            // case GLFW_KEY_Q:
            //     // if (action == GLFW_PRESS) {
            //     this->position.y -= this->moveSpeed.y;
            //     this->viewCenter.y -= this->moveSpeed.y;
            //     // }
            //     break;
            // //* Forward
            // case GLFW_KEY_W:
            //     // if (action == GLFW_PRESS) {
            //     this->position.z -= this->moveSpeed.z;
            //     this->viewCenter.z -= this->moveSpeed.z;
            //     // }
            //     break;
            // //* Backward
            // case GLFW_KEY_S:
            //     // if (action == GLFW_PRESS) {
            //     this->position.z += this->moveSpeed.z;
            //     this->viewCenter.z += this->moveSpeed.z;
            //     // }
            //     break;
            // //* Left
            // case GLFW_KEY_A:
            //     // if (action == GLFW_PRESS) {
            //     this->position.x -= this->moveSpeed.x;
            //     this->viewCenter.x -= this->moveSpeed.x;
            //     // }
            //     break;
            // //* Right
            // case GLFW_KEY_D:
            //     // if (action == GLFW_PRESS) {
            //     this->position.x += this->moveSpeed.x;
            //     this->viewCenter.x += this->moveSpeed.x;
            //     // }
            //     break;
            //* Orbit X: CW (Pitch)
            // case GLFW_KEY_I:
            case GLFW_KEY_W:
                // if (action == GLFW_PRESS) {
                this->position = glm::rotate(
                    this->position, 0.1f * this->moveSpeed.x, glm::vec3(1.0f, 0.0f, 0.0f));
                // }
                break;
            //* Orbit X: CCW (Pitch)
            // case GLFW_KEY_K:
            case GLFW_KEY_S:
                // if (action == GLFW_PRESS) {
                this->position = glm::rotate(
                    this->position, 0.1f * -this->moveSpeed.x, glm::vec3(1.0f, 0.0f, 0.0f));
                // }
                break;
            //* Orbit Y: CW (Yaw)
            // case GLFW_KEY_J:
            case GLFW_KEY_A:
                // if (action == GLFW_PRESS) {
                this->position = glm::rotate(
                    this->position, 0.1f * this->moveSpeed.y, glm::vec3(0.0f, 1.0f, 0.0f));
                // }
                break;
            //* Orbit Y: CCW (Yaw)
            // case GLFW_KEY_L:
            case GLFW_KEY_D:
                // if (action == GLFW_PRESS) {
                this->position = glm::rotate(
                    this->position, 0.1f * -this->moveSpeed.y, glm::vec3(0.0f, 1.0f, 0.0f));
                // }
                break;
            //* Orbit Z: CW (Roll)
            // case GLFW_KEY_U:
            case GLFW_KEY_Q:
                // if (action == GLFW_PRESS) {
                this->position = glm::rotate(
                    this->position, 0.1f * this->moveSpeed.z, glm::vec3(0.0f, 0.0f, 1.0f));
                // }
                break;
            //* Orbit Z: CCW (Roll)
            // case GLFW_KEY_O:
            case GLFW_KEY_E:
                // if (action == GLFW_PRESS) {
                this->position = glm::rotate(
                    this->position, 0.1f * -this->moveSpeed.z, glm::vec3(0.0f, 0.0f, 1.0f));
                // }
                break;
        }
    }
}

//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝
std::string MyCamera::getName() { return this->name; }
void MyCamera::setName(std::string name) { this->name = name; }

glm::vec3 MyCamera::getPosition() { return this->position; }
void MyCamera::setPosition(glm::vec3 position) { this->position = position; }

glm::mat4 MyCamera::getPositionMatrix() { return this->positionMatrix; }
void MyCamera::setPositionMatrix(glm::mat4 positionMatrix) {
    this->positionMatrix = positionMatrix;
}

glm::mat4 MyCamera::getProjection() { return this->projection; }
void MyCamera::setProjection(glm::mat4 projection) { this->projection = projection; }

glm::mat4 MyCamera::getView() { return this->view; }
void MyCamera::setView(glm::mat4 view) { this->view = view; }

glm::vec3 MyCamera::getViewCenter() { return this->viewCenter; }
void MyCamera::setViewCenter(glm::vec3 viewCenter) { this->viewCenter = viewCenter; }

// namespace models