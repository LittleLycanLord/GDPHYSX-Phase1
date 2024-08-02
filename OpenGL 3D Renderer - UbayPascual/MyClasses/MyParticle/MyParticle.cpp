#include "MyParticle.hpp"

using namespace MyPhysics;

//* ╔═══════════════════════════════╗
//* ║ Constructors & Deconstructors ║
//* ╚═══════════════════════════════╝
MyParticle::MyParticle()
    : mass(1.0f),
      radius(DEFAULT_PARTICLE_SIZE),
      restitution(1.0f),
      position(0.0f),
      originalPosition(0.0f) {}
MyParticle::MyParticle(vector<double> attributes)
    : mass(attributes[0]),
      radius(attributes[1]),
      restitution(attributes[2]),
      position(0.0f),
      originalPosition(0.0f) {}
MyParticle::MyParticle(MyVector3 position)
    : mass(1.0f),
      radius(DEFAULT_PARTICLE_SIZE),
      restitution(1.0f),
      position(position),
      originalPosition(position) {}
MyParticle::MyParticle(vector<double> attributes, MyVector3 position)
    : mass(attributes[0]),
      radius(attributes[1]),
      restitution(attributes[2]),
      position(position),
      originalPosition(position) {}

//* ╔═════════╗
//* ║ Methods ║
//* ╚═════════╝
void MyParticle::updateModel() {}
void MyParticle::updatePosition(double time) {
    MyVector3 initialPosition(this->position);
    this->position =
        this->position + (this->velocity * time) + ((this->acceleration * time * time) / 2.0f);
    MyVector3 angularVelocity  = this->angularVelocity * time;
    double angularMagnitude    = angularVelocity.getMagnitude();
    MyVector3 angularDirection = angularVelocity.getNormalized();

    if (angularMagnitude != 0) {
        quat rotateBy  = rotate(mat4(1.0f), (float)angularMagnitude, (vec3)angularDirection);
        this->rotation = toMat4(toQuat(this->rotation) * rotateBy);
    }

    MyVector3 finalPosition(this->position);
    this->magnitudeVelocity = sqrt(pow((finalPosition.x - initialPosition.x), 2) +
                                   pow((finalPosition.y - initialPosition.y), 2) +
                                   pow((finalPosition.z - initialPosition.z), 2)) *
                              FRAMERATE;
}
void MyParticle::updateVelocity(double time) {
    this->acceleration += this->accumulatedForce * (1 / this->mass);
    this->velocity         = this->velocity + (this->acceleration * time);
    this->velocity         = this->velocity * pow(damping, time);

    double momentOfInertia = this->momentOfInertia();
    this->angularVelocity += this->accumulatedTorque * time * (1.0f / momentOfInertia);
    this->angularVelocity = this->angularVelocity * pow(angularDamping, time);
}
void MyParticle::updateAverageVelocity(int physicsUpdateCount) {
    this->totalVelocity += this->velocity;
    this->averageVelocity = this->totalVelocity / physicsUpdateCount;
}
double MyParticle::momentOfInertia() { return (2.0f / 5.0f) * mass * pow(this->radius, 2); }

void MyParticle::update(double time, int physicsUpdateCount) {
    if (isPersistent) {
        if (this->position.x != this->originalPosition.x ||
            this->position.y != this->originalPosition.y ||
            this->position.z != this->originalPosition.z)
            this->position = this->originalPosition;
        this->resetForce();
        this->stop();
        this->updateModel();
        return;
    }
    this->updatePosition(time);
    if (this->lockX && this->position.x != this->originalPosition.x)
        this->position.x = this->originalPosition.x;
    if (this->lockY && this->position.y != this->originalPosition.y)
        this->position.y = this->originalPosition.y;
    if (this->lockZ && this->position.z != this->originalPosition.z)
        this->position.z = this->originalPosition.z;

    this->updateModel();
    this->updateVelocity(time);
    //* - - - - - ADDITIONAL UPDATES - - - - -
    // this->updateAverageVelocity(physicsUpdateCount);
    // if (this->lifetime <= 0.0f) {
    //     this->destroy();
    // } else {
    //     this->lifetime -= time;
    // }
    //* - - - - - END OF ADDITIONAL UPDATES - - - - -
    this->resetForce();
}
void MyParticle::moveTowards(MyVector3 target, double magnitudeVelocity) {
    this->velocity = MyVector3(
        (magnitudeVelocity * (target.x - this->position.x)) /
            sqrt(pow((target.x - this->position.x), 2) + pow((target.y - this->position.y), 2) +
                 pow((target.z - this->position.z), 2)),
        (magnitudeVelocity * (target.y - this->position.y)) /
            sqrt(pow((target.x - this->position.x), 2) + pow((target.y - this->position.y), 2) +
                 pow((target.z - this->position.z), 2)),
        (magnitudeVelocity * (target.z - this->position.z)) /
            sqrt(pow((target.x - this->position.x), 2) + pow((target.y - this->position.y), 2) +
                 pow((target.z - this->position.z), 2)));
}
void MyParticle::addForce(MyVector3 force) { this->accumulatedForce += force; }
void MyParticle::addForceAtPoint(MyVector3 force, MyVector3 point) {
    this->accumulatedForce += force;
    this->accumulatedTorque = point.CrossMultiplication(force);
}
void MyParticle::resetForce() {
    this->accumulatedForce  = MyVector3(0.0f, 0.0f, 0.0f);
    this->testAcceleration  = this->acceleration;
    this->acceleration      = MyVector3(0.0f, 0.0f, 0.0f);
    this->accumulatedTorque = MyVector3(0.0f, 0.0f, 0.0f);
}
void MyParticle::extendLifetime(double amount) { this->lifetime += amount; }
void MyParticle::stop() {
    this->velocity     = MyVector3();
    this->acceleration = MyVector3();
}
void MyParticle::destroy() {
    this->isDestroyed = true;
    this->updateModel();
}

//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝
My3DModel* MyParticle::getModel3D() { return NULL; }
double MyParticle::getMass() { return this->mass; }
void MyParticle::setMass(double mass) { this->mass = mass; }
double MyParticle::getRadius() { return this->radius; }
void MyParticle::setRadius(double radius) { this->radius = radius; }
double MyParticle::getRestitution() { return this->restitution; }
void MyParticle::setRestitution(double restitution) { this->restitution = restitution; }
double MyParticle::getDamping() { return this->damping; }
void MyParticle::setDamping(double damping) { this->damping = damping; }
double MyParticle::getLifetime() { return this->lifetime; }
void MyParticle::setLifetime(double lifetime) { this->lifetime = lifetime; }
bool MyParticle::getIsDestroyed() { return this->isDestroyed; }
bool MyParticle::getUsesGravity() { return this->usesGravity; }
void MyParticle::setUsesGravity(bool usesGravity) { this->usesGravity = usesGravity; }
bool MyParticle::getHasCollision() { return this->hasCollision; }
void MyParticle::setHasCollision(bool hasCollision) { this->hasCollision = hasCollision; }
bool MyParticle::getIsPersistent() { return this->isPersistent; }
void MyParticle::setIsPersistent(bool isPersistent) { this->isPersistent = isPersistent; }
bool MyParticle::getLockX() { return this->lockX; }
void MyParticle::setLockX(bool lockX) { this->lockX = lockX; }
bool MyParticle::getLockY() { return this->lockY; }
void MyParticle::setLockY(bool lockY) { this->lockY = lockY; }
bool MyParticle::getLockZ() { return this->lockZ; }
void MyParticle::setLockZ(bool lockZ) { this->lockZ = lockZ; }
MyVector3 MyParticle::getPosition() { return this->position; }
void MyParticle::setPosition(MyVector3 position) { this->position = position; }
MyVector3 MyParticle::getOriginalPosition() { return this->originalPosition; }
void MyParticle::setOriginalPosition(MyVector3 originalPosition, bool updateCurrentPosition) {
    this->originalPosition = originalPosition;
    if (updateCurrentPosition) this->position = originalPosition;
}
void MyParticle::setPosition(double x, double y, double z) { this->position = MyVector3(x, y, z); }
double MyParticle::getMagnitudeVelocity() { return this->magnitudeVelocity; }
MyVector3 MyParticle::getAverageVelocity() { return this->averageVelocity; }
MyVector3 MyParticle::getVelocity() { return this->velocity; }
void MyParticle::setVelocity(MyVector3 velocity) { this->velocity = velocity; }
void MyParticle::setVelocity(double x, double y, double z) { this->velocity = MyVector3(x, y, z); }
MyVector3 MyParticle::getAcceleration() { return this->acceleration; }
void MyParticle::setAcceleration(MyVector3 acceleration) {
    this->acceleration = acceleration / FRAMERATE;
}
void MyParticle::setAcceleration(double x, double y, double z) {
    this->acceleration = MyVector3(x, y, z);
}
MyVector3 MyParticle::getTestAcceleration() { return this->testAcceleration; }

mat4 MyParticle::getRotation() { return this->rotation; }
void MyParticle::setRotation(mat4 rotation) { this->rotation = rotation; }
MyVector3 MyParticle::getAccumulatedTorque() { return this->accumulatedTorque; }
void MyParticle::setAccumulatedTorque(MyVector3 accumulatedTorque) {
    this->accumulatedTorque = accumulatedTorque;
}
MyVector3 MyParticle::getAngularVelocity() { return this->angularVelocity; }
void MyParticle::setAngularVelocity(MyVector3 angularVelocity) {
    this->angularVelocity = angularVelocity;
}