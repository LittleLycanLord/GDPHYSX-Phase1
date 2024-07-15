#include "MyGravityGenerator.hpp"

using namespace MyPhysics;

//* ╔═══════════════════════════════╗
//* ║ Constructors & Deconstructors ║
//* ╚═══════════════════════════════╝
MyGravityGenerator::MyGravityGenerator() {}
MyGravityGenerator::MyGravityGenerator(MyVector3 gravity) : gravity(gravity) {}
//* ╔═════════╗
//* ║ Methods ║
//* ╚═════════╝
void MyGravityGenerator::updateForce(MyParticle* targetParticle, double time) {
    if (targetParticle->getMass() <= 0) return;
    targetParticle->addForce(this->gravity * targetParticle->getMass());
}

//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝
