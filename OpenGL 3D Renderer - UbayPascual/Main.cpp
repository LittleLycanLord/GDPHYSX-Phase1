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
#include "MyClasses/MyParticleConnections/MyAnchoredRope/MyAnchoredRope.hpp"
#include "MyClasses/MyParticleConnections/MyAnchoredChain/MyAnchoredChain.hpp"
#include "MyClasses/MyParticle/MyParticleSystem/MyParticleSystem.hpp"
#include "MyClasses/MyParticle/MyRenderParticle/MyRenderParticle.hpp"
#include "MyClasses/MyParticleContact/MyParticleContact.hpp"
#include "MyClasses/MyParticleLink/MyParticleLink.hpp"
#include "MyClasses/MyParticleLink/MyRod/MyCable/MyCable.hpp"
#include "MyClasses/MyParticleLink/MyRod/MyRod.hpp"
#include "MyClasses/MyPhysicsWorld/MyPhysicsWorld.hpp"
#include "MyClasses/MyRenderLine/MyRenderLine.hpp"
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
bool pausePhysics = true;
bool physicsStep = false;
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
        //* Physics Step
        case GLFW_KEY_P:
            if (action == GLFW_PRESS) {
                if(pausePhysics)
                    physicsStep = true;
            }
            break;
    }
}

void createNewtonsCradle(My3DModel* particleModel,
                         double cableLength,
                         double particleGap,
                         double particleRadius,
                         double gravityStrength,
                         MyVector3 appliedForce) {
    physicsWorld.setGravity(gravityStrength);

    //? Cable Pendulum
    //? Create the 2 Ends
    double x = 0 - ((particleRadius) * 6);
    for (int i = 0; i < 5; i++) {
        x += particleRadius * 2;
        MyRenderParticle* particleA =
            new MyRenderParticle(particleModel, MyVector3(1.0f, 0.2f, 0.2f));
        MyRenderParticle* particleB =
            new MyRenderParticle(particleModel, MyVector3(0.2f, 0.2f, 1.0f));
        physicsWorld.addParticle(particleA, false);
        physicsWorld.addParticle(particleB, false);
        particleA->setOriginalPosition(MyVector3(x, 10.0f, -particleGap / 2.0f), true);
        particleA->setRadius(particleRadius);
        particleB->setOriginalPosition(MyVector3(x, 10.0f, particleGap / 2.0f), true);
        particleB->setRadius(particleRadius);

        MyRenderParticle* particleC =
            new MyRenderParticle(particleModel, MyVector3(0.2f, 1.0f, 0.2f));
        physicsWorld.addParticle(particleC, true);
        particleC->setOriginalPosition(MyVector3(x, 0.0f, 0.0f), true);
        particleC->setRadius(particleRadius);

        particleA->setIsPersistent(true);
        particleB->setIsPersistent(true);

        physicsWorld.addCable(particleA, particleC, cableLength);
        physicsWorld.addCable(particleB, particleC, cableLength);

        particleA->setRestitution(0.9f);
        particleB->setRestitution(0.9f);
        particleC->setRestitution(0.9f);
        particleA->setMass(50.0f);
        particleB->setMass(50.0f);
        particleC->setMass(50.0f);
        if (i == 0) particleC->addForce(appliedForce);
    }
}


void makeCarousel(My3DModel* particleModel, MyParticle* topParticle) {
    //? Create the Anchors and Seats
    double fortyFiveDegree((sqrt(2) / 2) *
                           40.0f);  //* evenly spaced out on a circle with a diameter of 80m.

    MyRenderParticle* contactParticle =
        new MyRenderParticle(particleModel, MyVector3(1.0f, 0.0f, 0.0f));

    vector<MyRenderParticle*> seats;
    for (int i = 0; i < 8; i++) {
        MyRenderParticle* anchorParticle =
            new MyRenderParticle(particleModel, MyVector3(0.5f, 0.5f, 0.5f));
        if (i == 0) contactParticle = anchorParticle;
        physicsWorld.addParticle(anchorParticle, false);
        MyRenderParticle* seatParticle =
            new MyRenderParticle(particleModel, MyVector3(1.0f, 1.0f, 0.0f));
        physicsWorld.addParticle(seatParticle, true);

        seats.push_back(seatParticle);

        //? Following the Unit Circle
        switch (i) {
            case 0:
                anchorParticle->setOriginalPosition(MyVector3(40.0f, 40.0f, 0.0f), true);
                seatParticle->setPosition(MyVector3(40.0f, 25.0f, 0.0f));
                break;
            case 1:
                anchorParticle->setOriginalPosition(
                    MyVector3(fortyFiveDegree, 40.0f, fortyFiveDegree), true);
                seatParticle->setPosition(MyVector3(fortyFiveDegree, 25.0f, fortyFiveDegree));
                break;
            case 2:
                anchorParticle->setOriginalPosition(MyVector3(0.0f, 40.0f, 40.0f), true);
                seatParticle->setPosition(MyVector3(0.0f, 25.0f, 40.0f));
                break;
            case 3:
                anchorParticle->setOriginalPosition(
                    MyVector3(-fortyFiveDegree, 40.0f, fortyFiveDegree), true);
                seatParticle->setPosition(MyVector3(-fortyFiveDegree, 25.0f, fortyFiveDegree));
                break;
            case 4:
                anchorParticle->setOriginalPosition(MyVector3(-40.0f, 40.0f, 0.0f), true);
                seatParticle->setPosition(MyVector3(-40.0f, 25.0f, 0.0f));
                break;
            case 5:
                anchorParticle->setOriginalPosition(
                    MyVector3(-fortyFiveDegree, 40.0f, -fortyFiveDegree), true);
                seatParticle->setPosition(MyVector3(-fortyFiveDegree, 25.0f, -fortyFiveDegree));
                break;
            case 6:
                anchorParticle->setOriginalPosition(MyVector3(0.0f, 40.0f, -40.0f), true);
                seatParticle->setPosition(MyVector3(0.0f, 25.0f, -40.0f));
                break;
            case 7:
                anchorParticle->setOriginalPosition(
                    MyVector3(fortyFiveDegree, 40.0f, -fortyFiveDegree), true);
                seatParticle->setPosition(MyVector3(fortyFiveDegree, 25.0f, -fortyFiveDegree));
                break;
        }
        anchorParticle->setRadius(1.0f);
        anchorParticle->setLockY(true);
        anchorParticle->setMass(60.0f);
        seatParticle->setRadius(5.0f);  //* "with a radius of 5m"
        seatParticle->setMass(60.0f);   //* "and around 60kg"
        MyVector3 anchorRod = topParticle->getPosition() - anchorParticle->getPosition();
        physicsWorld.addRod(topParticle, anchorParticle, anchorRod.getMagnitude());
        physicsWorld.addCable(anchorParticle, seatParticle, 15.0f);
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
             new MyRenderParticle(particleModel, MyVector3(1.0f, 1.0f, 1.0f))},
            false);
        physicsWorld.getParticleListAsVector()[0]->setPosition(MyVector3(10.0f, 0.0f, 0.0f));
        physicsWorld.getParticleListAsVector()[0]->setHasCollision(false);
        physicsWorld.getParticleListAsVector()[0]->setRadius(0.1f);
        physicsWorld.getParticleListAsVector()[1]->setPosition(MyVector3(0.0f, 10.0f, 0.0f));
        physicsWorld.getParticleListAsVector()[1]->setHasCollision(false);
        physicsWorld.getParticleListAsVector()[1]->setRadius(0.1f);
        physicsWorld.getParticleListAsVector()[2]->setPosition(MyVector3(0.0f, 0.0f, 10.0f));
        physicsWorld.getParticleListAsVector()[2]->setHasCollision(false);
        physicsWorld.getParticleListAsVector()[2]->setRadius(0.1f);
        physicsWorld.getParticleListAsVector()[3]->setHasCollision(false);
        physicsWorld.getParticleListAsVector()[3]->setRadius(0.1f);
        originParticles = physicsWorld.getParticleListAsVector().size();
    }
    //* - - - - - END OF PARTICLE SETUP - - - - -

    //* - - - - - PARTICLES - - - - -
    // //? Damping - Drag Test : PASS
    // //? PASS if: The test particle slows down over time due to it's damping attribute.
    // MyRenderParticle* testParticle =
    //     new MyRenderParticle(particleModel, MyVector3(1.0f, 1.0f, 1.0f));
    // physicsWorld.addParticle(testParticle, false);
    // testParticle->setPosition(MyVector3(-10.0f, 0.0f, 0.0f));
    // testParticle->addForce(MyVector3(10.0f, 0.0f, 0.0f));
    // //? Debugging Code:
    // // testParticle->getVelocity().DisplayValuesIndividually("Velocity: ", 2.0f);

    // //? 2nd Law of Motion Test : PASS
    // //? PASS if: The heavier red particle moves slower due to the 2nd law of motion.
    // MyRenderParticle* testParticleA =
    //     new MyRenderParticle(particleModel, MyVector3(0.0f, 0.0f, 1.0f));
    // physicsWorld.addParticle(testParticleA, false);
    // testParticleA->setPosition(MyVector3(-10.0f, 3.0f, 0.0f));

    // testParticleA->setMass(1.0f);
    // testParticleA->addForce(MyVector3(100.0f, 0.0f, 0.0f));
    // MyRenderParticle* testParticleB =
    //     new MyRenderParticle(particleModel, MyVector3(1.0f, 0.0f, 0.0f));
    // physicsWorld.addParticle(testParticleB, false);
    // testParticleB->setPosition(MyVector3(-10.0f, -3.0f, 0.0f));

    // testParticleB->setMass(100.0f);
    // testParticleB->addForce(MyVector3(100.0f, 0.0f, 0.0f));
    // //? Debugging Code:
    // // testParticleA->getVelocity().DisplayValuesIndividually("Velocity A: ", 2.0f);
    // // testParticleB->getVelocity().DisplayValuesIndividually("Velocity B: ", 2.0f);

    // //? D'Alembert's Principle Test : PASS
    // //? PASS if: Both test particles move at the same rate, as particle B's accumulated force
    // should
    // //? be the same, despite the force being added in multiple instances.
    // MyRenderParticle* testParticleA =
    //     new MyRenderParticle(particleModel, MyVector3(0.0f, 0.0f, 1.0f));
    // physicsWorld.addParticle(testParticleA, false);
    // testParticleA->setPosition(MyVector3(-10.0f, 3.0f, 0.0f));

    // testParticleA->addForce(MyVector3(100.0f, 0.0f, 0.0f));
    // MyRenderParticle* testParticleB =
    //     new MyRenderParticle(particleModel, MyVector3(1.0f, 0.0f, 0.0f));
    // physicsWorld.addParticle(testParticleB, false);
    // testParticleB->setPosition(MyVector3(-10.0f, -3.0f, 0.0f));

    // testParticleB->addForce(MyVector3(50.0f, 0.0f, 0.0f));
    // testParticleB->addForce(MyVector3(50.0f, 0.0f, 0.0f));
    // //? Debugging Code:
    // // testParticleA->getVelocity().DisplayValuesIndividually("Velocity A: ", 2.0f);
    // // testParticleB->getVelocity().DisplayValuesIndividually("Velocity B: ", 2.0f);

    // //? Gravity Test : PASS
    // //? PASS if: The test particle has a downwards acceleration equal to the gravity.
    // MyRenderParticle* testParticle =
    //     new MyRenderParticle(particleModel, MyVector3(1.0f, 1.0f, 1.0f));
    // physicsWorld.addParticle(testParticle, true);
    // testParticle->setPosition(MyVector3(0.0f, 1.0f, 0.0f));
    // //? Debugging Code:
    // // testParticle->getTestAcceleration().DisplayValuesIndividually("Acceleration", 2.0f);

    // //? Drag Test : NOT NEEDED YET

    // //? Interpenetration Test: ???
    // //? PASS if: Both particles are simply side by side, with no velocity.
    // //? - Yes, they'll be in constant collision as their "skins" are simply touching.
    // MyRenderParticle* particleA = new MyRenderParticle(particleModel, MyVector3(1.0f, 0.0f,
    // 0.0f)); MyRenderParticle* particleB = new MyRenderParticle(particleModel, MyVector3(0.0f,
    // 0.0f, 1.0f)); physicsWorld.addParticle(particleA, false); physicsWorld.addParticle(particleB,
    // false); particleA->setRadius(3.0f); particleB->setRadius(3.0f);
    // particleA->setPosition(MyVector3(0.01f, 12.0f, 0.0f));
    // particleB->setPosition(MyVector3(-0.01f, 12.0f, 0.0f));

    // MyRenderParticle* particleAA = new MyRenderParticle(particleModel, MyVector3(1.0f, 0.0f,
    // 0.0f)); MyRenderParticle* particleBB = new MyRenderParticle(particleModel, MyVector3(0.0f,
    // 0.0f, 1.0f)); physicsWorld.addParticle(particleAA, false);
    // physicsWorld.addParticle(particleBB, false);
    // particleAA->setRadius(3.0f);
    // particleBB->setRadius(3.0f);
    // particleAA->setPosition(MyVector3(1.5f, 4.0f, 0.0f));
    // particleBB->setPosition(MyVector3(-1.5f, 4.0f, 0.0f));

    // MyRenderParticle* particleAAA =
    //     new MyRenderParticle(particleModel, MyVector3(1.0f, 0.0f, 0.0f));
    // MyRenderParticle* particleBBB =
    //     new MyRenderParticle(particleModel, MyVector3(0.0f, 0.0f, 1.0f));
    // physicsWorld.addParticle(particleAAA, false);
    // physicsWorld.addParticle(particleBBB, false);
    // particleAAA->setRadius(3.0f);
    // particleBBB->setRadius(3.0f);
    // particleAAA->setPosition(MyVector3(2.5f, -4.0f, 0.0f));
    // particleBBB->setPosition(MyVector3(-2.5f, -4.0f, 0.0f));

    // MyRenderParticle* particleAAAA =
    //     new MyRenderParticle(particleModel, MyVector3(1.0f, 0.0f, 0.0f));
    // MyRenderParticle* particleBBBB =
    //     new MyRenderParticle(particleModel, MyVector3(0.0f, 0.0f, 1.0f));
    // physicsWorld.addParticle(particleAAAA, false);
    // physicsWorld.addParticle(particleBBBB, false);
    // particleAAAA->setRadius(3.0f);
    // particleBBBB->setRadius(3.0f);
    // particleAAAA->setPosition(MyVector3(2.99f, -12.0f, 0.0f));
    // particleBBBB->setPosition(MyVector3(-2.99f, -12.0f, 0.0f));
    // //? Debugging Code:

    // //? Collision Test: PASS
    // //? PASS if: Both particles bounce off each other
    // MyRenderParticle* particleGoingLeft =
    //     new MyRenderParticle(particleModel, MyVector3(1.0f, 0.0f, 0.0f));
    // MyRenderParticle* particleGoingRight =
    //     new MyRenderParticle(particleModel, MyVector3(0.0f, 0.0f, 1.0f));
    // physicsWorld.addParticle(particleGoingLeft, false);
    // physicsWorld.addParticle(particleGoingRight, false);

    // particleGoingLeft->setRadius(5.0f);
    // particleGoingLeft->setMass(50.0f);
    // particleGoingRight->setRadius(5.0f);
    // particleGoingRight->setMass(50.0f);

    // particleGoingRight->setPosition(MyVector3(-20.0f, 0.0f, 0.0f));
    // particleGoingRight->addForce(MyVector3(10000.0f, 0.0f, 0.0f));
    // particleGoingLeft->setPosition(MyVector3(20.0f, 0.0f, 0.0f));
    // particleGoingLeft->addForce(MyVector3(-10000.0f, 0.0f, 0.0f));
    // //? Debugging Code:

    // //? Mass Agregate Test: PASS
    // //? PASS if: The right-most particle moves
    // MyRenderParticle* particle1 = new MyRenderParticle(particleModel, MyVector3(0.5f, 0.0f,
    // 0.0f)); MyRenderParticle* particle2 = new MyRenderParticle(particleModel, MyVector3(1.0f,
    // 0.0f, 0.0f)); MyRenderParticle* particle3 = new MyRenderParticle(particleModel,
    // MyVector3(1.0f, 0.0f, 1.0f)); MyRenderParticle* particle4 = new
    // MyRenderParticle(particleModel, MyVector3(0.0f, 0.0f, 0.5f)); MyRenderParticle* particle5 =
    // new MyRenderParticle(particleModel, MyVector3(0.0f, 0.0f, 1.0f));
    // physicsWorld.addParticle(particle1, false);
    // physicsWorld.addParticle(particle2, false);
    // physicsWorld.addParticle(particle3, false);
    // physicsWorld.addParticle(particle4, false);
    // physicsWorld.addParticle(particle5, false);
    // particle1->setMass(5.0f);
    // particle2->setMass(5.0f);
    // particle3->setMass(5.0f);
    // particle4->setMass(5.0f);
    // particle5->setMass(5.0f);
    // particle1->setPosition(-10.0f, 0.0f, 0.0f);
    // particle2->setPosition(-2.0f, 0.0f, 0.0f);
    // particle3->setPosition(0.0f, 0.0f, 0.0f);
    // particle4->setPosition(2.0f, 0.0f, 0.0f);
    // particle5->setPosition(4.0f, 0.0f, 0.0f);
    // particle1->addForce(MyVector3(1000.0f, 0.0f, 0.0f));
    // //? Debugging Code:

    // //? Rod Creation
    // //? Create the 2 Ends
    // MyRenderParticle* particleA = new MyRenderParticle(particleModel, MyVector3(1.0f, 0.0f,
    // 0.0f)); MyRenderParticle* particleB = new MyRenderParticle(particleModel, MyVector3(0.0f,
    // 0.0f, 1.0f)); MyRenderParticle* groundA   = new MyRenderParticle(particleModel,
    // MyVector3(1.0f, 0.3f, 0.3f)); MyRenderParticle* groundB   = new
    // MyRenderParticle(particleModel, MyVector3(0.3f, 0.3f, 1.0f));
    // physicsWorld.addParticle(particleA, true);
    // physicsWorld.addParticle(particleB, false);
    // physicsWorld.addParticle(groundA, false);
    // physicsWorld.addParticle(groundB, false);
    // groundA->setIsPersistent(true);
    // groundB->setIsPersistent(true);
    // groundA->setOriginalPosition(MyVector3(-2.5f, 0.0f, 0.0f), true);
    // groundB->setOriginalPosition(MyVector3(2.5f, 0.0f, 0.0f), true);
    // particleA->setMass(100.0f);
    // particleB->setMass(1.0f);
    // groundA->setMass(10.0f);
    // groundB->setMass(10.0f);

    //? Test the Rod's Behaviour
    // //? - Rod Drop :
    // particleA->setOriginalPosition(MyVector3(-2.5f, 15.0f, 0.0f), true);
    // particleB->setOriginalPosition(MyVector3(2.5f, 10.0f, 0.0f), true);
    // MyVector3 rodLength = particleA->getPosition() - particleB->getPosition();
    // physicsWorld.addRod(particleA, particleB, rodLength.getMagnitude());
    // particleA->addForce(MyVector3(0.0f, -1000.0f, 0.0f));
    // particleB->addForce(MyVector3(0.0f, -1000.0f, 0.0f));
    // //? Debugging Code:
    // MyVector3 distanceVector = particleA->getPosition() - particleB->getPosition();
    // cout << "Distance: " << distanceVector.getMagnitude() << endl;

    // //? - Rod Stretch
    // particleA->setOriginalPosition(MyVector3(-2.5f, 15.0f, 0.0f), true);
    // particleB->setOriginalPosition(MyVector3(2.5f, 10.0f, 0.0f), true);
    // particleB->setIsPersistent(true);
    // MyVector3 rodLength = particleA->getPosition() - particleB->getPosition();
    // physicsWorld.addRod(particleA, particleB, rodLength.getMagnitude());
    // particleA->addForce(MyVector3(-1000.0f, 0.0, 0.0f));

    //? - Rod Spin :
    // particleA->setOriginalPosition(MyVector3(-2.5f, 10.0f, 0.0f), true);
    // particleB->setOriginalPosition(MyVector3(2.5f, 10.0f, 0.0f), true);
    // MyVector3 rodLength = particleA->getPosition() - particleB->getPosition();
    // physicsWorld.addRod(particleA, particleB, rodLength.getMagnitude());
    // particleA->addForce(MyVector3(0.0f, -1000.0f, 0.0f));
    // particleB->addForce(MyVector3(0.0f, 1000.0f, 0.0f));

    // //? Rod Collision
    // //? Create the 4 Rod Ends
    // MyRenderParticle* particleTopA =
    //     new MyRenderParticle(particleModel, MyVector3(1.0f, 0.0f, 0.0f));
    // MyRenderParticle* particleTopB =
    //     new MyRenderParticle(particleModel, MyVector3(1.0f, 0.0f, 0.0f));
    // physicsWorld.addParticle(particleTopA, false);
    // physicsWorld.addParticle(particleTopB, false);

    // MyRenderParticle* particleBottomA =
    //     new MyRenderParticle(particleModel, MyVector3(1.0f, 0.0f, 0.0f));
    // MyRenderParticle* particleBottomB =
    //     new MyRenderParticle(particleModel, MyVector3(1.0f, 0.0f, 0.0f));
    // physicsWorld.addParticle(particleBottomA, false);
    // physicsWorld.addParticle(particleBottomB, false);

    //? Test the Collision
    // //? - Head On : PASS
    // particleTopA->setPosition(MyVector3(-2.5f, 5.0f, 0.0f));
    // particleTopB->setPosition(MyVector3(2.5f, 5.0f, 0.0f));
    // MyVector3 topRodLength = particleTopA->getPosition() - particleTopB->getPosition();
    // physicsWorld.addRod(particleTopA, particleTopB, topRodLength.getMagnitude());
    // particleBottomA->setPosition(MyVector3(-2.5f, -5.0f, 0.0f));
    // particleBottomB->setPosition(MyVector3(2.5f, -5.0f, 0.0f));
    // MyVector3 bottomRodLength = particleBottomA->getPosition() -
    // particleBottomB->getPosition(); physicsWorld.addRod(particleBottomA, particleBottomB,
    // bottomRodLength.getMagnitude()); particleTopA->addForce(MyVector3(0.0f, -1000.0f, 0.0f));
    // particleTopB->addForce(MyVector3(0.0f, -1000.0f, 0.0f));
    // particleBottomA->addForce(MyVector3(0.0f, 1000.0f, 0.0f));
    // particleBottomB->addForce(MyVector3(0.0f, 1000.0f, 0.0f));

    // //? - Side : PASS
    // particleTopA->setPosition(MyVector3(-2.5f, 5.0f, 0.0f));
    // particleTopB->setPosition(MyVector3(0.0f, 5.0f, 0.0f));
    // MyVector3 topRodLength = particleTopA->getPosition() - particleTopB->getPosition();
    // physicsWorld.addRod(particleTopA, particleTopB, topRodLength.getMagnitude());
    // particleBottomA->setPosition(MyVector3(0.0f, -5.0f, 0.0f));
    // particleBottomB->setPosition(MyVector3(2.5f, -5.0f, 0.0f));
    // MyVector3 bottomRodLength = particleBottomA->getPosition() -
    // particleBottomB->getPosition(); physicsWorld.addRod(particleBottomA, particleBottomB,
    // bottomRodLength.getMagnitude()); particleTopA->addForce(MyVector3(0.0f, -1000.0f, 0.0f));
    // particleTopB->addForce(MyVector3(0.0f, -1000.0f, 0.0f));
    // particleBottomA->addForce(MyVector3(0.0f, 1000.0f, 0.0f));
    // particleBottomB->addForce(MyVector3(0.0f, 1000.0f, 0.0f));

    // //? - Complex : PASS
    // particleTopA->setPosition(MyVector3(-2.5f, 5.0f, 0.0f));
    // particleTopB->setPosition(MyVector3(5.0f, 5.0f, 0.0f));
    // MyVector3 topRodLength = particleTopA->getPosition() - particleTopB->getPosition();
    // physicsWorld.addRod(particleTopA, particleTopB, topRodLength.getMagnitude());
    // particleBottomA->setPosition(MyVector3(-5.0f, -5.0f, 0.0f));
    // particleBottomB->setPosition(MyVector3(2.5f, -5.0f, 0.0f));
    // MyVector3 bottomRodLength = particleBottomA->getPosition() -
    // particleBottomB->getPosition(); physicsWorld.addRod(particleBottomA, particleBottomB,
    // bottomRodLength.getMagnitude());

    // particleTopA->addForce(MyVector3(0.0f, -1000.0f, 0.0f));
    // particleBottomB->addForce(MyVector3(0.0f, 1000.0f, 0.0f));
    // //* Confirmed Working

    // //? Rod Chain
    // //? Create the 4 Points
    // MyRenderParticle* particleA = new MyRenderParticle(particleModel, MyVector3(1.0f, 0.0f,
    // 0.0f)); MyRenderParticle* particleB = new MyRenderParticle(particleModel, MyVector3(1.0f,
    // 0.0f, 0.0f)); MyRenderParticle* particleC = new MyRenderParticle(particleModel,
    // MyVector3(1.0f, 0.0f, 0.0f)); MyRenderParticle* particleD = new
    // MyRenderParticle(particleModel, MyVector3(1.0f, 0.0f, 0.0f));
    // physicsWorld.addParticle(particleA, false);
    // physicsWorld.addParticle(particleB, false);
    // physicsWorld.addParticle(particleC, false);
    // physicsWorld.addParticle(particleD, false);
    // particleA->setPosition(MyVector3(-7.5f, 0.0f, 0.0f));
    // particleB->setPosition(MyVector3(-2.5f, 0.0f, 0.0f));
    // particleC->setPosition(MyVector3(2.5f, 0.0f, 0.0f));
    // particleD->setPosition(MyVector3(7.5f, 0.0f, 0.0f));

    // //? Join with 3 Rods
    // MyVector3 rodABLength = particleA->getPosition() - particleB->getPosition();
    // physicsWorld.addRod(particleA, particleB, rodABLength.getMagnitude());
    // MyVector3 rodBCLength = particleB->getPosition() - particleC->getPosition();
    // physicsWorld.addRod(particleB, particleC, rodBCLength.getMagnitude());
    // MyVector3 rodCDLength = particleC->getPosition() - particleD->getPosition();
    // physicsWorld.addRod(particleC, particleD, rodCDLength.getMagnitude());
    // //* Confirmed Working

    // //? Test Behaviour
    // //? Movement
    // particleC->addForce(MyVector3(0.0f, -1000.0f, 0.0f));

    // //? Anchored Rod
    // //? Create the attached Particle and the anchor Particle
    // MyRenderParticle* attachedParticle =
    //     new MyRenderParticle(particleModel, MyVector3(1.0f, 0.0f, 0.0f));
    // MyRenderParticle* anchorParticle =
    //     new MyRenderParticle(particleModel, MyVector3(1.0f, 1.0f, 1.0f));
    // physicsWorld.addParticle(attachedParticle, false);
    // physicsWorld.addParticle(anchorParticle, false);
    // attachedParticle->setPosition(MyVector3(-5.0f, 0.0f, 0.0f));
    // anchorParticle->setPosition(MyVector3(0.0f, 0.0f, 0.0f));
    // anchorParticle->setIsPersistent(true);

    // //? Test the Rod's Behaviour
    // //? - Spin : PASS
    // MyVector3 rodLength = attachedParticle->getPosition() - anchorParticle->getPosition();
    // physicsWorld.addRod(attachedParticle, anchorParticle, rodLength.getMagnitude());
    // attachedParticle->addForce(MyVector3(0.0f, 1000.0f, 0.0f));
    // //? - Stretch : PASS
    // MyVector3 rodLength = attachedParticle->getPosition() - anchorParticle->getPosition();
    // physicsWorld.addRod(attachedParticle, anchorParticle, rodLength.getMagnitude());
    // attachedParticle->addForce(MyVector3(-1000.0f, 0.0f, 0.0f));
    // //? - Compress : PASS
    // MyVector3 rodLength = attachedParticle->getPosition() - anchorParticle->getPosition();
    // physicsWorld.addRod(attachedParticle, anchorParticle, rodLength.getMagnitude());
    // attachedParticle->addForce(MyVector3(1000.0f, 0.0f, 0.0f));
    // //? - Custom Rod Length : PASS
    // double rodLength = 10;  //? Originally 5
    // physicsWorld.addRod(attachedParticle, anchorParticle, rodLength);
    // attachedParticle->addForce(MyVector3(1000.0f, 0.0f, 0.0f));
    // //* Confirmed Working

    // //? Cable Creation
    // //? Create the 2 Ends
    // MyRenderParticle* particleA = new MyRenderParticle(particleModel, MyVector3(1.0f, 0.0f,
    // 0.0f)); MyRenderParticle* particleB = new MyRenderParticle(particleModel, MyVector3(1.0f,
    // 0.0f, 0.0f)); physicsWorld.addParticle(particleA, false); physicsWorld.addParticle(particleB,
    // true); particleA->setOriginalPosition(MyVector3(-2.5f, 0.0f, 0.0f), true);
    // particleA->setRadius(1.0f);
    // particleB->setOriginalPosition(MyVector3(2.5f, 0.0f, 0.0f), true);
    // particleB->setRadius(1.0f);

    // //? Test the Cable's Behaviour
    // //? - Spin : PASS
    // particleA->setIsPersistent(true);
    // MyVector3 cableLength = particleA->getPosition() - particleB->getPosition();
    // physicsWorld.addRod(particleA, particleB, cableLength.getMagnitude());
    // particleB->setMass(100.0f);
    // particleB->addForce(MyVector3(0.0f, 0.0f, 0.0f));
    // //? - Stretch : PASS
    // MyVector3 cableLength = particleA->getPosition() - particleB->getPosition();
    // physicsWorld.addRod(particleA, particleB, cableLength.getMagnitude());
    // particleA->addForce(MyVector3(-1000.0f, 0.0f, 0.0f));
    // particleB->addForce(MyVector3(1000.0f, 0.0f, 0.0f));
    // //? - Compress : PASS
    // MyVector3 cableLength = particleA->getPosition() - particleB->getPosition();
    // physicsWorld.addRod(particleA, particleB, cableLength.getMagnitude());
    // particleA->addForce(MyVector3(1000.0f, 0.0f, 0.0f));
    // particleB->addForce(MyVector3(-1000.0f, 0.0f, 0.0f));
    // //? - Custom Rod Length : PASS
    // double cableLength = 10;  //? Originally 5
    // physicsWorld.addRod(particleA, particleB, cableLength);
    // particleA->addForce(MyVector3(1000.0f, 0.0f, 0.0f));
    // particleB->addForce(MyVector3(-1000.0f, 0.0f, 0.0f));
    // //* Confirmed Working

    // //? Cable Pendulum
    // //? Create the 2 Ends
    // MyRenderParticle* particleA = new MyRenderParticle(particleModel, MyVector3(1.0f, 0.0f,
    // 0.0f)); MyRenderParticle* particleB = new MyRenderParticle(particleModel, MyVector3(0.0f,
    // 0.0f, 1.0f)); physicsWorld.addParticle(particleA, false); physicsWorld.addParticle(particleB,
    // false); particleA->setOriginalPosition(MyVector3(0.0f, 10.0f, -10.0f), true);
    // particleA->setRadius(1.0f);
    // particleB->setOriginalPosition(MyVector3(0.0f, 10.0f, 10.0f), true);
    // particleB->setRadius(1.0f);

    // MyRenderParticle* particleC = new MyRenderParticle(particleModel, MyVector3(0.0f, 1.0f,
    // 0.0f)); physicsWorld.addParticle(particleC, true);
    // particleC->setOriginalPosition(MyVector3(0.0f, 0.0f, 0.0f), true);
    // particleC->setRadius(1.0f);

    // //? Test the Cable's Behaviour
    // //? - Push : PASS
    // particleA->setIsPersistent(true);
    // particleB->setIsPersistent(true);
    // MyVector3 cableALength = particleA->getPosition() - particleC->getPosition();
    // MyVector3 cableBLength = particleB->getPosition() - particleC->getPosition();
    // physicsWorld.addCable(particleA, particleC, cableALength.getMagnitude());
    // physicsWorld.addCable(particleB, particleC, cableBLength.getMagnitude());
    // particleC->setMass(10.0f);
    // particleC->addForce(MyVector3(1000.0f, 0.0f, 0.0f));

    //? Whirly Creation
    //? Create the Pole
    MyRenderParticle* topParticle =
        new MyRenderParticle(particleModel, MyVector3(1.0f, 0.0f, 0.0f));
    MyRenderParticle* bottomParticle =
        new MyRenderParticle(particleModel, MyVector3(0.0f, 0.0f, 1.0f));
    physicsWorld.addParticle(topParticle, false);
    physicsWorld.addParticle(bottomParticle, false);
    // topParticle->setOriginalPosition(MyVector3(0.0f, 40.0f, 0.0f), true);
    // topParticle->setIsPersistent(true);
    topParticle->setPosition(MyVector3(0.0f, 2.0f, 0.0f));
    bottomParticle->setOriginalPosition(MyVector3(0.0f, 0.0f, 0.0f), true);
    bottomParticle->setIsPersistent(true);
    topParticle->setRadius(1.0f);
    topParticle->setMass(60.0f);
    bottomParticle->setRadius(1.0f);
    bottomParticle->setMass(60.0f);

    MyVector3 rodABLength = topParticle->getPosition() - bottomParticle->getPosition();
    MyRod* pole = physicsWorld.addRod(topParticle, bottomParticle, rodABLength.getMagnitude());

    // //? Create the Anchors and Seats
    // double fortyFiveDegree((sqrt(2) / 2) *
    //                        40.0f);  //* evenly spaced out on a circle with a diameter of 80m.

    // MyRenderParticle* contactParticle =
    //     new MyRenderParticle(particleModel, MyVector3(1.0f, 0.0f, 0.0f));

    // vector<MyRenderParticle*> seats;

    // //* "It should seat - 8 people represented by spheres"
    // for (int i = 0; i < 8; i++) {
    //     MyRenderParticle* anchorParticle =
    //         new MyRenderParticle(particleModel, MyVector3(0.5f, 0.5f, 0.5f));
    //     if (i == 0) contactParticle = anchorParticle;
    //     physicsWorld.addParticle(anchorParticle, false);
    //     MyRenderParticle* seatParticle =
    //         new MyRenderParticle(particleModel, MyVector3(1.0f, 1.0f, 0.0f));
    //     physicsWorld.addParticle(seatParticle, true);

    //     seats.push_back(seatParticle);

    //     //? Following the Unit Circle
    //     switch (i) {
    //         case 0:
    //             anchorParticle->setOriginalPosition(MyVector3(40.0f, 40.0f, 0.0f), true);
    //             seatParticle->setPosition(MyVector3(40.0f, 25.0f, 0.0f));
    //             break;
    //         case 1:
    //             anchorParticle->setOriginalPosition(
    //                 MyVector3(fortyFiveDegree, 40.0f, fortyFiveDegree), true);
    //             seatParticle->setPosition(MyVector3(fortyFiveDegree, 25.0f, fortyFiveDegree));
    //             break;
    //         case 2:
    //             anchorParticle->setOriginalPosition(MyVector3(0.0f, 40.0f, 40.0f), true);
    //             seatParticle->setPosition(MyVector3(0.0f, 25.0f, 40.0f));
    //             break;
    //         case 3:
    //             anchorParticle->setOriginalPosition(
    //                 MyVector3(-fortyFiveDegree, 40.0f, fortyFiveDegree), true);
    //             seatParticle->setPosition(MyVector3(-fortyFiveDegree, 25.0f, fortyFiveDegree));
    //             break;
    //         case 4:
    //             anchorParticle->setOriginalPosition(MyVector3(-40.0f, 40.0f, 0.0f), true);
    //             seatParticle->setPosition(MyVector3(-40.0f, 25.0f, 0.0f));
    //             break;
    //         case 5:
    //             anchorParticle->setOriginalPosition(
    //                 MyVector3(-fortyFiveDegree, 40.0f, -fortyFiveDegree), true);
    //             seatParticle->setPosition(MyVector3(-fortyFiveDegree, 25.0f, -fortyFiveDegree));
    //             break;
    //         case 6:
    //             anchorParticle->setOriginalPosition(MyVector3(0.0f, 40.0f, -40.0f), true);
    //             seatParticle->setPosition(MyVector3(0.0f, 25.0f, -40.0f));
    //             break;
    //         case 7:
    //             anchorParticle->setOriginalPosition(
    //                 MyVector3(fortyFiveDegree, 40.0f, -fortyFiveDegree), true);
    //             seatParticle->setPosition(MyVector3(fortyFiveDegree, 25.0f, -fortyFiveDegree));
    //             break;
    //     }
    //     anchorParticle->setRadius(1.0f);
    //     anchorParticle->setLockY(true);
    //     anchorParticle->setMass(60.0f);
    //     seatParticle->setRadius(5.0f);  //* "with a radius of 5m"
    //     seatParticle->setMass(60.0f);   //* "and around 60kg"
    //     MyVector3 anchorRod = topParticle->getPosition() - anchorParticle->getPosition();
    //     physicsWorld.addRod(topParticle, anchorParticle, anchorRod.getMagnitude());
    //     physicsWorld.addCable(anchorParticle, seatParticle, 15.0f);
    // }

    //? Spin the Pole
    // topParticle->addForceAtPoint(
    //     MyVector3(1000.0f, 0.0f, 0.0f),
    //     MyVector3(topParticle->getPosition().x,
    //               topParticle->getPosition().y,
    //               topParticle->getPosition().z - topParticle->getRadius()));
    // contactParticle->addForce(MyVector3(0.0f, 0.0f, 100000.0f));

    // double spinPower = 100000.0f;
    // for (int i = 0; i < 8; i++) {
    //     switch (i) {
    //         case 0:
    //             seats[i]->addForce(MyVector3(0.0f, 0.0f, 40.0f) * spinPower);
    //             break;
    //         case 1:
    //             seats[i]->addForce(MyVector3(-fortyFiveDegree, 0.0f, fortyFiveDegree) *
    //             spinPower); break;
    //         case 2:
    //             seats[i]->addForce(MyVector3(-40.0f, 0.0f, 0.0f) * spinPower);
    //             break;
    //         case 3:
    //             seats[i]->addForce(MyVector3(-fortyFiveDegree, 0.0f, -fortyFiveDegree) *
    //             spinPower); break;
    //         case 4:
    //             seats[i]->addForce(MyVector3(0.0f, 0.0f, -40.0f) * spinPower);
    //             break;
    //         case 5:
    //             seats[i]->addForce(MyVector3(fortyFiveDegree, 0.0f, -fortyFiveDegree) *
    //             spinPower); break;
    //         case 6:
    //             seats[i]->addForce(MyVector3(40.0f, 0.0f, 0.0f) * spinPower);
    //             break;
    //         case 7:
    //             seats[i]->addForce(MyVector3(fortyFiveDegree, 0.0f, fortyFiveDegree) *
    //             spinPower); break;
    //     }

    //* - - - - - END OF PARTICLES - - - - -

    //* - - - - - PRE-RUNTIME - - - - -
    // //? Phase 2 - Newton's Cradle
    // double cableLength;
    // double particleGap;
    // double particleRadius;
    // double gravityStrength;
    // MyVector3 appliedForce;
    // double x;
    // double y;
    // double z;
    // cout << "Cable Length: ";
    // cin >> cableLength;
    // cout << "Particle Gap: ";
    // cin >> particleGap;
    // cout << "Particle Radius: ";
    // cin >> particleRadius;
    // cout << "Gravity Strength: ";
    // cin >> gravityStrength;
    // cout << "Applied Force: " << endl;
    // cout << "x: ";
    // cin >> x;
    // cout << "y: ";
    // cin >> y;
    // cout << "z: ";
    // cin >> z;

    // createNewtonsCradle(particleModel,
    //                     cableLength,
    //                     particleGap,
    //                     particleRadius,
    //                     gravityStrength,
    //                     MyVector3(x, y, z));

    //? PC02 - Carousel Thingy
    bool carouselMade = false;
    //* - - - - - END OF PRE-RUNTIME - - - - -

    //* - - - - - PHYSICS WORLD INITIALIZATION - - - - -
    timerClockStart   = timerClock::now();
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
                if (!carouselMade) {
                    topParticle->setPosition(topParticle->getPosition() +
                                             MyVector3(0.0f, 1.0f, 0.0f));
                    rodABLength  = topParticle->getPosition() - bottomParticle->getPosition();
                    pole->length = rodABLength.getMagnitude();
                }
                if (!carouselMade && topParticle->getPosition().y == 40.0f) {
                    carouselMade = true;
                    topParticle->setOriginalPosition(MyVector3(0.0f, 40.0f, 0.0f), true);
                    topParticle->setIsPersistent(true);
                    makeCarousel(particleModel, topParticle);
                }

                //* - - - - - END OF DEBUGGING - - - - -
                //? Place physics related updates ABOVE this line
                physicsWorld.update((double)millisecond.count() / 1000);
                currentNanosecond -= currentNanosecond;
            }
        } else {
            if (physicsStep) {
                physicsWorld.update(0.1f);
                physicsStep = false;
            }
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

        //* - - - - - PARTICLE LINE RENDERING - - - - -
        for (MyParticleLine* line : physicsWorld.lines) {
            if (SHOW_RENDER_LINES) {
                line->update(activeCamera->getProjection() * activeCamera->getView());
                line->draw();
            }
        }
        //* - - - - - END OF PARTICLE LINE RENDERING - - - - -

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
