#include "MyVector2.hpp"

using namespace MyPhysics;
using namespace std;

//* ╔═══════════════════════════════╗
//* ║ Constructors & Deconstructors ║
//* ╚═══════════════════════════════╝
MyVector2::MyVector2() : x(0), y(0){};
MyVector2::MyVector2(double xy) : x(xy), y(xy){};
MyVector2::MyVector2(double x, double y) : x(x), y(y){};
MyVector2::MyVector2(glm::vec2 xy) : x(xy.x), y(xy.y) {}
//* ╔═════════╗
//* ║ Methods ║
//* ╚═════════╝
void MyVector2::DisplayValuesIndividually(std::string message, int precision) {
    cout << fixed << setprecision(precision) << message << " -> " << "X: " << this->x
         << ", Y: " << this->y << endl;
}
void MyVector2::DisplayValues(int precision) {
    cout << fixed << setprecision(precision) << "(" << this->x << ", " << this->y << ")";
}

//? Vector Operations
MyVector2 MyVector2::operator+(const MyVector2 addend) {
    return MyVector2(this->x + addend.x, this->y + addend.y);
}
void MyVector2::operator+=(const MyVector2 addend) {
    this->x += addend.x;
    this->y += addend.y;
}
MyVector2 MyVector2::operator-(const MyVector2 subtrahend) {
    return MyVector2(this->x - subtrahend.x, this->y - subtrahend.y);
}
void MyVector2::operator-=(const MyVector2 subtrahend) {
    this->x -= subtrahend.x;
    this->y -= subtrahend.y;
}
MyVector2 MyVector2::operator*(const double multiplicand) {
    return MyVector2(this->x * multiplicand, this->y * multiplicand);
}
void MyVector2::operator*=(const double multiplicand) {
    this->x *= multiplicand;
    this->y *= multiplicand;
}
MyVector2 MyVector2::operator*(const MyVector2 multiplicand) {
    return MyVector2(this->x * multiplicand.x, this->y * multiplicand.y);
}
void MyVector2::operator*=(const MyVector2 multiplicand) {
    this->x *= multiplicand.x;
    this->y *= multiplicand.y;
}
double MyVector2::DotMultiplication(MyVector2 multiplicand) {
    return (this->x * multiplicand.x) + (this->y * multiplicand.y);
}
MyVector2 MyVector2::operator/(const double dividend) {
    if (dividend != 0) {
        return MyVector2(this->x / dividend, this->y / dividend);
    } else {
        if (DEBUG_MODE_MATH) std::cout << "- - - - - ERROR: DIVIDING BY 0! - - - - -" << std::endl;
        return MyVector2();
    }
}
void MyVector2::operator/=(const double dividend) {
    if (dividend != 0) {
        this->x /= dividend;
        this->y /= dividend;
    } else {
        if (DEBUG_MODE_MATH) std::cout << "- - - - - ERROR: DIVIDING BY 0! - - - - -" << std::endl;
    }
}
MyVector2 MyVector2::operator/(const MyVector2 dividend) {
    if (dividend.x != 0 && dividend.y != 0) {
        return MyVector2(this->x / dividend.x, this->y / dividend.y);
    } else {
        if (DEBUG_MODE_MATH) std::cout << "- - - - - ERROR: DIVIDING BY 0! - - - - -" << std::endl;
        return MyVector2();
    }
}
void MyVector2::operator/=(const MyVector2 dividend) {
    if (dividend.x != 0 && dividend.y != 0) {
        this->x /= dividend.x;
        this->y /= dividend.y;
    } else {
        if (DEBUG_MODE_MATH) std::cout << "- - - - - ERROR: DIVIDING BY 0! - - - - -" << std::endl;
    }
}
double MyVector2::getMagnitude() { return sqrt(pow(this->x, 2) + pow(this->y, 2)); }
double MyVector2::getSquareMagnitude() { return pow(this->x, 2) + pow(this->y, 2 ); }
MyVector2 MyVector2::getNormalized() {
    return MyVector2(this->x / this->getMagnitude(), this->y / this->getMagnitude());
}
//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝
double MyVector2::getX() { return this->x; }
void MyVector2::setX(double x) { this->x = x; }
double MyVector2::getY() { return this->y; }
void MyVector2::setY(double y) { this->y = y; }
