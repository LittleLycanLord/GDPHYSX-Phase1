#include "MyPointLight.hpp"

using namespace rendering;

//* ╔═══════════════════════════════╗
//* ║ Constructors & Deconstructors ║
//* ╚═══════════════════════════════╝
MyPointLight::MyPointLight(std::string name,
                           bool enabled,
                           glm::vec3 position,
                           glm::vec3 color,
                           float ambientStrength,
                           glm::vec3 ambientColor,
                           float specularStrength,
                           float specularPhong,
                           float brightness)
    : MyLight(name,
              enabled,
              color,
              ambientStrength,
              ambientColor,
              specularStrength,
              specularPhong,
              brightness),
      position(position) {}
//* ╔═════════╗
//* ║ Methods ║
//* ╚═════════╝
void MyPointLight::update(bool enabled,
                          std::vector<glm::vec3> vectorUpdates,
                          std::vector<float> floatUpdates) {
    this->color = vectorUpdates[0];
    this->ambientStrength += floatUpdates[0];
    this->ambientColor = vectorUpdates[1];
    this->specularStrength += floatUpdates[1];
    this->specularPhong += floatUpdates[2];
    this->brightness += floatUpdates[3];

    this->position += vectorUpdates[2];

    if (this->brightness < 0.0f) this->brightness = 0.0f;  //? Brightness clamp
}

//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝
glm::vec3 MyPointLight::getPosition() { return this->position; }
// namespace models
