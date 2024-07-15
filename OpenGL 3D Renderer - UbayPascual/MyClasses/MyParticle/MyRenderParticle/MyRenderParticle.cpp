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
                          glm::vec3(DEFAULT_PARTICLE_SIZE),
                          glm::vec3(0.0f))),
      tint(tint) {
    this->model->setTint((glm::vec3)tint);
}
MyRenderParticle::MyRenderParticle(My3DModel* model, vector<double> attributes, MyVector3 tint)
    : MyParticle(attributes),
      model(new My3DModel("DEFAULT PARTICLE",
                          model,
                          glm::vec3(0.0f, 0.0f, 0.0f),
                          glm::mat4(1.0f),
                          glm::vec3(DEFAULT_PARTICLE_SIZE),
                          glm::vec3(0.0f))),
      tint(tint) {
    this->model->setTint((glm::vec3)tint);
}
MyRenderParticle::MyRenderParticle(My3DModel* model,
                                   vector<double> attributes,
                                   MyVector3 position,
                                   MyVector3 tint)
    : MyParticle(attributes, position),
      model(new My3DModel("DEFAULT PARTICLE",
                          model,
                          glm::vec3(position),
                          glm::mat4(1.0f),
                          glm::vec3(DEFAULT_PARTICLE_SIZE),
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
        this->model->setScale(glm::vec3(this->radius));
        this->model->setPosition(glm::vec3(this->position));
    }
}

//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝
My3DModel* MyRenderParticle::getModel3D() { return this->model; }
MyVector3 MyRenderParticle::getColor() { return this->tint; }
void MyRenderParticle::setColor(MyVector3 tint) { this->tint = tint; }