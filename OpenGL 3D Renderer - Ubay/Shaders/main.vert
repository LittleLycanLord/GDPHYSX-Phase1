#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 vertexNormal;
out vec3 normalCoordinate;
layout(location = 2) in vec2 aTexture;
out vec2 textureCoordinate;
layout(location = 3) in vec3 mTangent;
layout(location = 4) in vec3 mBitangent;
out mat3 TBN;

out vec3 fragmentPosition;

uniform mat4 transform;
uniform mat4 projection;
uniform mat4 view;

void main() {
    // normalCoordinate = mat3(transpose(inverse(transform))) * vertexNormal;
    mat3 modelMatrix = mat3(transpose(inverse(transform)));
    normalCoordinate = modelMatrix * vertexNormal;

    vec3 T = normalize(modelMatrix * mTangent);
    vec3 B = normalize(modelMatrix * mBitangent);
    vec3 N = normalize(normalCoordinate);
    TBN = mat3(T, B, N);

    textureCoordinate = aTexture;

    fragmentPosition = vec3(transform * vec4(aPosition, 1.0f));
    gl_Position = projection * view * transform * vec4(aPosition, 1.0);
}