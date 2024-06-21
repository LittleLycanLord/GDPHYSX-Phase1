#include "My3DModel.hpp"

using namespace rendering;
using namespace std;

//* ╔═══════════════════════════════╗
//* ║ Constructors & Deconstructors ║
//* ╚═══════════════════════════════╝
My3DModel::My3DModel(std::string name,
                     My3DModel* model,
                     glm::vec3 position,
                     glm::mat4 positionMatrix,
                     glm::vec3 scale,
                     glm::vec3 orientation)
    : name(name),
      enabled(true),
      texture(""),
      normalMap(""),
      texturePath(""),
      normalPath(""),
      modelPath(""),
      tint(glm::vec3(1.0f)),
      position(position),
      positionMatrix(positionMatrix),
      scale(scale),
      orientation(orientation),
      fullVertexData(model->getFullVertexData()),
      tangents({}),
      bitangents({}),
      VAO(*model->getVAO()),
      VBO(*model->getVBO()) {}
My3DModel::My3DModel(std::string name,
                     std::string modelPath,
                     std::string texturePath,
                     std::string normalPath,
                     glm::vec3 position,
                     glm::mat4 positionMatrix,
                     glm::vec3 scale,
                     glm::vec3 orientation)
    : isDestroyed(false),
      name(name),
      enabled(true),
      texture(texturePath),
      normalMap(normalPath),
      texturePath(texturePath),
      normalPath(normalPath),
      modelPath(modelPath),
      tint(glm::vec3(1.0f)),
      position(position),
      positionMatrix(positionMatrix),
      scale(scale),
      orientation(orientation),
      fullVertexData({}),
      tangents({}),
      bitangents({}),
      VAO(0),
      VBO(0) {}

My3DModel::My3DModel(std::string name,
                     std::string modelPath,
                     std::string texturePath,
                     std::string normalPath,
                     glm::vec3 tint,
                     glm::vec3 position,
                     glm::mat4 positionMatrix,
                     glm::vec3 scale,
                     glm::vec3 orientation)
    : isDestroyed(false),
      name(name),
      enabled(true),
      texture(texturePath),
      normalMap(normalPath),
      texturePath(texturePath),
      normalPath(normalPath),
      modelPath(modelPath),
      tint(tint),
      position(position),
      positionMatrix(positionMatrix),
      scale(scale),
      orientation(orientation),
      fullVertexData({}),
      tangents({}),
      bitangents({}),
      VAO(0),
      VBO(0) {}

//* ╔═════════╗
//* ║ Methods ║
//* ╚═════════╝
void My3DModel::loadModel() {
    if (this->texturePath != "") this->texture.loadTexture();
    if (this->normalPath != "") this->normalMap.loadNormal();
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;
    tinyobj::attrib_t attributes;

    if (!tinyobj::LoadObj(
            &attributes, &shapes, &material, &warning, &error, this->modelPath.c_str())) {
        cout << "ERROR: Model failed to load" << endl;
        return;
    } else {
        for (int i = 0; i < shapes[0].mesh.indices.size(); i += 3) {
            tinyobj::index_t vData1  = shapes[0].mesh.indices[i];
            tinyobj::index_t vData2  = shapes[0].mesh.indices[i + 1];
            tinyobj::index_t vData3  = shapes[0].mesh.indices[i + 2];

            //* Positions
            glm::vec3 vertex1        = glm::vec3(attributes.vertices[(vData1.vertex_index * 3)],
                                          attributes.vertices[(vData1.vertex_index * 3) + 1],
                                          attributes.vertices[(vData1.vertex_index * 3) + 2]);
            glm::vec3 vertex2        = glm::vec3(attributes.vertices[(vData2.vertex_index * 3)],
                                          attributes.vertices[(vData2.vertex_index * 3) + 1],
                                          attributes.vertices[(vData2.vertex_index * 3) + 2]);
            glm::vec3 vertex3        = glm::vec3(attributes.vertices[(vData3.vertex_index * 3)],
                                          attributes.vertices[(vData3.vertex_index * 3) + 1],
                                          attributes.vertices[(vData3.vertex_index * 3) + 2]);

            //* UVs
            glm::vec2 UV1            = glm::vec2(attributes.texcoords[(vData1.texcoord_index * 2)],
                                      attributes.texcoords[(vData1.texcoord_index * 2) + 1]);
            glm::vec2 UV2            = glm::vec2(attributes.texcoords[(vData2.texcoord_index * 2)],
                                      attributes.texcoords[(vData2.texcoord_index * 2) + 1]);
            glm::vec2 UV3            = glm::vec2(attributes.texcoords[(vData3.texcoord_index * 2)],
                                      attributes.texcoords[(vData3.texcoord_index * 2) + 1]);

            //* Position Deltas
            glm::vec3 deltaPosition1 = vertex2 - vertex1;
            glm::vec3 deltaPosition2 = vertex3 - vertex1;

            //* UV Deltas
            glm::vec2 deltaUV1       = UV2 - UV1;
            glm::vec2 deltaUV2       = UV3 - UV1;

            //* Tangent & Bitangents
            float r             = 1.0f / ((deltaUV1.x * deltaUV2.y) - (deltaUV1.y * deltaUV2.x));
            glm::vec3 tangent   = (deltaPosition1 * deltaUV2.y - deltaPosition2 * deltaUV1.y) * r;
            glm::vec3 bitangent = (deltaPosition2 * deltaUV1.x - deltaPosition1 * deltaUV2.x) * r;

            //* Push to our vectors
            this->tangents.push_back(tangent);
            this->tangents.push_back(tangent);
            this->tangents.push_back(tangent);
            this->bitangents.push_back(bitangent);
            this->bitangents.push_back(bitangent);
            this->bitangents.push_back(bitangent);
        }

        for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
            tinyobj::index_t vData = shapes[0].mesh.indices[i];
            //* Positions
            this->fullVertexData.push_back(attributes.vertices[(vData.vertex_index * 3)]);
            this->fullVertexData.push_back(attributes.vertices[(vData.vertex_index * 3) + 1]);
            this->fullVertexData.push_back(attributes.vertices[(vData.vertex_index * 3) + 2]);
            //* Normals
            this->fullVertexData.push_back(attributes.normals[(vData.normal_index * 3)]);
            this->fullVertexData.push_back(attributes.normals[(vData.normal_index * 3) + 1]);
            this->fullVertexData.push_back(attributes.normals[(vData.normal_index * 3) + 2]);
            //* UVs
            this->fullVertexData.push_back(attributes.texcoords[(vData.texcoord_index * 2)]);
            this->fullVertexData.push_back(attributes.texcoords[(vData.texcoord_index * 2) + 1]);
            //* Tangents
            this->fullVertexData.push_back(tangents[i].x);
            this->fullVertexData.push_back(tangents[i].y);
            this->fullVertexData.push_back(tangents[i].z);
            //* Bitangents
            this->fullVertexData.push_back(bitangents[i].x);
            this->fullVertexData.push_back(bitangents[i].y);
            this->fullVertexData.push_back(bitangents[i].z);
        }
    }

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GLfloat) * this->fullVertexData.size(),
                 this->fullVertexData.data(),
                 GL_DYNAMIC_DRAW);
    //* Positions
    glVertexAttribPointer(
        POSITION_INDEX, 3, GL_FLOAT, GL_FALSE, STRIDE_LENGTH * sizeof(float), (void*)0);
    glEnableVertexAttribArray(POSITION_INDEX);

    //* Normals
    GLintptr normalPtr = 3 * sizeof(float);
    glVertexAttribPointer(
        NORMAL_INDEX, 3, GL_FLOAT, GL_FALSE, STRIDE_LENGTH * sizeof(float), (void*)normalPtr);
    glEnableVertexAttribArray(NORMAL_INDEX);

    //* UVs
    GLintptr uvPtr = 6 * sizeof(float);
    glVertexAttribPointer(
        UV_INDEX, 2, GL_FLOAT, GL_FALSE, STRIDE_LENGTH * sizeof(float), (void*)uvPtr);
    glEnableVertexAttribArray(UV_INDEX);

    //* Tangents
    GLintptr tangentPtr = 8 * sizeof(float);
    glVertexAttribPointer(
        TANGENT_INDEX, 3, GL_FLOAT, GL_FALSE, STRIDE_LENGTH * sizeof(GLfloat), (void*)tangentPtr);
    glEnableVertexAttribArray(TANGENT_INDEX);

    //* Bitangents
    GLintptr bitangentPtr = 11 * sizeof(float);
    glVertexAttribPointer(BITANGENT_INDEX,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          STRIDE_LENGTH * sizeof(GLfloat),
                          (void*)bitangentPtr);
    glEnableVertexAttribArray(BITANGENT_INDEX);

    //* Close for Editing
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
void My3DModel::update() { this->updatePositionMatrix(); }
void My3DModel::updatePositionMatrix() {
    this->positionMatrix = glm::translate(glm::mat4(1.0f), this->position);
    this->positionMatrix = glm::scale(this->positionMatrix, this->scale);
    this->positionMatrix =
        glm::rotate(this->positionMatrix, glm::radians(this->orientation.x), rotateAroundTheXAxis);
    this->positionMatrix =
        glm::rotate(this->positionMatrix, glm::radians(this->orientation.y), rotateAroundTheYAxis);
    this->positionMatrix =
        glm::rotate(this->positionMatrix, glm::radians(this->orientation.z), rotateAroundTheZAxis);
}
void My3DModel::destroy() { this->isDestroyed = true; }

//* ╔═══════════════════╗
//* ║ Getters & Setters ║
//* ╚═══════════════════╝
bool My3DModel::getIsDestroyed() { return this->isDestroyed; }
std::string My3DModel::getName() { return this->name; }
void My3DModel::setName(std::string name) { this->name = name; }
bool My3DModel::getEnabled() { return this->enabled; }
void My3DModel::setEnabled(bool enabled) { this->enabled = enabled; }
MyTexture My3DModel::getTexture() { return this->texture; }
MyNormal My3DModel::getNormalMap() { return this->normalMap; }
glm::vec3 My3DModel::getTint() { return this->tint; }
void My3DModel::setTint(glm::vec3 tint) { this->tint = tint; }
glm::vec3 My3DModel::getPosition() { return this->position; }
void My3DModel::setPosition(glm::vec3 position) { this->position = position; }
glm::mat4 My3DModel::getPositionMatrix() { return this->positionMatrix; }
void My3DModel::setPositionMatrix(glm::mat4 positionMatrix) {
    this->positionMatrix = positionMatrix;
}
glm::vec3 My3DModel::getScale() { return this->scale; }
void My3DModel::setScale(glm::vec3 scale) { this->scale = scale; }
glm::vec3 My3DModel::getOrientation() { return this->orientation; }
void My3DModel::setOrientation(glm::vec3 orientation) { this->orientation = orientation; }
std::vector<GLfloat> My3DModel::getFullVertexData() { return this->fullVertexData; }
GLuint* My3DModel::getVAO() { return &this->VAO; }
GLuint* My3DModel::getVBO() { return &this->VBO; }  // namespace models
