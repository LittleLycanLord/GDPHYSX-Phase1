#include "MyAnchoredSpring.hpp"

using namespace MyPhysics;

//* ╔═══════════════════════════════╗
//* ║ Constructors & Deconstructors ║
//* ╚═══════════════════════════════╝
MyAnchoredSpring::MyAnchoredSpring(MyVector3 anchorPoint, double springConstant, double restLength)
    : anchorPoint(anchorPoint),
      springConstant(springConstant),
      restLength(restLength) {}
//* ╔═════════╗
//* ║ Methods ║
//* ╚═════════╝
void MyAnchoredSpring::updateForce(MyParticle* targetParticle, double time) {
    MyVector3 position = targetParticle->getPosition();
    MyVector3 force    = position - this->anchorPoint;
    double magnitude   = force.getMagnitude();

    double springForce = -this->springConstant * abs(magnitude - this->restLength);
    targetParticle->addForce(force.getNormalized() * springForce);
}

//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝
