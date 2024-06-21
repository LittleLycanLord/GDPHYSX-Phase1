#include "Player.hpp"

using namespace rendering;
using namespace std;

//* ╔═══════════════════════════════╗
//* ║ Constructors & Deconstructors ║
//* ╚═══════════════════════════════╝
Player::Player()
    : model(new My3DModel("Player Submarine",           //? Model Name
                          "Assets/MeepballSub.obj",     //? Model Path
                          "Assets/Submarine.png",       //? Texture Path
                          "",                           //? Normal Path
                          glm::vec3(0.0f, 0.0f, 0.0f),  //? Position
                          glm::mat4(1.0f),              //? Position Matrix
                          glm::vec3(1.0f),              //? Scale
                          glm::vec3(0.0f, 0.0f, 0.0f))  //? Orientation
            ),
      firstPersonView(new MyPerspectiveCamera("First Person View",
                                              glm::vec3(0.0f, 0.0f, 0.0f),
                                              glm::vec3(0.0f, 0.0f, -1.0f),
                                              60.0f,
                                              1000.0f)),
      thirdPersonView(new MyPerspectiveCamera("Third Person View",
                                              glm::vec3(0.0f, 5.0f, 5.0f),
                                              glm::vec3(0.0f, 0.0f, -1.0f),
                                              60.0f,
                                              100.0f)),
      turnInput(0.0f),
      yInput(0.0f),
      zInput(0.0f),
      lightIntensity(0),
      activeCamera(this->thirdPersonView) {}

//* ╔═════════╗
//* ║ Methods ║
//* ╚═════════╝
void Player::playerKeyboardInput(int key, int action) {
    switch (key) {
        case GLFW_KEY_W:
            if (action == GLFW_REPEAT) {
            }
            break;
        case GLFW_KEY_A:
            if (action == GLFW_REPEAT) {
            }
            break;
        case GLFW_KEY_S:
            if (action == GLFW_REPEAT) {
            }
            break;
        case GLFW_KEY_D:
            if (action == GLFW_REPEAT) {
            }
            break;
        case GLFW_KEY_Q:
            if (action == GLFW_REPEAT) {
            }
            break;
        case GLFW_KEY_E:
            if (action == GLFW_REPEAT) {
            }
            break;
    }
}
void Player::playerMouseInput() {}
void Player::movePlayer() {
    if (this->yInput != 0) {
        this->model->setPosition(glm::vec3(this->model->getPosition().x,
                                           this->model->getPosition().y + (yInput * MOVE_SPEED),
                                           this->model->getPosition().z));
        if (this->model->getPosition().y >= 0.0f) {
            this->model->setPosition(
                glm::vec3(this->model->getPosition().x, -MOVE_SPEED, this->model->getPosition().z));
        }
    }

    if (this->zInput != 0) {
        float changeInX =
            -(zInput * MOVE_SPEED) * sin(this->model->getOrientation().y * 3.14159265f / 180.0f);
        float changeInZ =
            -(zInput * MOVE_SPEED) * cos(this->model->getOrientation().y * 3.14159265f / 180.0f);

        this->model->setPosition(glm::vec3(this->model->getPosition().x + changeInX,
                                           this->model->getPosition().y,
                                           this->model->getPosition().z + changeInZ));
    }
}
void Player::turnPlayer() {
    if (this->turnInput != 0) {
        this->model->setOrientation(
            glm::vec3(this->model->getOrientation().x,
                      this->model->getOrientation().y + (turnInput * ROTATE_SPEED),
                      this->model->getOrientation().z));
    }
}
void Player::resetInputs() {
    if (this->turnInput != 0) {
        if (this->turnInput > 0)
            this->turnInput -= 0.01f;
        else
            this->turnInput += 0.01f;
    }
    if (this->yInput != 0) {
        if (this->yInput > 0)
            this->yInput -= 0.01f;
        else
            this->yInput += 0.01f;
    }
    if (this->zInput != 0) {
        if (this->zInput > 0)
            this->zInput -= 0.01f;
        else
            this->zInput += 0.01f;
    }
}

void Player::haveCamerasFollowModel() {
    this->firstPersonView->setPosition(this->model->getPosition());
    this->firstPersonView->setViewCenter(glm::vec3(this->model->getPosition().x,
                                                   this->model->getPosition().y,
                                                   this->model->getPosition().z - 5));
    this->thirdPersonView->setPosition(glm::vec3(this->model->getPosition().x,
                                                 this->model->getPosition().y + 3,
                                                 this->model->getPosition().z + 5));
    this->thirdPersonView->setViewCenter(glm::vec3(this->model->getPosition()));
}
//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝
My3DModel* Player::getModel() { return this->model; }
MyPerspectiveCamera* Player::getFirstPersonView() { return this->firstPersonView; }
MyPerspectiveCamera* Player::getThirdPersonView() { return this->thirdPersonView; }
MyCamera* Player::getActiveCamera() { return this->activeCamera; }