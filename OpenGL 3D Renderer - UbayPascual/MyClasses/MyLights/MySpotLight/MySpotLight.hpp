#pragma once
#include "../MyLight.hpp"
#include "stdafx.h"

namespace rendering {
class MySpotLight : public MyLight {
    //* ╔════════════╗
    //* ║ Attributes ║
    //* ╚════════════╝
public:
    glm::vec3 position;
    glm::vec3 direction;
    float coneSize;

    //* ╔═══════════════════════════════╗
    //* ║ Constructors & Deconstructors ║
    //* ╚═══════════════════════════════╝
public:
    MySpotLight(std::string name,
                bool enabled,
                glm::vec3 position,
                glm::vec3 direction,
                float coneSize,
                glm::vec3 color,
                float ambientStrength,
                glm::vec3 ambientColor,
                float specularStrength,
                float specularPhong,
                float brightness);

    //* ╔═════════╗
    //* ║ Methods ║
    //* ╚═════════╝
public:
    void update(bool enabled,
                std::vector<glm::vec3> vectorUpdates,
                std::vector<float> floatUpdates);

    //* ╔═══════════════════╗
    //* ║ Getters & Setters ║
    //* ╚═══════════════════╝
public:
    glm::vec3 getPosition();
    void setPosition(glm::vec3 position);
    void setDirection(glm::vec3 direction);
};
}  // namespace rendering