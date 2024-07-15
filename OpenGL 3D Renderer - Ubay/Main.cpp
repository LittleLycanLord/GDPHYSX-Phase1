// clang-format off
#include "glad/glad.h"
#include "GLFW/glfw3.h"         

#include "MyClasses/My3DModel/My3DModel.hpp"
#include "MyClasses/MySprings/MyAnchoredSpring/MyAnchoredSpring.hpp"
#include "MyClasses/MySprings/MyParticleSpring/MyParticleSpring.hpp"
#include "MyClasses/MyCameras/MyCamera.hpp"
#include "MyClasses/MyCameras/MyOrthographicCamera/MyOrthographicCamera.hpp"
#include "MyClasses/MyCameras/MyPerspectiveCamera/MyPerspectiveCamera.hpp"
#include "MyClasses/MyContactResolver/MyContactResolver.hpp"
#include "MyClasses/MyForceGenerator/MyDragGenerator/MyDragGenerator.hpp"
#include "MyClasses/MyForceGenerator/MyGravityGenerator/MyGravityGenerator.hpp"
#include "MyClasses/MyForceRegistry/MyForceRegistry.hpp"
#include "MyClasses/MyLights/MyDirectionalLight/MyDirectionalLight.hpp"
#include "MyClasses/MyLights/MyLight.hpp"
#include "MyClasses/MyLights/MyPointLight/MyPointLight.hpp"
#include "MyClasses/MyLights/MySpotLight/MySpotLight.hpp"
#include "MyClasses/MyNormal/MyNormal.hpp"
#include "MyClasses/MyParticle/MyParticle.hpp"
#include "MyClasses/MyParticle/MyParticleSystem/MyParticleSystem.hpp"
#include "MyClasses/MyParticle/MyRenderParticle/MyRenderParticle.hpp"
#include "MyClasses/MyParticleContact/MyParticleContact.hpp"
#include "MyClasses/MyParticleLink/MyParticleLink.hpp"
#include "MyClasses/MyParticleLink/MyRod/MyRod.hpp"
#include "MyClasses/MyPhysicsWorld/MyPhysicsWorld.hpp"
#include "MyClasses/MyShader/MyShader.hpp"
#include "MyClasses/MyTexture/MyTexture.hpp"
#include "MyClasses/Player/Player.hpp"
#include "stdafx.h"      
// clang-format on    

using namespace std;
using namespace chrono;
using namespace rendering;
using namespace MyPhysics;

//* - - - - - POINTERS AND CONTAINERS - - - - -
vector<MyDirectionalLight*> directionalLights = {};
vector<MyPointLight*> pointLights             = {};
vector<MySpotLight*> spotLights               = {};
vector<MyLight*> lights                       = {};

list<My3DModel*> renderingList                     ={};

vector<MyCamera*> cameras                     = {};
MyCamera* activeCamera                        = NULL;

Player* player                              = NULL;
//* - - - - - END OF POINTERS AND CONTAINERS - - - - -

//* - - - - - PHYSICS - - - - -
using physicsClock                          = high_resolution_clock;
using timerClock                            = steady_clock;
bool pausePhysics = false;
auto timerClockStart                        = timerClock::now();
auto timerClockMark                         = timerClock::now();
nanoseconds currentNanosecond(0);
MyPhysicsWorld physicsWorld = MyPhysicsWorld(&renderingList);
//* - - - - - END OF PHYSICS - - - - -

void updateModelsList() {
    renderingList.remove_if([](My3DModel* model) { return model->getIsDestroyed(); });
}

//? Reserved GLFW Function for Keyboard Inputs
void Key_Callback(
    GLFWwindow* window,  //? Which window did we get the event?
    int key,             //? What key was pressed?
    int scancode,        //? What exact physical key was pressed?
    int action,  //? What is being done to the key? [GLFW_PRESS, GLFW_REPEAT or GLFW_RELEASE]
    int mods) {  //? Which modifer keys are held? [alt, control, shift, Super, num lock, and caps
    //?lock]
    activeCamera->CameraMovement(key, action, mods);
    activeCamera->OtherInputs(key, action, mods);
    switch (key) {
        //* Toggle Physics
        case GLFW_KEY_SPACE:
            if (action == GLFW_PRESS) {
               pausePhysics = !pausePhysics;
            }
            break;
        //* Swap Camera to Orthographic
        case GLFW_KEY_1:
            if (action == GLFW_PRESS) {
                activeCamera = cameras[1];
                system("cls");
               cout << "Current Camera Switched to: " + activeCamera->getName() << endl;
            }
            break;
        //* Swap Camera to Perspective
        case GLFW_KEY_2:
            if (action == GLFW_PRESS) {
                activeCamera = cameras[0];
                system("cls");
               cout << "Current Camera Switched to: " + activeCamera->getName() << endl;
            }
            break;
    }
}

int main(void) {
    //* - - - - - WINDOW CREATION - - - - -
    GLFWwindow* window;
    if (!glfwInit()) return -1;

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME.c_str(), NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSetKeyCallback(window, Key_Callback);
    //* - - - - - END OF WINDOW CREATION - - - - -

    //* - - - - - USER INPUT - - - - -
    //* Fountain Test
    // int sparkCount = 0;
    // cout << "How many sparks to spawn?" << endl;
    // cin >> sparkCount;
    // cout << "Spawning " << sparkCount << " sparks!" << endl;
    //* - - - - - END OF USER INPUT - - - - -

    //* - - - - - PLAYER INITIALIZATION - - - - - 
    player = new Player();
    //* - - - - - END OF PLAYER INITIALIZATION - - - - -

    //* - - - - - SHADER CREATION - - - - -
    MyShader lightingFragmentShader(LIGHTING_FRAGMENT_SHADER_PATH, ShaderType::FRAGMENT);
    lightingFragmentShader.loadShader();
    MyShader lightingVertexShader(LIGHTING_VERTEX_SHADER_PATH, ShaderType::VERTEX);
    lightingVertexShader.loadShader();
    MyShader skyboxFragmentShader(SKYBOX_FRAGMENT_SHADER_PATH, ShaderType::FRAGMENT);
    skyboxFragmentShader.loadShader();
    MyShader skyboxVertexShader(SKYBOX_VERTEX_SHADER_PATH, ShaderType::VERTEX);
    skyboxVertexShader.loadShader();

    GLuint lightingShaderProgram = glCreateProgram();
    glAttachShader(lightingShaderProgram, lightingVertexShader.getShader());
    glAttachShader(lightingShaderProgram, lightingFragmentShader.getShader());

    glLinkProgram(lightingShaderProgram);

    GLuint skyboxShaderProgram = glCreateProgram();
    glAttachShader(skyboxShaderProgram, skyboxVertexShader.getShader());
    glAttachShader(skyboxShaderProgram, skyboxFragmentShader.getShader());

    glLinkProgram(skyboxShaderProgram);
    //* - - - - - END OF SHADER CREATION - - - - -

    //* - - - - - SKYBOX CREATION - - - - -
    //?   7--------6
    //?  /|       /|
    //? 4--------5 |
    //? | |      | |
    //? | 3------|-2
    //? |/       |/
    //? 0--------1

    //* Define our skybox's vertices
    float skyboxVertices[]{
       // clang-format off
       -1.0f, -1.0f,  1.0f, //? 0 - Near Bottom Left Corner
        1.0f, -1.0f,  1.0f, //? 1 - Near Bottom Right Corner
        1.0f, -1.0f, -1.0f, //? 2 - Far Bottom Right Corner
       -1.0f, -1.0f, -1.0f, //? 3 - Far Bottom Left Corner
       -1.0f,  1.0f,  1.0f, //? 4 - Near Top Left Corner
        1.0f,  1.0f,  1.0f, //? 5 - Near Top Right Corner
        1.0f,  1.0f, -1.0f, //? 6 - Far Top Right Corner
       -1.0f,  1.0f, -1.0f  //? 7 - Far Top Left Corner
       // clang-format on
    };

    //* Define our skybox's fragments
    //* Note: Each face consist of two triangles, thus each face is made up of a pair of vertex
    // triplets (cuz triangles have three corners, duh)
    unsigned int skyboxFragments[]{
       // clang-format off
        1, 2, 6, 6, 5, 1, //? Right Face
        0, 4, 7, 7, 3, 0, //? Left Face
        4, 5, 6, 6, 7, 4, //? Top Face
        0, 3, 2, 2, 1, 0, //? Bottom Face
        0, 1, 5, 5, 4, 0, //? Near Face
        3, 7, 6, 6, 2, 3  //? Far Face
       // clang-format on
    };

    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;

    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);

    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GL_INT) * 36, &skyboxFragments, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    //* - - - - - END OF SKYBOX CREATION - - - - -

    //* - - - - - SKYBOX TEXTURING - - - - -
    string skyboxTexturePaths[]{SKYBOX_RIGHT_TEXTURE_PATH,
                                SKYBOX_LEFT_TEXTURE_PATH,
                                SKYBOX_TOP_TEXTURE_PATH,
                                SKYBOX_BOTTOM_TEXTURE_PATH,
                                SKYBOX_FRONT_TEXTURE_PATH,
                                SKYBOX_BACK_TEXTURE_PATH};

    unsigned int skyboxTexture;
    glGenTextures(1, &skyboxTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    for (unsigned int i = 0; i < 6; i++) {
        int w, h, skyCChannel;
        stbi_set_flip_vertically_on_load(false);
        unsigned char* data = stbi_load(skyboxTexturePaths[i].c_str(), &w, &h, &skyCChannel, 0);

        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0,
                         GL_RGBA,
                         w,
                         h,
                         0,
                         GL_RGBA,
                         GL_UNSIGNED_BYTE,
                         data);

            stbi_image_free(data);
        }

        stbi_set_flip_vertically_on_load(true);
    }
    //* - - - - - END OF SKYBOX TEXTURING - - - - -

    //* - - - - - CAMERAS - - - - -
    cameras = {new MyPerspectiveCamera("Perspective Camera",
                                       glm::vec3(0.0f, 0.0f, WINDOW_WIDTH),
                                       glm::vec3(0.0f, 0.0f, 0.0f),
                                       60.0f,
                                       1000.0f)};
    if (ONE_PIXEL_PER_METER) {
        if (POSITIVE_Y_ONLY) {
            cameras.push_back(new MyOrthographicCamera(
                "Orthographic Camera",
                glm::vec3(0.0f, WINDOW_WIDTH / 2 + HEIGHT_OFFSET, WINDOW_WIDTH / 2),
                glm::vec3(0.0f, WINDOW_WIDTH / 2 + HEIGHT_OFFSET, 0.0f),
                -WINDOW_WIDTH / 2,
                WINDOW_WIDTH / 2,
                -WINDOW_WIDTH / 2,
                WINDOW_WIDTH / 2,
                0.001f,
                1000.0f));
        } else {
            if (ZOOM_IN_CENTER) {
                cameras.push_back(
                    new MyOrthographicCamera("Orthographic Camera",
                                             glm::vec3(0.0f, 0.0f, ZOOM_IN_CENTER_SIZE / 2),
                                             glm::vec3(0.0f, 0.0f, 0.0f),
                                             -ZOOM_IN_CENTER_SIZE / 2,
                                             ZOOM_IN_CENTER_SIZE / 2,
                                             -ZOOM_IN_CENTER_SIZE / 2,
                                             ZOOM_IN_CENTER_SIZE / 2,
                                             0.001f,
                                             1000.0f));
            } else {
                cameras.push_back(new MyOrthographicCamera("Orthographic Camera",
                                                           glm::vec3(0.0f, 0.0f, WINDOW_WIDTH / 2),
                                                           glm::vec3(0.0f, 0.0f, 0.0f),
                                                           -WINDOW_WIDTH / 2,
                                                           WINDOW_WIDTH / 2,
                                                           -WINDOW_WIDTH / 2,
                                                           WINDOW_WIDTH / 2,
                                                           0.001f,
                                                           1000.0f));
            }
        }
    }

    activeCamera      = cameras.back();
    //* - - - - - END OF CAMERAS - - - - -

    //* - - - - - WORLD FACTS - - - - -
    glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 Center  = glm::vec3(0.0f);
    glm::vec3 ForwardVector =
        glm::vec3(activeCamera->getViewCenter() - activeCamera->getPosition());
    ForwardVector         = glm::normalize(ForwardVector);
    glm::vec3 RightVector = glm::normalize(glm::cross(ForwardVector, WorldUp));
    glm::vec3 UpVector    = glm::normalize(glm::cross(RightVector, ForwardVector));
    //* - - - - - END OF WORLD FACTS - - - - -

    //* - - - - - LIGHTS - - - - -
    directionalLights     = {
       new MyDirectionalLight("Top White Directional Light",
                              true,
                              glm::vec3(0.0f, -1.0f, 0.0f),  //? Direction
                              glm::vec3(1.0f, 1.0f, 1.0f),   //? Color
                              0.1f,                          //? Ambient Strength
                              glm::vec3(1.0f, 1.0f, 1.0f),   //? Ambient Color
                              0.5f,                          //? Specular Strength
                              16,                            //? Specular Phong
                              5.0f),                         //? Brightness
       new MyDirectionalLight("Bottom White Directional Light",
                              true,
                              glm::vec3(0.0f, 1.0f, 0.0f),  //? Direction
                              glm::vec3(1.0f, 1.0f, 1.0f),  //? Color
                              0.1f,                         //? Ambient Strength
                              glm::vec3(1.0f, 1.0f, 1.0f),  //? Ambient Color
                              0.5f,                         //? Specular Strength
                              16,                           //? Specular Phong
                              5.0f),                        //? Brightness
    };
    for (MyDirectionalLight* directionalLight : directionalLights)
        lights.push_back(directionalLight);

    pointLights = {
       //    new MyPointLight("White Point Light",
       //                   true,
       //                   glm::vec3(0.0f, 0.8f, 0.2f),  //? Position
       //                   glm::vec3(1.0f, 1.0f, 1.0f),  //? Color
       //                   0.1f,                         //? Ambient Strength
       //                   glm::vec3(1.0f, 1.0f, 1.0f),  //? Ambient Color
       //                   0.5f,                         //? Specular Strength
       //                   16,                           //? Specular Phong
       //                   1.2f),                        //? Brightness
    };
    for (MyPointLight* pointLight : pointLights) lights.push_back(pointLight);

    spotLights = {
       //    new SpotLight("White Spotlight",
       //                  false,
       //                  glm::vec3(0.0f, 1.0f, 0.0f),   //? Position
       //                  glm::vec3(0.0f, -1.0f, 0.0f),  //? Direction
       //                  12.5f,                         //? Cone Size
       //                  glm::vec3(1.0f, 1.0f, 1.0f),   //? Color
       //                  0.1f,                          //? Ambient Strength
       //                  glm::vec3(1.0f, 1.0f, 1.0f),   //? Ambient Color
       //                  0.5f,                          //? Specular Strength
       //                  16,                            //? Specular Phong
       //                  1.0f),                         //? Brightness
    };
    for (MySpotLight* spotLight : spotLights) lights.push_back(spotLight);

    //* - - - - - END OF LIGHTS - - - - -

    //* - - - - - MODEL LOADING - - - - -
    renderingList = {};
    for (My3DModel* model : renderingList) {
        model->loadModel();
    }
    //* - - - - - END OF MODEL LOADING - - - - -

    //* - - - - - PARTICLE SETUP - - - - -
    My3DModel* particleModel = new My3DModel("DEFAULT PARTICLE",
                                             DEFAULT_PARTICLE_MODEL_PATH,
                                             DEFAULT_PARTICLE_TEXTURE_PATH,
                                             "",
                                             glm::vec3(1.0f),
                                             glm::vec3(0.0f, 0.0f, 0.0f),
                                             glm::mat4(1.0f),
                                             glm::vec3(DEFAULT_PARTICLE_SIZE),
                                             glm::vec3(0.0f));
    particleModel->loadModel();

    int originParticles = 0;
    if (ORIGIN_MARKER) {
        physicsWorld.addParticles(
            {new MyRenderParticle(particleModel, MyVector3(1.0f, 0.0f, 0.0f)),
             new MyRenderParticle(particleModel, MyVector3(0.0f, 1.0f, 0.0f)),
             new MyRenderParticle(particleModel, MyVector3(0.0f, 0.0f, 1.0f)),
             new MyRenderParticle(particleModel, MyVector3(1.0f, 1.0f, 1.0f))});
        physicsWorld.getParticleListAsVector()[0]->setPosition(MyVector3(100.0f, 0.0f, 0.0f));
        physicsWorld.getParticleListAsVector()[1]->setPosition(MyVector3(0.0f, 100.0f, 0.0f));
        physicsWorld.getParticleListAsVector()[2]->setPosition(MyVector3(0.0f, 0.0f, 100.0f));
        originParticles = 4;
    }
    //* - - - - - END OF PARTICLE SETUP - - - - -

    //* - - - - - PARTICLES - - - - -
    physicsWorld.addParticles({
       //? Particle System Test
       // new MyParticleSystem(
       // particleModel, MyVector3(0.0f, 0.0f, 0.0f), 10.0f, sparkCount, &physicsWorld)
       //? Contact Resolver Test
    //    new MyRenderParticle(particleModel, MyVector3(1.0f, 0.0f, 0.0f)),
    //    new MyRenderParticle(particleModel, MyVector3(0.0f, 0.0f, 1.0f))
       //? Rod, Particle Spring, and Anchored Spring Test
        // new MyRenderParticle(particleModel, MyVector3(1.0f, 0.0f, 0.0f)),
        // new MyRenderParticle(particleModel, MyVector3(0.5f, 0.0f, 0.0f)),
        // new MyRenderParticle(particleModel, MyVector3(0.0f, 1.0f, 0.0f)),
        // new MyRenderParticle(particleModel, MyVector3(0.0f, 0.5f, 0.0f)),
        // new MyRenderParticle(particleModel, MyVector3(0.0f, 0.0f, 1.0f))
    });

    //? Contact Resolver Test: With Collision Detection
    // physicsWorld.getParticleListAsVector()[0 + originParticles]->setPosition(
    //     MyVector3(-20.0f, 0.0f, 0.0f));
    // physicsWorld.getParticleListAsVector()[1 + originParticles]->setPosition(
    //     MyVector3(20.0f, 0.0f, 0.0f));
    // physicsWorld.getParticleListAsVector()[0 + originParticles]->setRadius(10.0f);
    // physicsWorld.getParticleListAsVector()[1 + originParticles]->setRadius(10.0f);
    // physicsWorld.getParticleListAsVector()[0 + originParticles]->addForce(
    //     MyVector3(1000.0f, 0.0f, 0.0f));
    // physicsWorld.getParticleListAsVector()[1 + originParticles]->addForce(
    //     MyVector3(-1000.0f, 0.0f, 0.0f));

    //? Contact Resolver Test: No Collision Detection (remove generateContacts from
    // physicsWorld)
    // physicsWorld.addParticleContact(
    //     physicsWorld.getParticleListAsVector()[0 + originParticles],
    //     physicsWorld.getParticleListAsVector()[1 + originParticles],
    //     1,
    //     0.5,
    //     MyVector3(physicsWorld.getParticleListAsVector()[0 + originParticles]->getPosition()
    //     -
    //               physicsWorld.getParticleListAsVector()[1 + originParticles]->getPosition())
    //         .getNormalized());

    //? Rod
    physicsWorld.getParticleListAsVector()[0 + originParticles]->setPosition(
        MyVector3(-75.0f, 25.0f, 0.0f));
    physicsWorld.getParticleListAsVector()[1 + originParticles]->setPosition(
        MyVector3(-25.0f, -25.0f, 0.0f));
    MyVector3 rodLength =
        physicsWorld.getParticleListAsVector()[0 + originParticles]->getPosition() -
        physicsWorld.getParticleListAsVector()[1 + originParticles]->getPosition();
    physicsWorld.addRod(physicsWorld.getParticleListAsVector()[0 + originParticles],
                        physicsWorld.getParticleListAsVector()[1 + originParticles],
                        rodLength.getMagnitude());
    // physicsWorld.getParticleListAsVector()[0 + originParticles]->setUsesGravity(true);
    // physicsWorld.getParticleListAsVector()[1 + originParticles]->setUsesGravity(true);
    physicsWorld.getParticleListAsVector()[0 + originParticles]->addForce(
        MyVector3(0.0f, 1000.0f, 0.0f));
    // cout << "70.710678 | " << rodLength.getMagnitude() << endl;

    //? Particle Spring
    physicsWorld.getParticleListAsVector()[2 + originParticles]->setPosition(
        MyVector3(-25.0f, 25.0f, 0.0f));
    physicsWorld.getParticleListAsVector()[2 + originParticles]->setUsesGravity(true);
    physicsWorld.getParticleListAsVector()[3 + originParticles]->setPosition(
        MyVector3(25.0f, -25.0f, 0.0f));
    MyVector3 particleSpringLength =
        physicsWorld.getParticleListAsVector()[2 + originParticles]->getPosition() -
        physicsWorld.getParticleListAsVector()[3 + originParticles]->getPosition();
    physicsWorld.addSpring(physicsWorld.getParticleListAsVector()[2 + originParticles],
                           physicsWorld.getParticleListAsVector()[3 + originParticles],
                           5.0f,
                           particleSpringLength.getMagnitude());

    //? Anchored Spring
    physicsWorld.getParticleListAsVector()[4 + originParticles]->setPosition(
        MyVector3(25.0f, 25.0f, 0.0f));
    physicsWorld.getParticleListAsVector()[4 + originParticles]->setUsesGravity(true);
    physicsWorld.addSpring(physicsWorld.getParticleListAsVector()[4 + originParticles],
                           MyVector3(75.0f, -25.0f, 0.0f),
                           5.0f,
                           1.0f);

    //* - - - - - END OF PARTICLES - - - - -

    //* - - - - - PRE-RUNTIME - - - - -
    timerClockStart   = timerClock::now();
    //* - - - - - END OF PRE-RUNTIME - - - - -

    //* - - - - - PHYSICS WORLD INITIALIZATION - - - - -

    //* - - - - - DEBUGGING - - - - -

    //* - - - - - END OF DEBUGGING - - - - -

    //* - - - - - END OF PHYSICS WORLD INITIALIZATION - - - - -

    //* - - - - - RUNTIME - - - - -
    auto currentTime  = physicsClock::now();
    auto previousTime = currentTime;

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        activeCamera->setView(
            glm::lookAt(activeCamera->getPosition(), activeCamera->getViewCenter(), WorldUp));

        //* - - - - - FIXED UPDATE - - - - -
        currentTime   = physicsClock::now();
        auto duration = duration_cast<nanoseconds>(currentTime - previousTime);
        previousTime  = currentTime;

        if (!pausePhysics) {
            currentNanosecond += duration;
            if (currentNanosecond >= TIMESTEP) {
                auto millisecond = duration_cast<milliseconds>(currentNanosecond);
                if (DEBUG_MODE_PHYSICS_TIME)
                    cout << "Millisecond: " << (double)millisecond.count() << endl;
                if (DEBUG_MODE_PHYSICS_TIME) cout << "Physics Update" << endl;

                //? Place physics related updates BELOW this line
                //* - - - - - DEBUGGING - - - - -
                //* - - - - - END OF DEBUGGING - - - - -

                physicsWorld.update((double)millisecond.count() / 1000);
                //? Place physics related updates ABOVE this line

                currentNanosecond -= currentNanosecond;
            }
        } else {
        }
        //* - - - - - END OF FIXED UPDATE - - - - -

        //* - - - - - UPDATE - - - - -
        if (DEBUG_MODE_PHYSICS_TIME) cout << "Normal Update" << endl;
        //* - - - - - END OF UPDATE - - - - -

        //* - - - - - SKYBOX SHADER SWITCH - - - - -
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);
        glUseProgram(skyboxShaderProgram);
        //* - - - - - END OF SKYBOX SHADER SWITCH - - - - -

        //* - - - - - SKYBOX RENDERING - - - - -
        glm::mat4 skyboxView           = glm::mat4(1.0f);
        skyboxView                     = glm::mat4(glm::mat3(activeCamera->getView()));

        unsigned int skyboxViewAddress = glGetUniformLocation(skyboxShaderProgram, "view");
        glUniformMatrix4fv(skyboxViewAddress, 1, GL_FALSE, glm::value_ptr(skyboxView));

        unsigned skyboxProjectionAddress = glGetUniformLocation(skyboxShaderProgram, "projection");
        glUniformMatrix4fv(
            skyboxProjectionAddress, 1, GL_FALSE, glm::value_ptr(activeCamera->getProjection()));

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
        if (DRAW_SKYBOX) glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        //* - - - - - END OF SKYBOX RENDERING - - - - -

        //* - - - - - LIGHTING SHADER SWITCH - - - - -
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
        glUseProgram(lightingShaderProgram);
        //* - - - - - END OF LIGHTING SHADER SWITCH - - - - -

        //* - - - - - CAMERA UPDATE - - - - -
        unsigned int cameraProjectionAddress =
            glGetUniformLocation(lightingShaderProgram, "projection");
        glUniformMatrix4fv(
            cameraProjectionAddress, 1, GL_FALSE, glm::value_ptr(activeCamera->getProjection()));

        unsigned int cameraViewAddress = glGetUniformLocation(lightingShaderProgram, "view");
        glUniformMatrix4fv(cameraViewAddress, 1, GL_FALSE, glm::value_ptr(activeCamera->getView()));

        GLuint cameraPositionAddress =
            glGetUniformLocation(lightingShaderProgram, "cameraPosition");
        glUniform3fv(cameraPositionAddress, 1, glm::value_ptr(activeCamera->getPosition()));
        //* - - - - - END OF CAMERA UPDATE - - - - -

        //* - - - - - MODEL UPDATE - - - - -
        updateModelsList();
        for (My3DModel* model : renderingList) {
            //* - - - - - MODEL LIGHTING - - - - -
            glBindVertexArray(*model->getVAO());

            //* Model Tinting
            if (model->getName() == "DEFAULT PARTICLE") {
                GLuint modelColorAddress = glGetUniformLocation(lightingShaderProgram, "modelTint");
                glUniform3fv(modelColorAddress, 1, glm::value_ptr(model->getTint()));
                if (DEBUG_MODE_MODEL_TINTING) {
                    cout << "Tinted " << model->getName() << " with Color";
                    MyVector3 tint(model->getTint());
                    tint.DisplayValues(COUT_PRECISION);
                    cout << endl;
                }
            }

            glActiveTexture(GL_TEXTURE0);
            GLuint modelTextureAddress =
                glGetUniformLocation(lightingShaderProgram, "modelTexture");
            glBindTexture(GL_TEXTURE_2D, model->getTexture().getTexture());

            glActiveTexture(GL_TEXTURE1);
            GLuint modelNormalAddress =
                glGetUniformLocation(lightingShaderProgram, "modelNormalTexture");
            glBindTexture(GL_TEXTURE_2D, model->getNormalMap().getNormal());

            GLuint directionalLightCountAddress =
                glGetUniformLocation(lightingShaderProgram, "directionalLightCount");
            glUniform1i(directionalLightCountAddress, GLint(directionalLights.size()));

            for (int i = 0; i < directionalLights.size(); i++) {
                if (directionalLights[i]->enabled) {
                    string address = "directionalLights[" + to_string(i) + "].direction";

                    GLuint lightDirectionAddress =
                        glGetUniformLocation(lightingShaderProgram, address.c_str());
                    glUniform3fv(
                        lightDirectionAddress, 1, glm::value_ptr(directionalLights[i]->direction));

                    address = "directionalLights[" + to_string(i) + "].color";
                    GLuint lightColorAddress =
                        glGetUniformLocation(lightingShaderProgram, address.c_str());
                    glUniform3fv(lightColorAddress, 1, glm::value_ptr(directionalLights[i]->color));

                    address = "directionalLights[" + to_string(i) + "].ambientStrength";
                    GLuint ambientStrengthAddress =
                        glGetUniformLocation(lightingShaderProgram, address.c_str());
                    glUniform1f(ambientStrengthAddress, directionalLights[i]->ambientStrength);

                    address = "directionalLights[" + to_string(i) + "].ambientColor";
                    GLuint ambientColorAddress =
                        glGetUniformLocation(lightingShaderProgram, address.c_str());
                    glUniform3fv(
                        ambientColorAddress, 1, glm::value_ptr(directionalLights[i]->ambientColor));

                    address = "directionalLights[" + to_string(i) + "].specularStrength";
                    GLuint specularStrengthAddress =
                        glGetUniformLocation(lightingShaderProgram, address.c_str());
                    glUniform1f(specularStrengthAddress, directionalLights[i]->specularStrength);

                    address = "directionalLights[" + to_string(i) + "].specularPhong";
                    GLuint specularPhongAddress =
                        glGetUniformLocation(lightingShaderProgram, address.c_str());
                    glUniform1f(specularPhongAddress, directionalLights[i]->specularPhong);

                    address = "directionalLights[" + to_string(i) + "].brightness";
                    GLuint brightnessAddress =
                        glGetUniformLocation(lightingShaderProgram, address.c_str());
                    glUniform1f(brightnessAddress, directionalLights[i]->brightness);
                }
            }

            GLuint pointLightCountAddress =
                glGetUniformLocation(lightingShaderProgram, "pointLightCount");
            glUniform1i(pointLightCountAddress, GLint(pointLights.size()));

            for (int i = 0; i < pointLights.size(); i++) {
                if (pointLights[i]->enabled) {
                    string address = "pointLights[" + to_string(i) + "].position";

                    GLuint lightPositionAddress =
                        glGetUniformLocation(lightingShaderProgram, address.c_str());
                    glUniform3fv(lightPositionAddress, 1, glm::value_ptr(pointLights[i]->position));

                    address = "pointLights[" + to_string(i) + "].color";
                    GLuint lightColorAddress =
                        glGetUniformLocation(lightingShaderProgram, address.c_str());
                    glUniform3fv(lightColorAddress, 1, glm::value_ptr(pointLights[i]->color));

                    address = "pointLights[" + to_string(i) + "].ambientStrength";
                    GLuint ambientStrengthAddress =
                        glGetUniformLocation(lightingShaderProgram, address.c_str());
                    glUniform1f(ambientStrengthAddress, pointLights[i]->ambientStrength);

                    address = "pointLights[" + to_string(i) + "].ambientColor";
                    GLuint ambientColorAddress =
                        glGetUniformLocation(lightingShaderProgram, address.c_str());
                    glUniform3fv(
                        ambientColorAddress, 1, glm::value_ptr(pointLights[i]->ambientColor));

                    address = "pointLights[" + to_string(i) + "].specularStrength";
                    GLuint specularStrengthAddress =
                        glGetUniformLocation(lightingShaderProgram, address.c_str());
                    glUniform1f(specularStrengthAddress, pointLights[i]->specularStrength);

                    address = "pointLights[" + to_string(i) + "].specularPhong";
                    GLuint specularPhongAddress =
                        glGetUniformLocation(lightingShaderProgram, address.c_str());
                    glUniform1f(specularPhongAddress, pointLights[i]->specularPhong);

                    address = "pointLights[" + to_string(i) + "].brightness";
                    GLuint brightnessAddress =
                        glGetUniformLocation(lightingShaderProgram, address.c_str());
                    glUniform1f(brightnessAddress, pointLights[i]->brightness);
                }
            }

            GLuint spotLightCountAddress =
                glGetUniformLocation(lightingShaderProgram, "spotLightCount");
            glUniform1i(spotLightCountAddress, GLint(spotLights.size()));

            for (int i = 0; i < spotLights.size(); i++) {
                if (spotLights[i]->enabled) {
                    string address = "spotLights[" + to_string(i) + "].position";

                    GLuint lightPositionAddress =
                        glGetUniformLocation(lightingShaderProgram, address.c_str());
                    glUniform3fv(lightPositionAddress, 1, glm::value_ptr(spotLights[i]->position));

                    address = "spotLights[" + to_string(i) + "].direction";
                    GLuint lightDirectionAddress =
                        glGetUniformLocation(lightingShaderProgram, address.c_str());
                    glUniform3fv(
                        lightDirectionAddress, 1, glm::value_ptr(spotLights[i]->direction));

                    address = "spotLights[" + to_string(i) + "].coneSize";
                    GLuint coneSizeAddress =
                        glGetUniformLocation(lightingShaderProgram, address.c_str());
                    glUniform1f(coneSizeAddress, spotLights[i]->coneSize);

                    address = "spotLights[" + to_string(i) + "].color";
                    GLuint lightColorAddress =
                        glGetUniformLocation(lightingShaderProgram, address.c_str());
                    glUniform3fv(lightColorAddress, 1, glm::value_ptr(spotLights[i]->color));

                    address = "spotLights[" + to_string(i) + "].ambientStrength";
                    GLuint ambientStrengthAddress =
                        glGetUniformLocation(lightingShaderProgram, address.c_str());
                    glUniform1f(ambientStrengthAddress, spotLights[i]->ambientStrength);

                    address = "spotLights[" + to_string(i) + "].ambientColor";
                    GLuint ambientColorAddress =
                        glGetUniformLocation(lightingShaderProgram, address.c_str());
                    glUniform3fv(
                        ambientColorAddress, 1, glm::value_ptr(spotLights[i]->ambientColor));

                    address = "spotLights[" + to_string(i) + "].specularStrength";
                    GLuint specularStrengthAddress =
                        glGetUniformLocation(lightingShaderProgram, address.c_str());
                    glUniform1f(specularStrengthAddress, spotLights[i]->specularStrength);

                    address = "spotLights[" + to_string(i) + "].specularPhong";
                    GLuint specularPhongAddress =
                        glGetUniformLocation(lightingShaderProgram, address.c_str());
                    glUniform1f(specularPhongAddress, spotLights[i]->specularPhong);

                    address = "spotLights[" + to_string(i) + "].brightness";
                    GLuint brightnessAddress =
                        glGetUniformLocation(lightingShaderProgram, address.c_str());
                    glUniform1f(brightnessAddress, spotLights[i]->brightness);
                }
            }
            //* - - - - - END OF MODEL LIGHTING - - - - -

            //* - - - - - MODEL TRANSFORM - - - - -
            model->update();
            unsigned int modelTransformAddress =
                glGetUniformLocation(lightingShaderProgram, "transform");
            glUniformMatrix4fv(
                modelTransformAddress, 1, GL_FALSE, glm::value_ptr(model->getPositionMatrix()));
            //* - - - - - END OF MODEL TRANSFORM - - - - -

            //* - - - - - MODEL RENDERING - - - - -
            glUniform1i(modelTextureAddress, model->getTexture().getTextureCount());
            if (model->getEnabled())
                glDrawArrays(
                    GL_TRIANGLES, 0, GLint(model->getFullVertexData().size() / STRIDE_LENGTH));
            //* - - - - - END OF MODEL RENDERING - - - - -
        }

        //* - - - - - END OF MODEL UPDATE - - - - -
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //* - - - - - END OF RUNTIME - - - - -

    //* - - - - - CLEAN UP - - - - -
    for (My3DModel* model : renderingList) {
        glDeleteVertexArrays(1, model->getVAO());
        glDeleteBuffers(1, model->getVBO());
    }
    glDeleteShader(lightingVertexShader.getShader());
    glDeleteShader(lightingFragmentShader.getShader());
    glDeleteShader(skyboxVertexShader.getShader());
    glDeleteShader(skyboxFragmentShader.getShader());
    //* - - - - - END OF CLEAN UP - - - - -
    glfwTerminate();
    return 0;
}