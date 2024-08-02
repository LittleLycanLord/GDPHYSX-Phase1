#include "MyRod.hpp"

using namespace MyPhysics;

//* ╔═══════════════════════════════╗
//* ║ Constructors & Deconstructors ║
//* ╚═══════════════════════════════╝
MyRod::MyRod(MyParticle* particleA, MyParticle* particleB, double length)
    : MyParticleLink(particleA, particleB),
      length(length),
      restitution(0.0f) {}

//* ╔═════════╗
//* ║ Methods ║
//* ╚═════════╝
MyParticleContact* MyRod::getContact() {
    if (this->getDistanceBetweenParticles() == this->length) return nullptr;
    MyVector3 direction =
        MyVector3(this->particleB->getPosition() - this->particleA->getPosition()).getNormalized();

    MyVector3 angularDirection = this->particleA->getAngularVelocity().getNormalized();

    //? When one of the particles rotate, the other particle's position has to change similar to an
    //? orbit, with a radius equal to the rod's length, with an axis parallel to the rotating
    //? particle's rotation.

    // quat rotateBy = rotate(mat4(1.0f), (float)angularMagnitude, (vec3)angularDirection);
    // this->particleB->setRotation((toQuat(this->particleB->getRotation()) * rotateBy));

    MyParticleContact* contact;

    if (this->getDistanceBetweenParticles() > this->length) {
        contact = new MyParticleContact(this->particleA,
                                        this->particleB,
                                        0.0f,
                                        this->getDistanceBetweenParticles() - this->length,
                                        direction);
    } else if (this->getDistanceBetweenParticles() < this->length) {
        contact = new MyParticleContact(this->particleA,
                                        this->particleB,
                                        0.0f,
                                        this->length - this->getDistanceBetweenParticles(),
                                        direction * -1);
    } else
        return nullptr;
    contact->setIsRod(false);
    contact->setRestitition(0.0f);
    return contact;
}

//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝
