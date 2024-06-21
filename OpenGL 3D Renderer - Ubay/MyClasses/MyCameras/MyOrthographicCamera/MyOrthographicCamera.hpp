#pragma once
#include "../MyCamera.hpp"
#include "stdafx.h"

namespace rendering {
class MyOrthographicCamera : public MyCamera {
    //* ╔════════════╗
    //* ║ Attributes ║
    //* ╚════════════╝
private:
    //* ╔═══════════════════════════════╗
    //* ║ Constructors & Deconstructors ║
    //* ╚═══════════════════════════════╝
public:
    MyOrthographicCamera(std::string name,
                         glm::vec3 position,
                         glm::vec3 viewCenter,
                         float left,
                         float right,
                         float bottom,
                         float top,
                         float near,
                         float far);

    //* ╔═════════╗
    //* ║ Methods ║
    //* ╚═════════╝
public:
    void OtherInputs(int key, int action, int mods);

    //* ╔═══════════════════╗
    //* ║ Getters & Setters ║
    //* ╚═══════════════════╝
public:
};
}  // namespace rendering