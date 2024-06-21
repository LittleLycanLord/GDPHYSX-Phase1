#include "MyRenderParticle.hpp"

using namespace MyPhysics;

//* ╔═══════════════════════════════╗
//* ║ Constructors & Deconstructors ║
//* ╚═══════════════════════════════╝
// Default Model with Custom Color
MyRenderParticle::MyRenderParticle(My3DModel* model, MyVector3 tint)
    : MyParticle(),
      model(new My3DModel("DEFAULT PARTICLE",
                          model,
                          glm::vec3(0.0f, 0.0f, 0.0f),
                          glm::mat4(1.0f),
                          glm::vec3(PARTICLE_SCALE),
                          glm::vec3(0.0f))),
      tint(tint) {
    this->model->setTint((glm::vec3)tint);
}
MyRenderParticle::MyRenderParticle(My3DModel* model, double mass, MyVector3 tint)
    : MyParticle(mass),
      model(new My3DModel("DEFAULT PARTICLE",
                          model,
                          glm::vec3(0.0f, 0.0f, 0.0f),
                          glm::mat4(1.0f),
                          glm::vec3(PARTICLE_SCALE),
                          glm::vec3(0.0f))),
      tint(tint) {
    this->model->setTint((glm::vec3)tint);
}
MyRenderParticle::MyRenderParticle(My3DModel* model,
                                   double mass,
                                   MyVector3 position,
                                   MyVector3 tint)
    : MyParticle(mass, position),
      model(new My3DModel("DEFAULT PARTICLE",
                          model,
                          glm::vec3(position),
                          glm::mat4(1.0f),
                          glm::vec3(PARTICLE_SCALE),
                          glm::vec3(0.0f))),
      tint(tint) {
    this->model->setTint((glm::vec3)tint);
}

//* ╔═════════╗
//* ║ Methods ║
//* ╚═════════╝
void MyRenderParticle::updateModel() {
    if (this->isDestroyed) {
        this->model->destroy();
    } else {
        this->model->setPosition(glm::vec3(this->position));
    }
}

//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝
My3DModel* MyRenderParticle::getModel3D() { return this->model; }
MyVector3 MyRenderParticle::getColor() { return this->tint; }
void MyRenderParticle::setColor(MyVector3 tint) { this->tint = tint; }