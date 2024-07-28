#include "MyCable.hpp"

using namespace MyPhysics;

//* ╔═══════════════════════════════╗
//* ║ Constructors & Deconstructors ║
//* ╚═══════════════════════════════╝
MyCable::MyCable(MyParticle* particleA, MyParticle* particleB, double length)
    : MyRod(particleA, particleB, length) {}

//* ╔═════════╗
//* ║ Methods ║
//* ╚═════════╝
MyParticleContact* MyCable::getContact() {
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
    } else
        return nullptr;
    contact->setIsRod(true);
    contact->setRestitition(0.0f);
    return contact;
}

//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝
