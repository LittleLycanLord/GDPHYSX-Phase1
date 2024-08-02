#pragma once
#include "../My3DModel/My3DModel.hpp"
#include "../MyNormal/MyNormal.hpp"
#include "../MyVectors/MyVector3.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "stdafx.h"

using namespace rendering;
using namespace std;
using namespace glm;

namespace MyPhysics {
class MyParticle {
    //* ╔════════════╗
    //* ║ Attributes ║
    //* ╚════════════╝
protected:
    double mass;         //? in kg
    double radius;       //? in m
    double restitution;  //? in sticky units
    MyVector3 position;  //? in m
    mat4 rotation = mat4(1.0f);
    MyVector3 originalPosition;  //? in m

    double damping        = DEFAULT_DAMPING;  //? [0 -> 1]; the lower the value, the higher the drag
    double angularDamping = DEFAULT_DAMPING;  //? [0 -> 1]; the lower the value, the higher the drag

    double lifetime       = 9999999.0f;  //? in s
    bool isDestroyed      = false;
    bool usesGravity      = false;
    bool hasCollision     = true;
    bool isPersistent     = false;
    bool lockX          = false;
    bool lockY          = false;
    bool lockZ          = false;
    double magnitudeVelocity    = 0.0f;         //? in m/s
    MyVector3 totalVelocity     = MyVector3();  //? in m/s
    MyVector3 averageVelocity   = MyVector3();  //? in m/s
    MyVector3 velocity          = MyVector3();  //? in m/s
    MyVector3 acceleration      = MyVector3();  //? in m/s^2
    MyVector3 testAcceleration  = MyVector3();  //? in m/s^2

    MyVector3 accumulatedForce  = MyVector3();  //? in kg m/s^2
    MyVector3 accumulatedTorque = MyVector3();
    MyVector3 angularVelocity   = MyVector3();

    //* ╔═══════════════════════════════╗
    //* ║ Constructors & Deconstructors ║
    //* ╚═══════════════════════════════╝
public:
    MyParticle();
    MyParticle(vector<double> attributes);
    MyParticle(MyVector3 position);
    MyParticle(vector<double> attributes, MyVector3 position);

    //* ╔═════════╗
    //* ║ Methods ║
    //* ╚═════════╝
protected:
    virtual void updateModel();
    void updatePosition(double time);
    void updateVelocity(double time);
    void updateAverageVelocity(int physicsUpdateCount);
    virtual double momentOfInertia();

public:
    virtual void update(double time, int physicsUpdateCount);
    void moveTowards(MyVector3 target, double magnitudeVelocity);
    void addForce(MyVector3 force);
    void addForceAtPoint(MyVector3 force, MyVector3 point);
    void resetForce();
    void extendLifetime(double amount);
    void stop();
    void destroy();

    //* ╔═══════════════════╗
    //* ║ Getters & Setters ║
    //* ╚═══════════════════╝
public:
    virtual My3DModel* getModel3D();
    double getMass();
    void setMass(double mass);
    double getRadius();
    void setRadius(double radius);
    double getRestitution();
    void setRestitution(double restitution);
    double getDamping();
    void setDamping(double damping);
    double getLifetime();
    void setLifetime(double lifetime);
    bool getIsDestroyed();
    bool getUsesGravity();
    void setUsesGravity(bool usesGravity);
    bool getHasCollision();
    void setHasCollision(bool hasCollision);
    bool getIsPersistent();
    void setIsPersistent(bool isPersistent);
    bool getLockX();
    void setLockX(bool lockX);
    bool getLockY();
    void setLockY(bool lockY);
    bool getLockZ();
    void setLockZ(bool lockZ);
    MyVector3 getPosition();
    void setPosition(MyVector3 position);
    MyVector3 getOriginalPosition();
    void setOriginalPosition(MyVector3 originalPosition, bool updateCurrentPosition);
    void setPosition(double x, double y, double z);
    double getMagnitudeVelocity();
    MyVector3 getAverageVelocity();
    MyVector3 getVelocity();
    void setVelocity(MyVector3 velocity);
    void setVelocity(double x, double y, double z);
    MyVector3 getAcceleration();
    void setAcceleration(MyVector3 acceleration);
    void setAcceleration(double x, double y, double z);
    MyVector3 getTestAcceleration();

    mat4 getRotation();
    void setRotation(mat4 rotation);
    MyVector3 getAccumulatedTorque();
    void setAccumulatedTorque(MyVector3 accumulatedTorque);
    MyVector3 getAngularVelocity();
    void setAngularVelocity(MyVector3 angularVelocity);
};
}  // namespace MyPhysics