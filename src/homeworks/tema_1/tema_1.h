#pragma once

#include "components/simple_scene.h"
#include "components/text_renderer.h"

#include "homeworks/tema_1/transform2D.h"
#include "homeworks/tema_1/object2D.h"

namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
     public:
        Tema1();
        ~Tema1();

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
        void BirdOrientation();
        void WingMovement(float deltaTimeSeconds);
        void EscapeNDeathLogic(float deltaTimeSeconds);
        void ManageScoreBar();
        void ManageLifesNBullets();
        void RenderBush();
        void RenderAim();
        void BirdAssemblyNMovement();

     protected:
        float cx, cy;
        glm::mat3 modelMatrix;
        glm::mat3 auxMatrix;
        float translateX, translateY, aimX, aimY;
        float scaleX, scaleY;
        float angularStep1;
        int wingState;
        int orientationX, orientationY;
        float orientationAngular, speedMultiplier;
        float clk, reflexDegreeIncrementation, movementDegree, randomDirectionCD;
        float resolX, resolY;
        int isAlive, flyAway;
        int speedX, speedY, bulletCount, lifeCount, score, maxScore, duckCount;
        gfxc::TextRenderer* textRender;


    };
}   // namespace m1
