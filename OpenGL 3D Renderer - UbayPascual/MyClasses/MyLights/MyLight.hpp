#pragma once
#include "stdafx.h"

namespace rendering {
class MyLight {
    //* ╔════════════╗
    //* ║ Attributes ║
    //* ╚════════════╝
public:
    std::string name;
    bool enabled;
    glm::vec3 color;
    float ambientStrength;
    glm::vec3 ambientColor;
    float specularStrength;
    float specularPhong;
    float brightness;

    //* ╔═══════════════════════════════╗
    //* ║ Constructors & Deconstructors ║
    //* ╚═══════════════════════════════╝
public:
    MyLight(std::string name,
            bool enabled,
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
    virtual void update(bool enabled,
                        std::vector<glm::vec3> vectorUpdates,
                        std::vector<float> floatUpdates) = 0;

    //* ╔═══════════════════╗
    //* ║ Getters & Setters ║
    //* ╚═══════════════════╝
public:
    std::string getName();
    bool getEnabled();
    glm::vec3 getColor();
    glm::vec3 getAmbientColor();
    float getBrightness();
};
}  // namespace rendering