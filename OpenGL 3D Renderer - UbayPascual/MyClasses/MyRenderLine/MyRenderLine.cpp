#include "MyRenderLine.hpp"

using namespace rendering;

//* ╔═══════════════════════════════╗
//* ║ Constructors & Deconstructors ║
//* ╚═══════════════════════════════╝
MyRenderLine::MyRenderLine(MyVector3 positionA, MyVector3 positionB)
    : positionA(positionA),
      positionB(positionB),
      projectionMatrix(mat4(1.0f)),
      color(1.0f) {}
MyRenderLine::MyRenderLine(MyVector3 positionA, MyVector3 positionB, MyVector3 color)
    : positionA(positionA),
      positionB(positionB),
      projectionMatrix(mat4(1.0f)),
      color(color) {}
//* ╔═════════╗
//* ║ Methods ║
//* ╚═════════╝
void MyRenderLine::update(mat4 projectionMatrix) { this->projectionMatrix = projectionMatrix; }
void MyRenderLine::update(MyVector3 positionA, MyVector3 positionB, mat4 projectionMatrix) {
    this->positionA        = positionA;
    this->positionB        = positionB;
    this->projectionMatrix = projectionMatrix;
}
void MyRenderLine::draw() {
    glUseProgram(0);
    vec4 pointA = this->projectionMatrix *
                  vec4(this->positionA.x, this->positionA.y, this->positionA.z, 1.0f);
    vec4 pointB = this->projectionMatrix *
                  vec4(this->positionB.x, this->positionB.y, this->positionB.z, 1.0f);

    glBegin(GL_LINES);
    glVertex3f(pointA.x, pointA.y, pointA.z);
    glVertex3f(pointB.x, pointB.y, pointB.z);
    glEnd();
}

//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝