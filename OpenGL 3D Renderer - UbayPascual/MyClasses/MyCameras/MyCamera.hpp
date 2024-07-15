#pragma once
// clang-format off
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/gtx/rotate_vector.hpp"
// clang-format on
#include "stdafx.h"

namespace rendering {
class MyCamera {
    //* ╔════════════╗
    //* ║ Attributes ║
    //* ╚════════════╝
protected:
    bool useInputs = true;
    std::string name;
    glm::vec3 position;
    glm::vec3 originalPosition;
    glm::mat4 positionMatrix;
    glm::mat4 view;
    glm::mat4 projection;
    glm::vec3 viewCenter;
    glm::vec3 originalViewCenter;
    glm::vec3 moveSpeed;

    //* ╔═══════════════════════════════╗
    //* ║ Constructors & Deconstructors ║
    //* ╚═══════════════════════════════╝
public:
    MyCamera(std::string name, glm::mat4 projection, glm::vec3 position, glm::vec3 viewCenter);

    //* ╔═════════╗
    //* ║ Methods ║
    //* ╚═════════╝
public:
    virtual void CameraMovement(int key, int action, int mods);
    virtual void OtherInputs(int key, int action, int mods) = 0;

    //* ╔═══════════════════╗
    //* ║ Getters & Setters ║
    //* ╚═══════════════════╝
public:
    std::string getName();
    void setName(std::string name);

    glm::vec3 getPosition();
    void setPosition(glm::vec3 position);

    glm::mat4 getPositionMatrix();
    void setPositionMatrix(glm::mat4 positionMatrix);

    glm::mat4 getProjection();
    void setProjection(glm::mat4 projection);

    glm::mat4 getView();
    void setView(glm::mat4 view);

    glm::vec3 getViewCenter();
    void setViewCenter(glm::vec3 viewCenter);
};
}  // namespace rendering