#include "MyDragGenerator.hpp"

using namespace MyPhysics;

//* ╔═══════════════════════════════╗
//* ║ Constructors & Deconstructors ║
//* ╚═══════════════════════════════╝
MyDragGenerator::MyDragGenerator() {}
MyDragGenerator::MyDragGenerator(double dragCoefficient1, double dragCoefficient2)
    : dragCoefficient1(dragCoefficient1),
      dragCoefficient2(dragCoefficient2) {}
//* ╔═════════╗
//* ║ Methods ║
//* ╚═════════╝
void MyDragGenerator::updateForce(MyParticle* affectedParticle, double time) {
    double magnitude = affectedParticle->getVelocity().getMagnitude();
    if (magnitude <= 0) return;

    double dragForce    = (dragCoefficient1 * magnitude) + (dragCoefficient2 * magnitude);
    MyVector3 direction = affectedParticle->getVelocity().getNormalized();

    affectedParticle->addForce(direction * -dragForce);
}

//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝
