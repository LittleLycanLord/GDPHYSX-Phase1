#pragma once
#include "chrono"
#include "string"
using namespace std::chrono_literals;

//* - - - - - DEBUG - - - - -
const bool DEBUG_MODE                           = false;
const bool DEBUG_MODE_CAMERA_MOVEMENT           = true;
const bool DEBUG_MODE_MODEL_TINTING             = false;
const bool DEBUG_MODE_MATH                      = true;
const bool DEBUG_MODE_PHYSICS                   = true;
const bool DEBUG_MODE_PHYSICS_TIME              = false;
const bool DEBUG_MODE_PROGRAMMING_CHALLENGE_1   = false;
const int COUT_PRECISION                        = 2;
//* - - - - - END OF DEBUG - - - - -

//* - - - - - RENDERING SETTINGS - - - - -
const int WINDOW_WIDTH                          = 800;
const int WINDOW_HEIGHT                         = 800;
const bool DRAW_SKYBOX                          = false;

const std::string WINDOW_NAME                   = "Quiz Conrad Amadeus Carmelo C. Ubay";

const std::string LIGHTING_FRAGMENT_SHADER_PATH = "Shaders/main.frag";
const std::string LIGHTING_VERTEX_SHADER_PATH   = "Shaders/main.vert";
const std::string SKYBOX_FRAGMENT_SHADER_PATH   = "Shaders/skybox.frag";
const std::string SKYBOX_VERTEX_SHADER_PATH     = "Shaders/skybox.vert";

const std::string SKYBOX_RIGHT_TEXTURE_PATH     = "Assets/Skybox/underwater_rt.png";
const std::string SKYBOX_LEFT_TEXTURE_PATH      = "Assets/Skybox/underwater_lf.png";
const std::string SKYBOX_TOP_TEXTURE_PATH       = "Assets/Skybox/underwater_up.png";
const std::string SKYBOX_BOTTOM_TEXTURE_PATH    = "Assets/Skybox/underwater_dn.png";
const std::string SKYBOX_FRONT_TEXTURE_PATH     = "Assets/Skybox/underwater_ft.png";
const std::string SKYBOX_BACK_TEXTURE_PATH      = "Assets/Skybox/underwater_bk.png";
//* - - - - - END OF ENGINE SETTINGS - - - - -

//* - - - - - PHYSICS SETTINGS - - - - -
const bool ONE_PIXEL_PER_METER                  = true;
const bool POSITIVE_Y_ONLY                      = false;
const bool ZOOM_IN_CENTER                       = true;
const float ZOOM_IN_CENTER_SIZE                 = 50.0f;
const float HEIGHT_OFFSET                       = -20.0f;
const bool ORIGIN_MARKER                        = true;
const bool SHOW_RENDER_LINES                    = true;
const double FRAMERATE                          = 60.0f;
constexpr std::chrono::nanoseconds TIMESTEP     = 16ms;

const double DEFAULT_PARTICLE_SIZE              = 1.0f;

const float DEFAULT_GRAVITY_X                   = 0.0f;
const float DEFAULT_GRAVITY_Y                   = -9.8f;
const float DEFAULT_GRAVITY_Z                   = 0.0f;
const float DEFAULT_DRAG_COEFFICIENT_1          = 0.74f;
const float DEFAULT_DRAG_COEFFICIENT_2          = 0.57f;

const double DEFAULT_DAMPING                    = 0.9f;

const int MAX_CONTACT_RESOLUTIONS_PER_FRAME     = 20;
//* - - - - - END OF PHYSICS SETTINGS - - - - -

//* - - - - - GAME SETTINGS - - - - -
//* - - - - - END OF GAME SETTINGS - - - - -