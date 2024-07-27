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
void MyGravityGenerator::updateForce(MyParticle* affectedParticle, double time) {
    if (affectedParticle->getMass() <= 0) return;
    affectedParticle->addForce(this->gravity * affectedParticle->getMass());
}

//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝
