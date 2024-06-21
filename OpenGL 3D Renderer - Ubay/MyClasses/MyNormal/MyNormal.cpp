#include "MyNormal.hpp"

using namespace rendering;
using namespace std;

//* ╔═══════════════════════════════╗
//* ║ Constructors & Deconstructors ║
//* ╚═══════════════════════════════╝
MyNormal::MyNormal(std::string normalPath) : normalPath(normalPath) {}
//* ╔═════════╗
//* ║ Methods ║
//* ╚═════════╝
void MyNormal::loadNormal() {
    stbi_set_flip_vertically_on_load(true);
    unsigned char* normBytes =
        stbi_load(this->normalPath.c_str(), &this->width, &this->height, &this->colorChannels, 0);

    glGenTextures(1, &this->normal);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, this->normal);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    if (!normBytes) {
        cout << "ERROR: Model normal failed to load" << endl;
        return;
    } else {
        switch (this->colorChannels) {
            case 3:
                glTexImage2D(GL_TEXTURE_2D,
                             1,  //? <- Texture Index
                             GL_RGB,
                             this->width,
                             this->height,
                             0,
                             GL_RGB,
                             GL_UNSIGNED_BYTE,
                             normBytes);
                break;
            case 4:
                glTexImage2D(GL_TEXTURE_2D,
                             1,  //? <- Texture Index
                             GL_RGBA,
                             this->width,
                             this->height,
                             0,
                             GL_RGBA,
                             GL_UNSIGNED_BYTE,
                             normBytes);
                break;
        }
        //? Mipmap: Low resolution versions of our texture for if we view the model from far
        // away,
        // we won't need to load as much detail
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(normBytes);
    }
}

//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝
GLuint MyNormal::getNormal() { return this->normal; }
void MyNormal::setNormal(GLuint normal) { this->normal = normal; }
std::string MyNormal::getNormalPath() { return this->normalPath; }
void MyNormal::setNormalPath(std::string normalPath) { this->normalPath = normalPath; }
int MyNormal::getWidth() { return this->width; }
void MyNormal::setWidth(int width) { this->width = width; }
int MyNormal::getHeight() { return this->height; }
void MyNormal::setHeight(int height) { this->height = height; }
int MyNormal::getColorChannels() { return this->colorChannels; }
void MyNormal::setColorChannels(int colorChannels) { this->colorChannels = colorChannels; }
