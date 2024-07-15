#include "MyParticleSpring.hpp"

using namespace MyPhysics;

//* ╔═══════════════════════════════╗
//* ║ Constructors & Deconstructors ║
//* ╚═══════════════════════════════╝
MyParticleSpring::MyParticleSpring(MyParticle* otherParticle,
                                   double springConstant,
                                   double restLength)
    : otherParticle(otherParticle),
      springConstant(springConstant),
      restLength(restLength) {}
//* ╔═════════╗
//* ║ Methods ║
//* ╚═════════╝
void MyParticleSpring::updateForce(MyParticle* targetParticle, double time) {
    MyVector3 position = targetParticle->getPosition();
    MyVector3 force    = position - this->otherParticle->getPosition();
    double magnitude   = force.getMagnitude();

    double springForce = -this->springConstant * abs(magnitude - this->restLength);
    targetParticle->addForce(force.getNormalized() * springForce);
}

//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝
