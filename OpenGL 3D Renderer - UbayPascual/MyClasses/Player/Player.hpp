#pragma once
#include "../My3DModel/My3DModel.hpp"
#include "../MyCameras/MyCamera.hpp"
#include "../MyCameras/MyOrthographicCamera/MyOrthographicCamera.hpp"
#include "../MyCameras/MyPerspectiveCamera/MyPerspectiveCamera.hpp"
#include "../MyLights/MyDirectionalLight/MyDirectionalLight.hpp"
#include "../MyLights/MyPointLight/MyPointLight.hpp"
#include "../MyLights/MySpotLight/MySpotLight.hpp"
#include "../MyNormal/MyNormal.hpp"
#include "../MyTexture/MyTexture.hpp"
#include "cstdlib"
#include "glm/gtx/transform.hpp"
#include "iomanip"
// // clang-format off
// #include "glad/glad.h"
// #include "GLFW/glfw3.h"
// // clang-format on
#include "stdafx.h"

namespace rendering {
class Player {
    //* ╔════════════╗
    //* ║ Attributes ║
    //* ╚════════════╝
protected:
    My3DModel* model;
    MyPerspectiveCamera* firstPersonView;
    MyPerspectiveCamera* thirdPersonView;
    MyCamera* activeCamera;
    float turnInput;
    float yInput;
    float zInput;
    int lightIntensity;

    //* ╔═══════════════════════════════╗
    //* ║ Constructors & Deconstructors ║
    //* ╚═══════════════════════════════╝
public:
    Player();

    //* ╔═════════╗
    //* ║ Methods ║
    //* ╚═════════╝
public:
    void playerKeyboardInput(int key, int action);
    void playerMouseInput();
    void movePlayer();
    void turnPlayer();
    void resetInputs();
    void haveCamerasFollowModel();

    //* ╔═══════════════════╗
    //* ║ Getters & Setters ║
    //* ╚═══════════════════╝
public:
    My3DModel* getModel();
    MyPerspectiveCamera* getFirstPersonView();
    MyPerspectiveCamera* getThirdPersonView();
    MyCamera* getActiveCamera();
};
}  // namespace rendering