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
void MyAnchoredSpring::updateForce(MyParticle* attachedParticle, double time) {
    //* Hooke's Law: F = -k * x
    // attachedParticle->addForce(
    //     //? -k
    //     (attachedParticle->getPosition() - this->anchorPoint).getNormalized() *
    //     -this->springConstant *
    //     //? x
    //     abs((attachedParticle->getPosition() - this->anchorPoint).getMagnitude() -
    //         this->restLength));

    MyVector3 position = attachedParticle->getPosition();
    MyVector3 force    = position - this->anchorPoint;
    double magnitude   = force.getMagnitude();
    double springForce = abs(magnitude - this->restLength) * -this->springConstant;
    force              = force.getNormalized();
    force              = force * springForce;
    attachedParticle->addForce(force);
}

//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝
