#include "MyParticleSpring.hpp"

using namespace MyPhysics;

//* ╔═══════════════════════════════╗
//* ║ Constructors & Deconstructors ║
//* ╚═══════════════════════════════╝
MyParticleSpring::MyParticleSpring(MyParticle* otherParticle,
                                   double springConstant,
                                   double dampingConstant,
                                   double restLength)
    : otherParticle(otherParticle),
      springConstant(springConstant),
      dampingConstant(dampingConstant),
      restLength(restLength) {}
//* ╔═════════╗
//* ║ Methods ║
//* ╚═════════╝
void MyParticleSpring::updateForce(MyParticle* targetParticle, double time) {
    MyVector3 position = targetParticle->getPosition();
    MyVector3 force    = position - this->otherParticle->getPosition();
    double magnitude   = force.getMagnitude();

    double springForce = -this->springConstant * abs(magnitude - this->restLength);
    MyVector3 springForceVec = force.getNormalized() * springForce;
    //targetParticle->addForce(force.getNormalized() * springForce);

    //Damping Force

    MyVector3 velocity = targetParticle->getVelocity(); // Assuming targetParticle has a getVelocity method
    double damping = -this->dampingConstant * velocity.DotMultiplication(force.getNormalized());
    MyVector3 dampingForceVec = force.getNormalized() * damping;

    MyVector3 totalForce = dampingForceVec + springForceVec;

    targetParticle->addForce(totalForce);

}

//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝
