#include "MyOrthographicCamera.hpp"

using namespace rendering;

//* ╔═══════════════════════════════╗
//* ║ Constructors & Deconstructors ║
//* ╚═══════════════════════════════╝
MyOrthographicCamera::MyOrthographicCamera(std::string name,
                                           glm::vec3 position,
                                           glm::vec3 viewCenter,
                                           float left,
                                           float right,
                                           float bottom,
                                           float top,
                                           float near,
                                           float far)
    : MyCamera(name, glm::ortho(left, right, bottom, top, near, far), position, viewCenter) {}

//* ╔═════════╗
//* ║ Methods ║
//* ╚═════════╝
void MyOrthographicCamera::OtherInputs(int key, int action, int mods) {}

//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝
// namespace models
