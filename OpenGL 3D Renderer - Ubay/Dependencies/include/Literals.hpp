#pragma once
#include "glm/gtc/matrix_transform.hpp"
#include "string"

//* - - - - - PHYSICS - - - - -
const std::string DEFAULT_PARTICLE_MODEL_PATH   = "Assets/Models/sphere.obj";
const std::string DEFAULT_PARTICLE_TEXTURE_PATH = "Assets/Black.png";
//* - - - - - END OF PHYSICS - - - - -

//* - - - - - ROTATION AXES - - - - -
const glm::vec3 rotateAroundTheXAxis            = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 rotateAroundTheYAxis            = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 rotateAroundTheZAxis            = glm::vec3(0.0f, 0.0f, 1.0f);
//* - - - - - END OF ROTATION AXES - - - - -

//* - - - - - DATA FORMATTING - - - - -
const int STRIDE_LENGTH                         = 14;
const int POSITION_INDEX                        = 0;
const int NORMAL_INDEX                          = 1;
const int UV_INDEX                              = 2;
const int TANGENT_INDEX                         = 3;
const int BITANGENT_INDEX                       = 4;
//* - - - - - END OF DATA FORMATTING - - - - -

//* - - - - - LITERALS - - - - -
const float ROTATE_SPEED                        = 5.0f;  //? In Degrees
const float MOVE_SPEED                          = 0.1f;
const float ORBIT_SPEED                         = MOVE_SPEED * 0.005f;
const float BRIGHTNESS_CHANGE                   = 0.5f;
const float ORBIT_RADIUS                        = 1.5f;
//* - - - - - END OF LITERALS - - - - -