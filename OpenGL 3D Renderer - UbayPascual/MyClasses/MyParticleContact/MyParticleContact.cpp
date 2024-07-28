#include "MyParticleContact.hpp"

using namespace MyPhysics;

//* ╔═══════════════════════════════╗
//* ║ Constructors & Deconstructors ║
//* ╚═══════════════════════════════╝
MyParticleContact::MyParticleContact(MyParticle* particleA,
                                     MyParticle* particleB,
                                     double restitition,
                                     double depth,
                                     MyVector3 contactNormal)
    : particleA(particleA),
      particleB(particleB),
      restitition(restitition),
      depth(depth),
      contactNormal(contactNormal) {}

//* ╔═════════╗
//* ║ Methods ║
//* ╚═════════╝
double MyParticleContact::calculateSeparatingSpeed() {
    MyVector3 deltaVelocity = this->particleA->getVelocity() - this->particleB->getVelocity();
    return deltaVelocity.DotProduct(this->contactNormal);
}
void MyParticleContact::resolveVelocityWithoutConsiderationToMass(double time) {
    double separatingSpeed = this->calculateSeparatingSpeed();

    if (separatingSpeed > 0) return;
    double newSeparatingSpeed = -(this->restitition * separatingSpeed);
    double deltaSpeed         = newSeparatingSpeed - separatingSpeed;
    MyVector3 impulse         = this->contactNormal * deltaSpeed;
    this->particleA->setVelocity(this->particleA->getVelocity() + impulse);
    this->particleB->setVelocity(this->particleB->getVelocity() - impulse);
}
void MyParticleContact::resolveVelocityWithConsiderationToMass(double time) {
    double separatingSpeed = this->calculateSeparatingSpeed();

    if (separatingSpeed > 0) return;
    double newSeparatingSpeed = -(this->restitition * separatingSpeed);
    double deltaSpeed         = newSeparatingSpeed - separatingSpeed;
    double inverseMassOfA     = 1 / this->particleA->getMass();
    double inverseMassOfB     = 1 / this->particleB->getMass();

    // double totalMass =
    //     (double)1 / this->particleA->getMass() + (double)1 / this->particleB->getMass();
    double totalMass          = inverseMassOfA;
    if (this->particleB) totalMass += inverseMassOfB;

    if (totalMass <= 0) return;
    double impulseMagnitude   = deltaSpeed / totalMass;
    MyVector3 impulse         = this->contactNormal * impulseMagnitude;

    MyVector3 newVelocityForA = impulse * inverseMassOfA;
    this->particleA->setVelocity(this->particleA->getVelocity() + newVelocityForA);
    MyVector3 newVelocityForB = impulse * -inverseMassOfB;
    this->particleB->setVelocity(this->particleB->getVelocity() + newVelocityForB);
}
void MyParticleContact::resolveInterpenetrationWithoutConsiderationToMass(double time) {
    if (this->depth <= 0) return;
    MyVector3 collisionResolution = this->contactNormal * this->depth;
    this->particleA->setPosition(this->particleA->getPosition() + collisionResolution);
    this->particleB->setPosition(this->particleB->getPosition() - collisionResolution);
    this->depth = 0;
}
void MyParticleContact::resolveInterpenetrationWithConsiderationToMass(double time) {
    if (this->depth <= 0) return;

    //* Resolve with consideration of Mass
    double inverseMassOfA = 1 / this->particleA->getMass();
    double inverseMassOfB = 1 / this->particleB->getMass();
    double totalMass      = inverseMassOfA + inverseMassOfB;
    if (totalMass <= 0) return;

    double collisionResolutionByMass = this->depth / totalMass;
    MyVector3 collisionResolution    = this->contactNormal * collisionResolutionByMass;

    MyVector3 particleAResolution    = collisionResolution * inverseMassOfA;
    this->particleA->setPosition(this->particleA->getPosition() + particleAResolution);

    MyVector3 particleBResolution = collisionResolution * -inverseMassOfB;
    this->particleB->setPosition(this->particleB->getPosition() + particleBResolution);

    this->depth = 0;
}
void MyParticleContact::resolve(double time) {
    if (isRod) {
        this->resolveInterpenetrationWithoutConsiderationToMass(time);
        this->resolveVelocityWithoutConsiderationToMass(time);
    } else {
        this->resolveInterpenetrationWithConsiderationToMass(time);
        this->resolveVelocityWithConsiderationToMass(time);
    }
}

//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝
double MyParticleContact::getDepth() { return this->depth; }
void MyParticleContact::setDepth(double depth) { this->depth = depth; }
bool MyParticleContact::getIsRod() { return this->isRod; }
void MyParticleContact::setIsRod(bool isRod) { this->isRod = isRod; }
double MyParticleContact::getRestitition() { return this->restitition; }
void MyParticleContact::setRestitition(double restitition) { this->restitition = restitition; }
MyVector3 MyParticleContact::getContactNormal() { return this->contactNormal; }
void MyParticleContact::setContactNormal(MyVector3 contactNormal) {
    this->contactNormal = contactNormal;
}