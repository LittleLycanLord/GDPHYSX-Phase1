#include "MyVector3.hpp"

using namespace MyPhysics;
using namespace std;

//* ╔═══════════════════════════════╗
//* ║ Constructors & Deconstructors ║
//* ╚═══════════════════════════════╝
MyVector3::MyVector3() : x(0), y(0), z(0){};
MyVector3::MyVector3(double xyz) : x(xyz), y(xyz), z(xyz){};
MyVector3::MyVector3(double x, double y, double z) : x(x), y(y), z(z){};
MyVector3::MyVector3(glm::vec3 xyz) : x(xyz.x), y(xyz.y), z(xyz.z) {}

//* ╔═════════╗
//* ║ Methods ║
//* ╚═════════╝
void MyVector3::DisplayValuesIndividually(std::string message, int precision) {
    cout << fixed << setprecision(precision) << message << " -> " << "X: " << this->x << ", Y: " << this->y
         << ", Z: " << this->z << endl;
}
void MyVector3::DisplayValues(int precision) {
    cout << fixed << setprecision(precision) << "(" << this->x << ", " << this->y << ", " << this->z << ")";
}

//? Vector Operations
MyVector3 MyVector3::operator+(const MyVector3 addend) {
    return MyVector3(this->x + addend.x, this->y + addend.y, this->z + addend.z);
}
void MyVector3::operator+=(const MyVector3 addend) {
    this->x += addend.x;
    this->y += addend.y;
    this->z += addend.z;
}
MyVector3 MyVector3::operator-(const MyVector3 subtrahend) {
    return MyVector3(this->x - subtrahend.x, this->y - subtrahend.y, this->z - subtrahend.z);
}
void MyVector3::operator-=(const MyVector3 subtrahend) {
    this->x -= subtrahend.x;
    this->y -= subtrahend.y;
    this->z -= subtrahend.z;
}
MyVector3 MyVector3::operator*(const double multiplicand) {
    return MyVector3(this->x * multiplicand, this->y * multiplicand, this->z * multiplicand);
}
void MyVector3::operator*=(const double multiplicand) {
    this->x *= multiplicand;
    this->y *= multiplicand;
    this->z *= multiplicand;
}
MyVector3 MyVector3::operator*(const MyVector3 multiplicand) {
    return MyVector3(this->x * multiplicand.x, this->y * multiplicand.y, this->z * multiplicand.z);
}
void MyVector3::operator*=(const MyVector3 multiplicand) {
    this->x *= multiplicand.x;
    this->y *= multiplicand.y;
    this->z *= multiplicand.z;
}
double MyVector3::DotMultiplication(MyVector3 multiplicand) {
    return (this->x * multiplicand.x) + (this->y * multiplicand.y) + (this->z * multiplicand.z);
}
MyVector3 MyVector3::CrossMultiplication(MyVector3 multiplicand) {
    return MyVector3((this->y * multiplicand.z) - (this->z * multiplicand.y),
                     (this->z * multiplicand.x) - (this->x * multiplicand.z),
                     (this->x * multiplicand.y) - (this->y * multiplicand.x));
}
MyVector3 MyVector3::operator/(const double dividend) {
    if (dividend != 0) {
        return MyVector3(this->x / dividend, this->y / dividend, this->z / dividend);
    } else {
        if (DEBUG_MODE_MATH) std::cout << "- - - - - ERROR: DIVIDING BY 0! - - - - -" << std::endl;
        return MyVector3();
    }
}
void MyVector3::operator/=(const double dividend) {
    if (dividend != 0) {
        this->x /= dividend;
        this->y /= dividend;
        this->z /= dividend;
    } else {
        if (DEBUG_MODE_MATH) std::cout << "- - - - - ERROR: DIVIDING BY 0! - - - - -" << std::endl;
    }
}
MyVector3 MyVector3::operator/(const MyVector3 dividend) {
    if (dividend.x != 0 && dividend.y != 0 && dividend.z != 0) {
        return MyVector3(this->x / dividend.x, this->y / dividend.y, this->z / dividend.z);
    } else {
        if (DEBUG_MODE_MATH) std::cout << "- - - - - ERROR: DIVIDING BY 0! - - - - -" << std::endl;
        return MyVector3();
    }
}
void MyVector3::operator/=(const MyVector3 dividend) {
    if (dividend.x != 0 && dividend.y != 0 && dividend.z != 0) {
        this->x /= dividend.x;
        this->y /= dividend.y;
        this->z /= dividend.z;
    } else {
        if (DEBUG_MODE_MATH) std::cout << "- - - - - ERROR: DIVIDING BY 0! - - - - -" << std::endl;
    }
}
double MyVector3::getMagnitude() {
    return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
}
MyVector3 MyVector3::getNormalized() {
    return MyVector3(this->x / this->getMagnitude(),
                     this->y / this->getMagnitude(),
                     this->z / this->getMagnitude());
}
//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝
double MyVector3::getX() { return this->x; }
void MyVector3::setX(double x) { this->x = x; }
double MyVector3::getY() { return this->y; }
void MyVector3::setY(double y) { this->y = y; }
double MyVector3::getZ() { return this->z; }
void MyVector3::setZ(double z) { this->z = z; }
