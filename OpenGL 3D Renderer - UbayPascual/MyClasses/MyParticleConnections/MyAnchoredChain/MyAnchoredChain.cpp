#include "MyAnchoredChain.hpp"

using namespace MyPhysics;

//* ╔═══════════════════════════════╗
//* ║ Constructors & Deconstructors ║
//* ╚═══════════════════════════════╝
MyAnchoredChain::MyAnchoredChain(My3DModel* model,
                                 MyVector3 tint,
                                 MyVector3 anchorPoint,
                                 double segmentLength,
                                 double totalLength,
                                 bool usesGravity)
    : model(model),
      tint(tint),
      anchorPoint(anchorPoint),
      segmentCount(totalLength / segmentLength),
      segmentLength(segmentLength),
      totalLength(totalLength),
      usesGravity(usesGravity),
      headSegment() {}
MyAnchoredChain::MyAnchoredChain(My3DModel* model,
                                 MyVector3 tint,
                                 MyVector3 anchorPoint,
                                 unsigned int segmentCount,
                                 double totalLength,
                                 bool usesGravity)
    : model(model),
      tint(tint),
      anchorPoint(anchorPoint),
      segmentCount(segmentCount),
      segmentLength(totalLength / segmentCount),
      totalLength(totalLength),
      usesGravity(usesGravity),
      headSegment() {}
//* ╔═════════╗
//* ║ Methods ║
//* ╚═════════╝
void MyAnchoredChain::addParticle(MyParticle* particle) { this->particles.push_back(particle); }
void MyAnchoredChain::addSegment(MyRod* rod) { this->segments.push_back(rod); }
void MyAnchoredChain::setHeadSegment(MyRod* headRod) { this->headSegment = headRod; }

//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝
My3DModel* MyAnchoredChain::getModel() { return this->model; }
MyVector3 MyAnchoredChain::getTint() { return this->tint; }
MyVector3 MyAnchoredChain::getAnchorPoint() { return this->anchorPoint; }
unsigned int MyAnchoredChain::getSegmentCount() { return this->segmentCount; }
double MyAnchoredChain::getSegmentLength() { return this->segmentLength; }
double MyAnchoredChain::getTotalLength() { return this->totalLength; }
bool MyAnchoredChain::getUsesGravity() { return this->usesGravity; }

MyRod* MyAnchoredChain::getHeadSegment() { return this->headSegment; }
vector<MyParticle*> MyAnchoredChain::getParticles() { return this->particles; }
vector<MyRod*> MyAnchoredChain::getSegments() { return this->segments; }
