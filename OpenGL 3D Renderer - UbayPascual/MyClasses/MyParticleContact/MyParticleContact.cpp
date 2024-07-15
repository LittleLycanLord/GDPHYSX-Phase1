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
    MyVector3 velocity = this->particleA->getVelocity();
    if (this->particleB) velocity -= this->particleB->getVelocity();
    return velocity.DotMultiplication(this->contactNormal);
}
void MyParticleContact::resolveVelocity(double time) {
    double separatingSpeed = calculateSeparatingSpeed();

    if (separatingSpeed > 0) return;
    double newSeparatingSpeed = -this->restitition * separatingSpeed;
    double deltaSpeed         = newSeparatingSpeed - separatingSpeed;
    double totalMass          = (double)1 / this->particleA->getMass();
    if (this->particleB) totalMass += (double)1 / this->particleB->getMass();

    if (totalMass <= 0) return;
    double impulseMagnitude   = deltaSpeed / totalMass;
    MyVector3 impulse         = this->contactNormal * impulseMagnitude;

    MyVector3 newVelocityForA = impulse * ((double)1 / this->particleA->getMass());
    this->particleA->setVelocity(this->particleA->getVelocity() + newVelocityForA);
    MyVector3 newVelocityForB = impulse * ((double)-1 / this->particleB->getMass());
    this->particleB->setVelocity(this->particleB->getVelocity() + newVelocityForB);
}
void MyParticleContact::resolveInterpenetration(double time) {
    if (this->depth <= 0) return;
    // double totalMass = (double)1 / this->particleA->getMass();
    double totalMass =
        (double)1 / this->particleA->getMass() + (double)1 / this->particleB->getMass();
    // if (this->particleB) totalMass += (double)1 / this->particleB->getMass();

    if (totalMass <= 0) return;
    double collisionResolutionByMass = depth / totalMass;
    MyVector3 collisionResolution    = this->contactNormal * collisionResolutionByMass;

    MyVector3 particleAResolution = collisionResolution * ((double)1 / this->particleA->getMass());
    this->particleA->setPosition(this->particleA->getPosition() + particleAResolution);
    if (this->particleB) {
        MyVector3 particleBResolution =
            collisionResolution * (-(double)1 / this->particleB->getMass());
        this->particleB->setPosition(this->particleB->getPosition() + particleBResolution);
    }
    this->depth = 0;
}
void MyParticleContact::resolve(double time) {
    this->resolveVelocity(time);
    this->resolveInterpenetration(time);
}

//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝
double MyParticleContact::getDepth() { return this->depth; }
void MyParticleContact::setDepth(double depth) { this->depth = depth; }
double MyParticleContact::getRestitition() { return this->restitition; }
void MyParticleContact::setRestitition(double restitition) { this->restitition = restitition; }
MyVector3 MyParticleContact::getContactNormal() { return this->contactNormal; }
void MyParticleContact::setContactNormal(MyVector3 contactNormal) {
    this->contactNormal = contactNormal;
}