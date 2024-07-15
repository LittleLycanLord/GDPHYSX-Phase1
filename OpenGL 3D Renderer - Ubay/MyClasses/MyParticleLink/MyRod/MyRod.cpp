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
    if (this->getCurrentLength() == this->length) return nullptr;
    MyVector3 direction =
        MyVector3(this->particleB->getPosition() - this->particleA->getPosition()).getNormalized();

    MyParticleContact* contact;

    if (this->getCurrentLength() > this->length) {
        contact = new MyParticleContact(this->particleA,
                                        this->particleB,
                                        this->restitution,
                                        this->getCurrentLength() - this->length,
                                        direction);
    } else {
        contact = new MyParticleContact(this->particleA,
                                        this->particleB,
                                        this->restitution,
                                        this->length - this->getCurrentLength(),
                                        direction * -1);
    }
    contact->setRestitition(this->restitution);
    return contact;
}

//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝
