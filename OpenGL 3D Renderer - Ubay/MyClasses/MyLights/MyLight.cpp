#include "MyLight.hpp"

using namespace rendering;

//* ╔═══════════════════════════════╗
//* ║ Constructors & Deconstructors ║
//* ╚═══════════════════════════════╝
MyLight::MyLight(std::string name,
                 bool enabled,
                 glm::vec3 color,
                 float ambientStrength,
                 glm::vec3 ambientColor,
                 float specularStrength,
                 float specularPhong,
                 float brightness)
    : name(name),
      enabled(enabled),
      color(color),
      ambientStrength(ambientStrength),
      ambientColor(ambientColor),
      specularStrength(specularStrength),
      specularPhong(specularPhong),
      brightness(brightness) {}
//* ╔═════════╗
//* ║ Methods ║
//* ╚═════════╝

//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝
std::string MyLight::getName() { return this->name; }
bool MyLight::getEnabled() { return this->enabled; }
glm::vec3 MyLight::getColor() { return this->color; }
glm::vec3 MyLight::getAmbientColor() { return this->ambientColor; }
float MyLight::getBrightness() { return this->brightness; }
// namespace models
