#pragma once
#include "../MyCamera.hpp"
#include "stdafx.h"

namespace rendering {
class MyPerspectiveCamera : public MyCamera {
    //* ╔════════════╗
    //* ║ Attributes ║
    //* ╚════════════╝
private:
    //* ╔═══════════════════════════════╗
    //* ║ Constructors & Deconstructors ║
    //* ╚═══════════════════════════════╝
public:
    MyPerspectiveCamera(std::string name,
                        glm::vec3 position,
                        glm::vec3 viewCenter,
                        float fieldOfView,
                        float farClippingPlane);

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