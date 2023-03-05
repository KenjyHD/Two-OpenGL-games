#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateTriangle(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(40, 0, 0), color),
        VertexFormat(corner + glm::vec3(20, 80, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        //indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateSquare(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateCircle(
    const float steps,
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float radius,
    glm::vec3 color,
    bool fill)
{

    std::vector<VertexFormat> vertices;
    glm::vec3 corner = leftBottomCorner;
    const float angle = M_PI * 2.f / steps;

    float xPos = 0; float yPos = 0;

    float prevX, prevY, newX, newY;
    prevX = xPos;
    prevY = yPos - radius;
    for (int i = 0; i <= steps; i++) {
        newX = radius * sin(angle * i);
        newY = -radius * cos(angle * i);
        vertices.push_back(VertexFormat(corner, color));
        vertices.push_back(VertexFormat(corner + glm::vec3(newX, newY, 0), color));
        vertices.push_back(VertexFormat(corner + glm::vec3(prevX, prevY, 0), color));
        prevX = newX;
        prevY = newY;
    }

    Mesh* circle = new Mesh(name);
    std::vector<unsigned int> indices;


    for (int i = 0; i <= steps; i++) {
        indices.push_back(3 * i);
        indices.push_back(3 * i + 1);
        indices.push_back(3 * i + 2);
    }

    if (!fill) {
        circle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(indices[(indices.size() - 2)]);
        indices.push_back(indices[(indices.size() - 1)]);
    }

    circle->InitFromData(vertices, indices);
    return circle;
}
