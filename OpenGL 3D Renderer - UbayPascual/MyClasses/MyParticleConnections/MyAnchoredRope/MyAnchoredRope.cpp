#include "MyAnchoredRope.hpp"

using namespace MyPhysics;

//* ╔═══════════════════════════════╗
//* ║ Constructors & Deconstructors ║
//* ╚═══════════════════════════════╝
MyAnchoredRope::MyAnchoredRope(My3DModel* model,
                               MyVector3 tint,
                               MyVector3 anchorPoint,
                               double springConstant,
                               double segmentRestLength,
                               double totalRestLength,
                               bool usesGravity)
    : model(model),
      tint(tint),
      anchorPoint(anchorPoint),
      segmentCount(totalRestLength / segmentRestLength),
      springConstant(springConstant),
      segmentRestLength(segmentRestLength),
      totalRestLength(totalRestLength),
      usesGravity(usesGravity),
      headSegment() {}
MyAnchoredRope::MyAnchoredRope(My3DModel* model,
                               MyVector3 tint,
                               MyVector3 anchorPoint,
                               double springConstant,
                               unsigned int segmentCount,
                               double totalRestLength,
                               bool usesGravity)
    : model(model),
      tint(tint),
      anchorPoint(anchorPoint),
      segmentCount(segmentCount),
      springConstant(springConstant),
      segmentRestLength(totalRestLength / segmentCount),
      totalRestLength(totalRestLength),
      usesGravity(usesGravity),
      headSegment() {}
//* ╔═════════╗
//* ║ Methods ║
//* ╚═════════╝
void MyAnchoredRope::addParticle(MyParticle* particle) { this->particles.push_back(particle); }
void MyAnchoredRope::addSegments(vector<MyParticleSpring*> springs) {
    for (MyParticleSpring* spring : springs) this->segments.push_back(spring);
}
void MyAnchoredRope::setHeadSegment(MyAnchoredSpring* anchoredSpring) {
    this->headSegment = anchoredSpring;
}

//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝
My3DModel* MyAnchoredRope::getModel() { return this->model; }
MyVector3 MyAnchoredRope::getTint() { return this->tint; }
MyVector3 MyAnchoredRope::getAnchorPoint() { return this->anchorPoint; }
unsigned int MyAnchoredRope::getSegmentCount() { return this->segmentCount; }
double MyAnchoredRope::getSpringConstant() { return this->springConstant; }
double MyAnchoredRope::getSegmentRestLength() { return this->segmentRestLength; }
bool MyAnchoredRope::getUsesGravity() { return this->usesGravity; }

MyAnchoredSpring* MyAnchoredRope::getHeadSegment() { return this->headSegment; }
vector<MyParticle*> MyAnchoredRope::getParticles() { return this->particles; }
vector<MyParticleSpring*> MyAnchoredRope::getSegments() { return this->segments; }
