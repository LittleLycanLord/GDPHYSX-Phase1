#include "MyPerspectiveCamera.hpp"

using namespace rendering;

//* ╔═══════════════════════════════╗
//* ║ Constructors & Deconstructors ║
//* ╚═══════════════════════════════╝
MyPerspectiveCamera::MyPerspectiveCamera(std::string name,
                                         glm::vec3 position,
                                         glm::vec3 viewCenter,
                                         float fieldOfView,
                                         float farClippingPlane)
    : MyCamera(name,
               glm::perspective(glm::radians(fieldOfView),
                                float(WINDOW_WIDTH / WINDOW_HEIGHT),
                                0.1f,
                                farClippingPlane),
               position,
               viewCenter) {}

//* ╔═════════╗
//* ║ Methods ║
//* ╚═════════╝
void MyPerspectiveCamera::OtherInputs(int key, int action, int mods) {}

//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝
// namespace models
