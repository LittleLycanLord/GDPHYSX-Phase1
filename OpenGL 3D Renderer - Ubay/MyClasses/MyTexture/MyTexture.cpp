#include "MyTexture.hpp"

using namespace rendering;
using namespace std;

//* ╔═══════════════════════════════╗
//* ║ Constructors & Deconstructors ║
//* ╚═══════════════════════════════╝
MyTexture::MyTexture(std::string texturePath) : texturePath(texturePath) {}

//* ╔═════════╗
//* ║ Methods ║
//* ╚═════════╝
void MyTexture::loadTexture() {
    stbi_set_flip_vertically_on_load(true);
    unsigned char* texBytes =
        stbi_load(this->texturePath.c_str(), &this->width, &this->height, &this->colorChannels, 0);

    glGenTextures(1, &this->texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture);

    if (!texBytes) {
        cout << "ERROR: Model texture failed to load" << endl;
        return;
    } else {
        switch (this->colorChannels) {
            case 3:
                glTexImage2D(GL_TEXTURE_2D,
                             0,  //? <- Texture Index
                             GL_RGB,
                             this->width,
                             this->height,
                             0,
                             GL_RGB,
                             GL_UNSIGNED_BYTE,
                             texBytes);
                break;
            case 4:
                glTexImage2D(GL_TEXTURE_2D,
                             0,  //? <- Texture Index
                             GL_RGBA,
                             this->width,
                             this->height,
                             0,
                             GL_RGBA,
                             GL_UNSIGNED_BYTE,
                             texBytes);
                break;
        }
        //? Mipmap: Low resolution versions of our texture for if we view the model from far away,
        // we won't need to load as much detail
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(texBytes);
    }
}

//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝
GLuint MyTexture::getTexture() { return this->texture; }
void MyTexture::setTexture(GLuint texture) { this->texture = texture; }
std::string MyTexture::getTexturePath() { return this->texturePath; }
void MyTexture::setTexturePath(std::string texturePath) { this->texturePath = texturePath; }
int MyTexture::getWidth() { return this->width; }
void MyTexture::setWidth(int width) { this->width = width; }
int MyTexture::getHeight() { return this->height; }
void MyTexture::setHeight(int height) { this->height = height; }
int MyTexture::getColorChannels() { return this->colorChannels; }
void MyTexture::setColorChannels(int colorChannels) { this->colorChannels = colorChannels; }
int MyTexture::getTextureCount() { return 0; }
