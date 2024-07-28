#include "MyParticleLink.hpp"

using namespace MyPhysics;

//* ╔═══════════════════════════════╗
//* ║ Constructors & Deconstructors ║
//* ╚═══════════════════════════════╝
MyParticleLink::MyParticleLink(MyParticle* particleA, MyParticle* particleB)
    : particleA(particleA),
      particleB(particleB) {}

//* ╔═════════╗
//* ║ Methods ║
//* ╚═════════╝
MyParticleContact* MyParticleLink::getContact() { return nullptr; }
double MyParticleLink::getDistanceBetweenParticles() {
    return MyVector3(this->particleA->getPosition() - this->particleB->getPosition())
        .getMagnitude();
}

//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝
