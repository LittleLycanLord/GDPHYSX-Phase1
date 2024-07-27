#include "MyParticleLine.hpp"

//* ╔═══════════════════════════════╗
//* ║ Constructors & Deconstructors ║
//* ╚═══════════════════════════════╝
MyParticleLine::MyParticleLine(MyParticle* particleA, MyParticle* particleB)
    : MyRenderLine(0.0f, 0.0f, 1.0f),
      particleA(particleA),
      particleB(particleB) {}
MyParticleLine::MyParticleLine(MyParticle* particleA, MyParticle* particleB, MyVector3 color)
    : MyRenderLine(0.0f, 0.0f, color),
      particleA(particleA),
      particleB(particleB) {}

//* ╔═════════╗
//* ║ Methods ║
//* ╚═════════╝
void MyParticleLine::update(mat4 projectionMatrix) {
    this->positionA        = this->particleA->getPosition();
    this->positionB        = this->particleB->getPosition();
    this->projectionMatrix = projectionMatrix;
}

//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝