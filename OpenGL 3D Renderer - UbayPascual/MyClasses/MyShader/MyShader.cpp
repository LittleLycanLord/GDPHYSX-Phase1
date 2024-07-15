#include "MyShader.hpp"

using namespace rendering;
using namespace std;

//* ╔═══════════════════════════════╗
//* ║ Constructors & Deconstructors ║
//* ╚═══════════════════════════════╝
MyShader::MyShader(std::string path, ShaderType type) : path(path), type(type) {}

//* ╔═════════╗
//* ║ Methods ║
//* ╚═════════╝
void MyShader::loadShader() {
    this->file = fstream(this->path);
    this->buffer << this->file.rdbuf();
    this->shaderString = this->buffer.str();
    this->charArray    = this->shaderString.c_str();

    switch (this->type) {
        case ShaderType::FRAGMENT:
            this->shader = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        case ShaderType::VERTEX:
            this->shader = glCreateShader(GL_VERTEX_SHADER);
            break;
    }
    glShaderSource(this->shader, 1, &this->charArray, NULL);
    glCompileShader(this->shader);
}

//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝
GLuint MyShader::getShader() { return this->shader; }
