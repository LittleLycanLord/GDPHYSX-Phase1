#include "MyAnchoredChain.hpp"

using namespace MyPhysics;

//* ╔═══════════════════════════════╗
//* ║ Constructors & Deconstructors ║
//* ╚═══════════════════════════════╝
MyAnchoredChain::MyAnchoredChain(My3DModel* model,
                                 MyVector3 tint,
                                 MyParticle* anchorParticle,
                                 double segmentLength,
                                 double totalLength,
                                 bool usesGravity)
    : model(model),
      tint(tint),
      anchorParticle(anchorParticle),
      segmentCount(totalLength / segmentLength),
      segmentLength(segmentLength),
      totalLength(totalLength),
      usesGravity(usesGravity) {
    this->particles.push_back(this->anchorParticle);
}
MyAnchoredChain::MyAnchoredChain(My3DModel* model,
                                 MyVector3 tint,
                                 MyParticle* anchorParticle,
                                 unsigned int segmentCount,
                                 double totalLength,
                                 bool usesGravity)
    : model(model),
      tint(tint),
      anchorParticle(anchorParticle),
      segmentCount(segmentCount),
      segmentLength(totalLength / segmentCount),
      totalLength(totalLength),
      usesGravity(usesGravity) {
    this->particles.push_back(this->anchorParticle);
}
//* ╔═════════╗
//* ║ Methods ║
//* ╚═════════╝
void MyAnchoredChain::addParticle(MyParticle* particle) { this->particles.push_back(particle); }
void MyAnchoredChain::addSegment(MyRod* rod) { this->segments.push_back(rod); }

//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝
My3DModel* MyAnchoredChain::getModel() { return this->model; }
MyVector3 MyAnchoredChain::getTint() { return this->tint; }
MyParticle* MyAnchoredChain::getAnchorParticle() { return this->anchorParticle; }
unsigned int MyAnchoredChain::getSegmentCount() { return this->segmentCount; }
double MyAnchoredChain::getSegmentLength() { return this->segmentLength; }
double MyAnchoredChain::getTotalLength() { return this->totalLength; }
bool MyAnchoredChain::getUsesGravity() { return this->usesGravity; }

vector<MyParticle*> MyAnchoredChain::getParticles() { return this->particles; }
vector<MyRod*> MyAnchoredChain::getSegments() { return this->segments; }
