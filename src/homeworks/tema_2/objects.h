#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace objects
{
    Mesh* CreateTriangle(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = true);
    Mesh* CreateSquare(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = true);
    Mesh* CreateRoad(const std::string& name, float width, glm::vec3 color, glm::vector<glm::vec3> *roadPoints, 
        glm::vector<glm::vec3>* roadR, glm::vector<glm::vec3>* roadA, bool fill);
    Mesh* CreateCar(const std::string& name, glm::vec3 color, bool fill);
    Mesh* CreateTree(const std::string& name, bool fill);
}