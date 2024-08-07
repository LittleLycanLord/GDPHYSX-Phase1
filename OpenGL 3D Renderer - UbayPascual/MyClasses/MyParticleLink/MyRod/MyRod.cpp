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
                                        direction);
    } else
        return nullptr;
    contact->setIsRod(true);
    contact->setRestitition(0.0f);
    return contact;
}

//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝
