#include "homeworks/tema_1/tema_1.h"

#include <vector>
#include <iostream>



using namespace std;
using namespace m1;

#define DUCK_LENGTH 300
#define BUSH_HEIGHT 200
#define AIM_WIDTH 10
#define FALL_SPEED 500
#define MAX_BASE_SPEED 400
#define MIN_BASE_SPEED 200
#define RANDOM_DIRECTION_CHANCE 500
#define WING_ANGLE 0.2
#define CLOCK_TIME 6
#define DUCK_ROTATION_X 90
#define DUCK_ROTATION_Y -20
#define HITBOX_LENGTH 70
#define HITBOX_WIDTH 60

Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    window->HidePointer();
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    textRender = new gfxc::TextRenderer(window->props.selfDir, resolution.x, resolution.y);
    textRender->Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Hack-Bold.ttf"), 50);

    float squareSide = 100;

    speedMultiplier = 1;
    randomDirectionCD = 0;

    speedX = rand() % ((MAX_BASE_SPEED - MIN_BASE_SPEED) + 1) + MIN_BASE_SPEED;
    speedY = rand() % ((MAX_BASE_SPEED - MIN_BASE_SPEED) + 1) + MIN_BASE_SPEED;
    orientationY = 1;
    orientationX = rand() % (2);
    if (orientationX == 0) {
        orientationX = -1;
    }

    movementDegree = DEGREES(atan((float)speedY / speedX));
    reflexDegreeIncrementation = 180 - DEGREES(atan((float)speedY / speedX)) * 2;

    angularStep1 = 2;
    bulletCount = 3;
    lifeCount = 4;
    score = 0;
    maxScore = 1000;
    duckCount = 0;

    wingState = 0;
    isAlive = 1;
    flyAway = 0;
    
    resolX = window->GetResolution().x;
    resolY = window->GetResolution().y;

    translateX = resolX;
    translateY = resolY + DUCK_LENGTH;

    aimX = 0;
    aimY = 0;

    orientationAngular = 0;

    clk = CLOCK_TIME;

    Mesh* wing = object2D::CreateTriangle("wing", corner, squareSide, glm::vec3(1, 0, 0));
    AddMeshToList(wing);

    Mesh* body = object2D::CreateTriangle("body", corner, squareSide, glm::vec3(0, 0, 1));
    AddMeshToList(body);
    
    Mesh* beak = object2D::CreateTriangle("beak", corner, squareSide, glm::vec3((219.0f / 255), (148.0f / 255), (7.0f / 255)));
    AddMeshToList(beak);

    Mesh* head = object2D::CreateCircle(36, "head", corner, squareSide, glm::vec3(0, 120.0f / 255, 0), true);
    AddMeshToList(head);

    Mesh* bullet = object2D::CreateCircle(36, "bullet", corner, squareSide, glm::vec3((119.0f / 255), (100.0f / 255), (37.0f / 255)), true);
    AddMeshToList(bullet);

    Mesh* life = object2D::CreateCircle(36, "life", corner, squareSide, glm::vec3((255.0f / 255), (0.0f / 255), (0.0f / 255)), true);
    AddMeshToList(life);

    Mesh* bush = object2D::CreateSquare("bush", corner, squareSide, glm::vec3(0, 100.0f / 255, 0));
    AddMeshToList(bush);

    Mesh* aim = object2D::CreateSquare("aim", corner, squareSide, glm::vec3((119.0f / 255), (100.0f / 255), (37.0f / 255)));
    AddMeshToList(aim);

    Mesh* scoreFrame = object2D::CreateSquare("scoreFrame", corner, squareSide, glm::vec3(1, 1, 1), false);
    AddMeshToList(scoreFrame);

    Mesh* scoreBar = object2D::CreateSquare("scoreBar", corner, squareSide, glm::vec3(0, 1, 0));
    AddMeshToList(scoreBar);
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(11.0f / 255, 171.0f/255, 250.0f / 255, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    if (maxScore == score) {
        glClearColor(189.0f / 255, 183.0f / 255, 107.0f / 255, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        textRender->RenderText("Congratulation, we can go on a longer hunt now", 200, 300, 0.6, glm::vec3(1, 1, 1));
        textRender->RenderText("Press Space to start again", 300, 350, 0.8, glm::vec3(1, 1, 1));
    }
    else if (lifeCount < 0) {
        glClearColor(189.0f / 255, 183.0f / 255, 107.0f / 255, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        textRender->RenderText("Game over", 500, 300, 0.8, glm::vec3(1, 1, 1));
        textRender->RenderText("Press Space to start again", 300, 350, 0.8, glm::vec3(1, 1, 1));
    }
    else {
        WingMovement(deltaTimeSeconds);

        BirdOrientation();

        EscapeNDeathLogic(deltaTimeSeconds);

        ManageScoreBar();

        ManageLifesNBullets();

        RenderAim();

        RenderBush();

        BirdAssemblyNMovement();
    }
}

void Tema1::BirdOrientation()
{
    movementDegree = DEGREES(atan((float)speedY / speedX));
    reflexDegreeIncrementation = 180 - DEGREES(atan((float)speedY / speedX)) * 2;

    int randomDirectionChange = -1;

    if (clk - randomDirectionCD >= 2) {
        randomDirectionChange = rand() % RANDOM_DIRECTION_CHANCE;
    }

    if (randomDirectionChange == 0) {
        orientationY *= -1;
        randomDirectionCD = clk;
    }

    if (randomDirectionChange == 1) {
        orientationX *= -1;
        randomDirectionCD = clk;
    }


    if (translateX > (resolX - DUCK_LENGTH / 2)) {
        orientationX = -1;
    }
    if (translateX < -(DUCK_LENGTH / 4)) {
        orientationX = 1;
    }

    if (translateY > resolY && !flyAway) {
        orientationY = -1;
    }
    if (translateY < BUSH_HEIGHT && isAlive) {
        orientationY = 1;
    }

    if (orientationX == 1 && orientationY == 1) {
        orientationAngular = -RADIANS(movementDegree);
    }
    if (orientationX == 1 && orientationY == -1) {
        orientationAngular = RADIANS(movementDegree);
    }
    if (orientationX == -1 && orientationY == -1) {
        orientationAngular = RADIANS(movementDegree + reflexDegreeIncrementation);
    }
    if (orientationX == -1 && orientationY == 1) {
        orientationAngular = -RADIANS(movementDegree + reflexDegreeIncrementation);
    }
}

void Tema1::WingMovement(float deltaTimeSeconds)
{
    if (angularStep1 > WING_ANGLE) {
        wingState = 1;
    }
    else if (angularStep1 < -WING_ANGLE) {
        wingState = 0;
    }

    if (wingState) {
        angularStep1 -= deltaTimeSeconds;
    }
    else {
        angularStep1 += deltaTimeSeconds;
    }
}

void Tema1::EscapeNDeathLogic(float deltaTimeSeconds)
{
    clk += deltaTimeSeconds;

    if (bulletCount <= 0) {
        clk = (CLOCK_TIME + 1);
    }

    if (!isAlive) {
        if (translateY > 0) {
            translateY += (deltaTimeSeconds * FALL_SPEED) * orientationY;
            orientationX = 0;
            orientationY = -1;
            orientationAngular = (2 * M_PI / 4);
        }
        else {
            clk = 0;
            flyAway = 0;
            isAlive = 1;
            bulletCount = 3;
            duckCount++;
            if (duckCount % 5 == 0) {
                speedMultiplier++;
            }
            randomDirectionCD = 0;
            translateX = rand() % (int)resolX;
            translateY = rand() % ((BUSH_HEIGHT - BUSH_HEIGHT / 2) + 1) + BUSH_HEIGHT / 2;
            speedX = rand() % ((MAX_BASE_SPEED - MIN_BASE_SPEED) + 1) + MIN_BASE_SPEED;
            speedY = rand() % ((MAX_BASE_SPEED - MIN_BASE_SPEED) + 1) + MIN_BASE_SPEED;
            for (int i = 0; i < speedMultiplier; i++) {
                speedX += speedX / 5;
                speedY += speedY / 5;
            }
            orientationY = 1;
            orientationX = rand() % (2);
            if (orientationX == 0) {
                orientationX = -1;
            }
        }
    }
    else if (clk <= CLOCK_TIME) {
        translateX += (deltaTimeSeconds * speedX) * orientationX;
        translateY += (deltaTimeSeconds * speedY) * orientationY;
    }
    else {
        flyAway = 1;
        if (translateY < resolY + DUCK_LENGTH / 2) {
            translateY += (deltaTimeSeconds * speedY) * orientationY;
            orientationX = 0;
            orientationY = 1;
            orientationAngular = (6 * M_PI / 4);
        }
        else {
            clk = 0;
            flyAway = 0;
            bulletCount = 3;
            duckCount++;
            lifeCount--;
            if (duckCount % 5 == 0) {
                speedMultiplier++;
            }
            randomDirectionCD = 0;
            speedX = (rand() % ((MAX_BASE_SPEED - MIN_BASE_SPEED) + 1) + MIN_BASE_SPEED);
            speedY = rand() % ((MAX_BASE_SPEED - MIN_BASE_SPEED) + 1) + MIN_BASE_SPEED;
            for (int i = 0; i < speedMultiplier; i++) {
                speedX += speedX / 5;
                speedY += speedY / 5;
            }
            translateX = rand() % (int)resolX;
            translateY = rand() % ((BUSH_HEIGHT - (BUSH_HEIGHT / 2)) + 1) + (BUSH_HEIGHT / 2);
            orientationY = 1;
            orientationX = rand() % (2);
            if (orientationX == 0) {
                orientationX = -1;
            }
        }
    }
}

void Tema1::ManageScoreBar()
{
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(1050, 570);
    modelMatrix *= transform2D::Scale(2, 0.3);
    RenderMesh2D(meshes["scoreFrame"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(1050, 570);
    modelMatrix *= transform2D::Scale(2 * ((float)score / maxScore), 0.3);
    RenderMesh2D(meshes["scoreBar"], shaders["VertexColor"], modelMatrix);
}

void Tema1::ManageLifesNBullets()
{
    for (int i = 0; i < bulletCount; i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(1100 + 50 * i, 650);
        modelMatrix *= transform2D::Scale(0.2, 0.2);
        RenderMesh2D(meshes["bullet"], shaders["VertexColor"], modelMatrix);
    }

    for (int i = 0; i < lifeCount; i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(100 + 50 * i, 650);
        modelMatrix *= transform2D::Scale(0.2, 0.2);
        RenderMesh2D(meshes["life"], shaders["VertexColor"], modelMatrix);
    }
}

void Tema1::RenderBush()
{
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Scale(15, 2);
    RenderMesh2D(meshes["bush"], shaders["VertexColor"], modelMatrix);
}

void Tema1::RenderAim()
{
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(aimX, aimY);
    modelMatrix *= transform2D::Scale(0.1, 0.1);
    RenderMesh2D(meshes["aim"], shaders["VertexColor"], modelMatrix);
}

void Tema1::BirdAssemblyNMovement()
{
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(translateX, translateY);
    modelMatrix *= transform2D::Translate(90, -20);
    modelMatrix *= transform2D::Rotate(-orientationAngular);
    modelMatrix *= transform2D::Translate(-90, 20);

    auxMatrix = modelMatrix;
    modelMatrix *= transform2D::Translate(120, -32);
    modelMatrix *= transform2D::Scale(0.3, 0.3);
    RenderMesh2D(meshes["head"], shaders["VertexColor"], modelMatrix);
    modelMatrix = auxMatrix;

    auxMatrix = modelMatrix;
    modelMatrix *= transform2D::Rotate(-M_PI / 2);
    modelMatrix *= transform2D::Scale(1.5, 1.5);
    RenderMesh2D(meshes["body"], shaders["VertexColor"], modelMatrix);
    modelMatrix = auxMatrix;

    auxMatrix = modelMatrix;
    modelMatrix *= transform2D::Translate(40, -80);
    if (isAlive) {
        modelMatrix *= transform2D::Translate(20, 80);
        modelMatrix *= transform2D::Rotate(angularStep1);
        modelMatrix *= transform2D::Translate(-20, -80);
    }

    modelMatrix *= transform2D::Scale(0.8, 0.8);
    RenderMesh2D(meshes["wing"], shaders["VertexColor"], modelMatrix);
    modelMatrix = auxMatrix;

    auxMatrix = modelMatrix;
    modelMatrix *= transform2D::Translate(72, 22);
    modelMatrix *= transform2D::Rotate(M_PI);
    if (isAlive) {
        modelMatrix *= transform2D::Translate(20, 80);
        modelMatrix *= transform2D::Rotate(-angularStep1);
        modelMatrix *= transform2D::Translate(-20, -80);
    }

    modelMatrix *= transform2D::Scale(0.8, 0.8);
    RenderMesh2D(meshes["wing"], shaders["VertexColor"], modelMatrix);
    modelMatrix = auxMatrix;

    auxMatrix = modelMatrix;
    modelMatrix *= transform2D::Translate(150, -24);
    modelMatrix *= transform2D::Rotate(-M_PI / 2);
    modelMatrix *= transform2D::Scale(0.4, 0.4);
    RenderMesh2D(meshes["beak"], shaders["VertexColor"], modelMatrix);
    modelMatrix = auxMatrix;
}

void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_SPACE && (lifeCount == -1  || score == maxScore)) {
        speedMultiplier = 1;
        bulletCount = 3;
        lifeCount = 3;
        if (maxScore == score) {
            maxScore += 200;
        }
        score = 0;
        duckCount = 0;

        clk = 0;
        flyAway = 0;
        isAlive = 1;
        bulletCount = 3;
        randomDirectionCD = 0;
        translateX = rand() % (int)resolX;
        translateY = rand() % ((BUSH_HEIGHT - BUSH_HEIGHT / 2) + 1) + BUSH_HEIGHT / 2;
        speedX = rand() % ((MAX_BASE_SPEED - MIN_BASE_SPEED) + 1) + MIN_BASE_SPEED;
        speedY = rand() % ((MAX_BASE_SPEED - MIN_BASE_SPEED) + 1) + MIN_BASE_SPEED;
        orientationY = 1;
        orientationX = rand() % (2);
        if (orientationX == 0) {
            orientationX = -1;
        }
    }
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    aimX = mouseX - (AIM_WIDTH / 2);
    aimY = window->GetResolution().y - mouseY - (AIM_WIDTH/2);
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    mouseY = window->GetResolution().y - mouseY;
    if (((mouseX - DUCK_ROTATION_X) < (translateX + HITBOX_LENGTH)) && ((mouseX - DUCK_ROTATION_X) > (translateX - HITBOX_LENGTH))
        && ((mouseY - DUCK_ROTATION_Y) < (translateY + HITBOX_WIDTH)) 
        && ((mouseY - DUCK_ROTATION_Y) > (translateY - HITBOX_WIDTH)) && (bulletCount > 0)) {
        if (isAlive) score += (speedX + speedY) / 10;
        if (score >= maxScore) score = maxScore;
        isAlive = 0;
    }
    else if (isAlive) {
        bulletCount--;
    }
    //cout << (mouseX - 90) << " " << (mouseY + 20) << " " << translateX << " " << translateY << endl;
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
