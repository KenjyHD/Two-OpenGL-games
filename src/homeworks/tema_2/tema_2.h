#pragma once

#include "components/simple_scene.h"
#include "homeworks/tema_2/lab_camera.h"


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
        Tema2();
        ~Tema2();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

     protected:
        implemented::Camera *camera;
        implemented::Camera* cameraMiniMap;
        glm::mat4 projectionMatrix, miniMapProjectionMatrix;
        bool renderCameraTarget;
        float angle = 0, speed = 0, curvature_coef = 0.003;
        glm::vector<glm::vec3> roadPointsExt;
        glm::vector<glm::vec3> roadR;
        glm::vector<glm::vec3> roadA;
        glm::vector<int> obstaclePositions;
        glm::vector<float> deviationFromCenter;
        glm::vector<int> direction;
        float lastTimeStamp = 0;
        glm::mat4 carMatrix;

        bool PointInTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c);
        bool SameSide(glm::vec3 p1, glm::vec3 p2, glm::vec3 a, glm::vec3 b);
        bool IsOnRoad();
        float VectorAngle(glm::vec3 ptA, glm::vec3 ptB);
        glm::vec3 CalculateDeviatedPosition(glm::vec3 centerPos, glm::vec3 nextCenterPos, float deviation);
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, int cameraType);
        glm::vec3 GetNextPos(int currPos, int step);
        bool CarIntersection();
        void MainScreen(float deltaTimeSeconds);
        void MiniMap(float deltaTimeSeconds);
        };
}
