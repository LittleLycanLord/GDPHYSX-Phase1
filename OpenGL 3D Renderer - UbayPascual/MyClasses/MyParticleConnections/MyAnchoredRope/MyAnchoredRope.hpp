#pragma once
#include "../../My3DModel/My3DModel.hpp"
#include "../../MyParticle/MyRenderParticle/MyRenderParticle.hpp"
#include "../../MySprings/MyAnchoredSpring/MyAnchoredSpring.hpp"
#include "../../MySprings/MyParticleSpring/MyParticleSpring.hpp"
#include "../../MyVectors/MyVector3.hpp"
#include "stdafx.h"

namespace MyPhysics {

using namespace std;
using namespace rendering;

class MyAnchoredRope {
    //* ╔════════════╗
    //* ║ Attributes ║
    //* ╚════════════╝
protected:
    My3DModel* model;
    MyVector3 tint;
    MyVector3 anchorPoint;
    unsigned int segmentCount;
    double springConstant;
    double segmentRestLength;
    double totalRestLength;
    bool usesGravity;

    MyAnchoredSpring* headSegment;
    vector<MyParticle*> particles;
    vector<MyParticleSpring*> segments;
    //* ╔═══════════════════════════════╗
    //* ║ Constructors & Deconstructors ║
    //* ╚═══════════════════════════════╝
public:
    //? Unknown totalMy3DModel* model, rope length (Easily solvable)
    // MyAnchoredRope(MyVector3 anchorPoint,
    //                double springConstant,
    //                unsigned int segmentCount,
    //                double segmentRestLength);
    //? Unknown rope segment count
    MyAnchoredRope(My3DModel* model,
                   MyVector3 tint,
                   MyVector3 anchorPoint,
                   double springConstant,
                   double segmentRestLength,
                   double totalRestLength,
                   bool usesGravity);
    //? Unknown segment rope length
    MyAnchoredRope(My3DModel* model,
                   MyVector3 tint,
                   MyVector3 anchorPoint,
                   double springConstant,
                   unsigned int segmentCount,
                   double totalRestLength,
                   bool usesGravity);

    //* ╔═════════╗
    //* ║ Methods ║
    //* ╚═════════╝
public:
    void addParticle(MyParticle* particle);
    void addSegments(vector<MyParticleSpring*> springs);
    void setHeadSegment(MyAnchoredSpring* anchoredSpring);

private:
    //* ╔═══════════════════╗
    //* ║ Getters & Setters ║
    //* ╚═══════════════════╝
public:
    My3DModel* getModel();
    MyVector3 getTint();
    MyVector3 getAnchorPoint();
    unsigned int getSegmentCount();
    double getSpringConstant();
    double getSegmentRestLength();
    bool getUsesGravity();
    
    MyAnchoredSpring* getHeadSegment();
    vector<MyParticle*> getParticles();
    vector<MyParticleSpring*> getSegments();
    
};
}  // namespace MyPhysics