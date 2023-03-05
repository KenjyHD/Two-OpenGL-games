#include "homeworks/tema_2/tema_2.h"
#include "homeworks/tema_2/objects.h"

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;
using namespace m1;

#define GROUND_WIDTH 500
#define ROAD_SCALE 2
#define ROAD_WIDTH 1
#define OBSTACLES_NR 50
#define MOVE_SPEED 20
#define ACC_FACTOR 3
constexpr float CAR_MASS = MOVE_SPEED / 30.f;

Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    speed = 0;
    cameraMiniMap = new implemented::Camera();
    cameraMiniMap->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, -200, 0), glm::vec3(0, 1, 0));
    
    camera = new implemented::Camera();
    camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
    angle = RADIANS(-20);
    camera->RotateThirdPerson_OY(angle);
    camera->MoveForward(5);
    camera->TranslateRight(12);
    camera->TranslateUpward(-1.31);

    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
    
    miniMapProjectionMatrix = glm::ortho(-10.0f, 10.0f, -5.f, 5.f, 0.0f, 200.0f);

    Mesh* ground = objects::CreateSquare("ground", glm::vec3(0, 0, 0), GROUND_WIDTH, glm::vec3(0, 100.0f / 255, 0), true);
    AddMeshToList(ground);

    Mesh* roads = objects::CreateRoad("roads", ROAD_WIDTH, glm::vec3(0, 0, 0), &roadPointsExt, &roadR, &roadA, true);
    AddMeshToList(roads);

    Mesh* tree = objects::CreateTree("tree", true);
    AddMeshToList(tree);

    Mesh* car = objects::CreateCar("car", glm::vec3(37 / 255.f, 58 / 255.f, 167 / 255.f), true);
    AddMeshToList(car);

    Mesh* mesh = new Mesh("box");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[mesh->GetMeshID()] = mesh;

    for (int i = 0; i < OBSTACLES_NR; i++) {
        obstaclePositions.push_back((roadPointsExt.size() / OBSTACLES_NR) * i);
        float maxDeviation = ROAD_WIDTH * 0.7;
        int rand2 = rand() % 2;
        if (rand2 == 0) rand2 = -1;
        direction.push_back(rand2);
        float rand1 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / maxDeviation));
        deviationFromCenter.push_back(rand1 * rand2);
    }

    Shader* shader = new Shader("LabShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::HOMEWORKS, "tema_2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::HOMEWORKS, "tema_2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;
}


void Tema2::FrameStart()
{
    glClearColor(135 / 255.f, 206 / 255.f, 235 / 255.f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
{
    MainScreen(deltaTimeSeconds);

    MiniMap(deltaTimeSeconds);
}

void Tema2::MainScreen(float deltaTimeSeconds) {
    float elapsedTime = (float)Engine::GetElapsedTime();
    
    if (CarIntersection() || !IsOnRoad()) {
        speed = 0;
    }
    // ground
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-150, 0, -150));
        carMatrix = modelMatrix;
        RenderSimpleMesh(meshes["ground"], shaders["LabShader"], modelMatrix, 0);
    }
    // trees
    for (int i = 0; i < roadPointsExt.size() - 1000; i += 1000) {
        glm::vec3 ptA = glm::vec3(roadPointsExt.at(i).x, 0, roadPointsExt.at(i).z);
        glm::vec3 ptB = GetNextPos(i, 1000);
        glm::vec3 posOnRoad = CalculateDeviatedPosition(ptA, ptB / glm::vec3(2), 1.5);

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(posOnRoad.x, -0.1, posOnRoad.z));
        modelMatrix *= glm::scale(modelMatrix, glm::vec3(0.5));
        RenderSimpleMesh(meshes["tree"], shaders["LabShader"], modelMatrix, 0);

        posOnRoad = CalculateDeviatedPosition(ptA, ptB / glm::vec3(2), -1.5);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(posOnRoad.x, -0.1, posOnRoad.z));
        modelMatrix *= glm::scale(modelMatrix, glm::vec3(0.5));
        RenderSimpleMesh(meshes["tree"], shaders["LabShader"], modelMatrix, 0);
    }
    // road
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.02, 0));
        modelMatrix *= glm::scale(modelMatrix, glm::vec3(ROAD_SCALE));
        RenderSimpleMesh(meshes["roads"], shaders["LabShader"], modelMatrix, 0);
    }
    // main car
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition() + glm::vec3(0, -0.3, 0));
        modelMatrix = glm::rotate(modelMatrix, angle + (float)(M_PI / 2), glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2, 0.2, 0.2));
        RenderSimpleMesh(meshes["car"], shaders["LabShader"], modelMatrix, 0);
    }
    // obstacles
    for (int i = 0; i < OBSTACLES_NR; i++) {
        float x = roadPointsExt.at(obstaclePositions.at(i)).x * ROAD_SCALE;
        float z = roadPointsExt.at(obstaclePositions.at(i)).z * ROAD_SCALE;

        glm::vec3 ptA = glm::vec3(x, 0, z);
        glm::vec3 ptB = GetNextPos(obstaclePositions.at(i), 10);
        glm::vec3 posOnRoad = CalculateDeviatedPosition(ptA, ptB, deviationFromCenter.at(i));

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(posOnRoad.x, 0.14, posOnRoad.z));
        modelMatrix = glm::rotate(modelMatrix, VectorAngle(ptA, ptB), glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2, 0.22, 0.2));
        RenderSimpleMesh(meshes["car"], shaders["LabShader"], modelMatrix, 0);
    }

    if (elapsedTime - lastTimeStamp > deltaTimeSeconds / 2) {
        lastTimeStamp = elapsedTime;
        int size = roadPointsExt.size();
        for (int i = 0; i < OBSTACLES_NR; i++) {
            obstaclePositions.at(i) += 6 * direction.at(i);
            if (obstaclePositions.at(i) >= size) {
                obstaclePositions.at(i) -= size;
            }
            else if (obstaclePositions.at(i) < 0) {
                obstaclePositions.at(i) += size;
            }
        }
    }
}

void Tema2::MiniMap(float deltaTimeSeconds) {
    cameraMiniMap->SetPosition(camera->GetTargetPosition() + glm::vec3(0, 10, 0));
    glm::ivec2 resolution = window->GetResolution();
    float minimapSizeX = resolution.x / 4;
    float minimapSizeY = resolution.y / 4;
    glViewport(resolution.x - minimapSizeX, 0, minimapSizeX, minimapSizeY);
    
    float elapsedTime = (float)Engine::GetElapsedTime();
    // ground
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-150, 0, -150));
        RenderSimpleMesh(meshes["ground"], shaders["VertexColor"], modelMatrix, 1);
    }
    // trees
    for (int i = 0; i < roadPointsExt.size() - 1000; i += 1000) {
        glm::vec3 ptA = glm::vec3(roadPointsExt.at(i).x, 0, roadPointsExt.at(i).z);
        glm::vec3 ptB = GetNextPos(i, 1000);
        glm::vec3 posOnRoad = CalculateDeviatedPosition(ptA, ptB / glm::vec3(2), 1.5);

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(posOnRoad.x, 0, posOnRoad.z));
        modelMatrix *= glm::scale(modelMatrix, glm::vec3(0.5));
        RenderSimpleMesh(meshes["tree"], shaders["VertexColor"], modelMatrix, 1);

        posOnRoad = CalculateDeviatedPosition(ptA, ptB / glm::vec3(2), -1.5);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(posOnRoad.x, 0, posOnRoad.z));
        modelMatrix *= glm::scale(modelMatrix, glm::vec3(0.5));
        RenderSimpleMesh(meshes["tree"], shaders["VertexColor"], modelMatrix, 1);
    }
    // road
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.02, 0));
        modelMatrix *= glm::scale(modelMatrix, glm::vec3(ROAD_SCALE));
        RenderSimpleMesh(meshes["roads"], shaders["VertexColor"], modelMatrix, 1);
    }
    // main car
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition() + glm::vec3(0, -0.3, 0));
        modelMatrix = glm::rotate(modelMatrix, angle + (float)(M_PI / 2), glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2, 0.2, 0.2));
        RenderSimpleMesh(meshes["car"], shaders["VertexColor"], modelMatrix, 1);
    }
    // obstacles
    for (int i = 0; i < OBSTACLES_NR; i++) {
        float x = roadPointsExt.at(obstaclePositions.at(i)).x * ROAD_SCALE;
        float z = roadPointsExt.at(obstaclePositions.at(i)).z * ROAD_SCALE;

        glm::vec3 ptA = glm::vec3(x, 0, z);
        glm::vec3 ptB = GetNextPos(obstaclePositions.at(i), 10);
        glm::vec3 posOnRoad = CalculateDeviatedPosition(ptA, ptB, deviationFromCenter.at(i));

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(posOnRoad.x, 0.17, posOnRoad.z));
        modelMatrix = glm::rotate(modelMatrix, VectorAngle(ptA, ptB), glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2, 0.2, 0.2));
        RenderSimpleMesh(meshes["car"], shaders["VertexColor"], modelMatrix, 1);
    }
    // move obstacles
    if (elapsedTime - lastTimeStamp > deltaTimeSeconds) {
        lastTimeStamp = elapsedTime;
        int size = roadPointsExt.size();
        for (int i = 0; i < OBSTACLES_NR; i++) {
            obstaclePositions.at(i) += 10 * direction.at(i);
            if (obstaclePositions.at(i) >= size) {
                obstaclePositions.at(i) -= size;
            }
            else if (obstaclePositions.at(i) < 0) {
                obstaclePositions.at(i) += size;
            }
        }
    }
}

void Tema2::FrameEnd()
{
}

void Tema2::RenderSimpleMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix, int cameraType)
{
    if (!mesh || !shader || !shader->program)
        return;

    glUseProgram(shader->program);

    int location_model = glGetUniformLocation(shader->program, "Model");

    glUniformMatrix4fv(location_model, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    int location_view = glGetUniformLocation(shader->program, "View");

    glm::mat4 viewMatrix;
    if (cameraType == 0) {
        viewMatrix = camera->GetViewMatrix();
    }
    else {
        viewMatrix = cameraMiniMap->GetViewMatrix();
    }
    glUniformMatrix4fv(location_view, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    int location_projection = glGetUniformLocation(shader->program, "Projection");

    glm::mat4 projectionMat;
    int location;
    
    if (cameraType == 0) {
        projectionMat = projectionMatrix;
    }
    else {
        projectionMat = miniMapProjectionMatrix;
    }
    location = glGetUniformLocation(shader->program, "carPos");
    glm::vec4 carPos = projectionMatrix * viewMatrix * glm::vec4(camera->GetTargetPosition(), 1);
    glUniform4fv(location, 1, glm::value_ptr(carPos));

    glUniform1d(glGetUniformLocation(shader->program, "CameraType"), cameraType);

    glUniform1f(glGetUniformLocation(shader->program, "Curvature"), curvature_coef);

    glUniformMatrix4fv(location_projection, 1, GL_FALSE, glm::value_ptr(projectionMat));

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    float rotationSpeed = 2;

    static float acc = 0;
    if (window->KeyHold(GLFW_KEY_W) && !CarIntersection()) {
        if (speed < 0) {
            speed += deltaTime * ACC_FACTOR * 15;
        }
        else {
            speed += deltaTime * ACC_FACTOR;
        }
        acc = std::sqrt(abs(speed));
        if (speed < 0) acc *= -1;
        if (acc > MOVE_SPEED) {
            acc = MOVE_SPEED;
        }
        camera->MoveForward(deltaTime * acc);
        if (!IsOnRoad()) {
            camera->MoveForward(-deltaTime * acc);
            speed = 0;
            acc = 0;
        }  
    }
    else if (window->KeyHold(GLFW_KEY_S) && !CarIntersection()) {
        if (speed > 0) {
            speed -= deltaTime * ACC_FACTOR * 15;
        }
        else {
            speed -= deltaTime * ACC_FACTOR;
        }
        acc = std::sqrt(abs(speed));
        if (speed < 0) acc *= -1;
        if (acc < -MOVE_SPEED) {
            acc = -MOVE_SPEED;
        }
        camera->MoveForward(deltaTime * acc);
        if (!IsOnRoad()) {
            camera->MoveForward(-deltaTime * acc);
            speed = 0;
            acc = 0;
        }
    }
    else {
        if (speed > 0) {
            speed -= deltaTime * ACC_FACTOR * 5;
            if (speed < 0) speed = 0;
            acc = std::sqrt(abs(speed));
            camera->MoveForward(deltaTime * acc);
        }
        else if (speed < 0) {
            speed += deltaTime * ACC_FACTOR * 5;
            if (speed > 0) speed = 0;
            acc = -std::sqrt(abs(speed));
            camera->MoveForward(deltaTime * acc);
        }
    }
    if (window->KeyHold(GLFW_KEY_A) && !CarIntersection()) {
        camera->RotateThirdPerson_OY(rotationSpeed * deltaTime);
        angle += rotationSpeed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_D) && !CarIntersection()) {
        camera->RotateThirdPerson_OY(-rotationSpeed * deltaTime);
        angle -= rotationSpeed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_EQUAL)) {
        curvature_coef += 0.001;
    }
    if (window->KeyHold(GLFW_KEY_MINUS)) {
        curvature_coef -= 0.001;
    }
}

void Tema2::OnKeyPress(int key, int mods)
{
}

void Tema2::OnKeyRelease(int key, int mods)
{
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Use this to change camera orientation position
    //if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    //{
    //    float sensivityOX = 0.001f;
    //    float sensivityOY = 0.001f;

    //    if (window->GetSpecialKeyState() == 0) {
    //        renderCameraTarget = false;
    //        camera->RotateFirstPerson_OX(-2 * sensivityOX * deltaY);
    //        camera->RotateFirstPerson_OY(-2 * sensivityOY * deltaX);
    //        /*cameraMiniMap->RotateFirstPerson_OX(-2 * sensivityOX * deltaY);
    //        cameraMiniMap->RotateFirstPerson_OY(-2 * sensivityOY * deltaX);*/

    //    }

    //    if (window->GetSpecialKeyState()) {
    //        renderCameraTarget = true;
    //        camera->RotateThirdPerson_OX(-2 * sensivityOX * deltaY);
    //        camera->RotateThirdPerson_OY(-2 * sensivityOY * deltaX);
    //        /*cameraMiniMap->RotateThirdPerson_OX(-2 * sensivityOX * deltaY);
    //        cameraMiniMap->RotateThirdPerson_OY(-2 * sensivityOY * deltaX);*/
    //    }
    //}
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}

bool Tema2::IsOnRoad() {
    int size = roadA.size();
    glm::vec3 p = glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z);
    glm::vec3 a;
    glm::vec3 b;
    glm::vec3 c;
    glm::vec3 d;
    int i = 0;
    while (i < size - 100) {
        a = roadR.at(i) * glm::vec3(ROAD_SCALE);
        b = roadA.at(i) * glm::vec3(ROAD_SCALE);
        c = roadA.at(i + 100) * glm::vec3(ROAD_SCALE);
        d = roadR.at(i + 100) * glm::vec3(ROAD_SCALE);

        if (PointInTriangle(p, a, b, c) || PointInTriangle(p, c, d, a)) {
            return true;
        }

        i += 100;
    }
    a = roadR.at(i) * glm::vec3(ROAD_SCALE);
    b = roadA.at(i) * glm::vec3(ROAD_SCALE);
    c = roadA.at(0) * glm::vec3(ROAD_SCALE);
    d = roadR.at(0) * glm::vec3(ROAD_SCALE);
    if (PointInTriangle(p, a, b, c) || PointInTriangle(p, c, d, a)) {
        return true;
    }
    return false;
}

float Tema2::VectorAngle(glm::vec3 ptA, glm::vec3 ptB)
{
    float delta_x = ptB.x - ptA.x;
    float delta_y = ptB.z - ptA.z;
    return -atan2(delta_y, delta_x);
}

bool Tema2::CarIntersection() {
    
    float carX = camera->GetTargetPosition().x;
    float carZ = camera->GetTargetPosition().z;
    for (int i = 0; i < OBSTACLES_NR; i++) {
        glm::vec3 nextPos = GetNextPos(obstaclePositions.at(i), 10);
        float obstacleX = roadPointsExt.at(obstaclePositions.at(i)).x * ROAD_SCALE;
        float obstacleZ = roadPointsExt.at(obstaclePositions.at(i)).z * ROAD_SCALE;
        glm::vec3 deviatedPos = CalculateDeviatedPosition(glm::vec3(obstacleX, 0, obstacleZ), nextPos, deviationFromCenter.at(i));
        float distance = sqrt((carX - deviatedPos.x) * (carX - deviatedPos.x) + (carZ - deviatedPos.z) * (carZ - deviatedPos.z));
        if (distance < 0.4) return true;
    }
    return false;
}

bool Tema2::SameSide(glm::vec3 p1, glm::vec3 p2, glm::vec3 a, glm::vec3 b) {
    glm::vec3 cp1 = cross(b - a, p1 - a);
    glm::vec3 cp2 = cross(b - a, p2 - a);
    if (dot(cp1, cp2) >= 0) {
        return true;
    }
    else {
        return false;
    }
}

bool Tema2::PointInTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    if (SameSide(p, a, b, c) && SameSide(p, b, a, c) && SameSide(p, c, a, b)) {
        return true;
    }
    else {
        return false;
    }
}

glm::vec3 Tema2::CalculateDeviatedPosition(glm::vec3 centerPos, glm::vec3 nextCenterPos, float deviation) {
    glm::vec3 d = nextCenterPos - centerPos;
    glm::vec3 p = glm::normalize(cross(d, glm::vec3(0, 1, 0)));
    return centerPos + deviation * (float)ROAD_SCALE * p;
}

glm::vec3 Tema2::GetNextPos(int currPos, int step) {
    glm::vec3 ptB;
    if (currPos + step >= roadPointsExt.size()) {
        int nextPos = currPos + step - roadPointsExt.size();
        ptB = glm::vec3(roadPointsExt.at(nextPos).x * ROAD_SCALE, 0, roadPointsExt.at(nextPos).z * ROAD_SCALE);
    }
    else {
        int nextPos = currPos + step;
        ptB = glm::vec3(roadPointsExt.at(nextPos).x * ROAD_SCALE, 0, roadPointsExt.at(nextPos).z * ROAD_SCALE);
    }
    return ptB;
}