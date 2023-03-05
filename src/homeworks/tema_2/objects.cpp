#include "objects.h"
#include "iostream"
#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"
#include <iostream>

Mesh* objects::CreateTriangle(
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
        VertexFormat(corner + glm::vec3(40, 0, 0), color),
        VertexFormat(corner + glm::vec3(20, 80, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        //indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* objects::CreateTree(
    const std::string& name,
        bool fill) {
    glm::vector<glm::vec3> trunk = {
        glm::vec3(0, 0, 0),
        glm::vec3(1, 0, 0),
        glm::vec3(1, 0, 1),
        glm::vec3(0, 0, 1),
        glm::vec3(0, 3, 0),
        glm::vec3(1, 3, 0),
        glm::vec3(1, 3, 1),
        glm::vec3(0, 3, 1)
    };
    glm::vector<glm::vec3> leaves = {
        glm::vec3(-1, 3, -1),
        glm::vec3(2, 3, -1),
        glm::vec3(2, 3, 2),
        glm::vec3(-1, 3, 2),
        glm::vec3(-1, 4.5, -1),
        glm::vec3(2, 4.5, -1),
        glm::vec3(2, 4.5, 2),
        glm::vec3(-1, 4.5, 2),
    };
    std::vector<VertexFormat> vertices;
    glm::vec3 trunkColor = glm::vec3(92 / 255.f, 64 / 255.f, 51 / 255.f);
    glm::vec3 leavesColor = glm::vec3(50 / 138.f, 205 / 255.f, 50 / 255.f);
    for (int i = 0; i < trunk.size(); i++) {
        vertices.push_back(VertexFormat(trunk.at(i), trunkColor));
    }
    for (int i = 0; i < leaves.size(); i++) {
        vertices.push_back(VertexFormat(leaves.at(i), leavesColor));
    }

    Mesh* tree = new Mesh(name);
    std::vector<unsigned int> indices = {
        // tree
        0, 1, 2,
        0, 2, 3,
        0, 4, 3,
        3, 4, 7,
        0, 1, 4,
        1, 4, 5,
        1, 2, 5,
        2, 5, 6,
        2, 3, 6,
        3, 6, 7,
        4, 5, 6,
        4, 6, 7,
        // leaves
        8, 9, 10,
        8, 10, 11,
        8, 12, 11,
        11, 12, 15,
        8, 9, 12,
        9, 12, 13,
        9, 10, 13,
        10, 13, 14,
        10, 11, 14,
        11, 14, 15,
        12, 13, 14,
        12, 14, 15
    };

    for (int i = 0; i < trunk.size(); i++) {
        vertices.push_back(VertexFormat(trunk.at(i), trunkColor));
    }

    if (!fill) {
        tree->SetDrawMode(GL_LINE_LOOP);
    }

    tree->InitFromData(vertices, indices);
    return tree;
}

Mesh* objects::CreateRoad(
    const std::string& name,
    float width,
    glm::vec3 color,
    glm::vector<glm::vec3> *roadPointsExt,
    glm::vector<glm::vec3> *roadR,
    glm::vector<glm::vec3> *roadA,
    bool fill)
{
    glm::vector<glm::vec3> roadPoints = {
        glm::vec3(6.44, 0, 3.05),
        glm::vec3(5.19, 0, 6.12),
        glm::vec3(4.49, 0, 9.26),
        glm::vec3(4.7, 0, 12.4),
        glm::vec3(6.79, 0, 16.23),
        glm::vec3(9.37, 0, 18.42),
        glm::vec3(11.75, 0, 18.75),
        glm::vec3(13.49, 0, 18.08),
        glm::vec3(16.14, 0, 17.69),
        glm::vec3(18.39, 0, 18.64),
        glm::vec3(19.7, 0, 22.3),
        glm::vec3(19.21, 0, 27.05),
        glm::vec3(20.26, 0, 31.17),
        glm::vec3(23.05, 0, 34.31),
        glm::vec3(27.52, 0, 35.56),
        glm::vec3(32.96, 0, 35.14),
        glm::vec3(38.61, 0, 34.51),
        glm::vec3(40.84, 0, 31.86),
        glm::vec3(41.26, 0, 29.28),
        glm::vec3(42, 0, 26.77),
        glm::vec3(43.89, 0, 25.77),
        glm::vec3(46.45, 0, 26.44),
        glm::vec3(48.46, 0, 29),
        glm::vec3(52.98, 0, 31.72),
        glm::vec3(56.19, 0, 33.75),
        glm::vec3(61.5, 0, 33.61),
        glm::vec3(63.73, 0, 30.89),
        glm::vec3(64.29, 0, 27.4),
        glm::vec3(64.5, 0, 23.14),
        glm::vec3(65.82, 0, 18.89),
        glm::vec3(66.24, 0, 14.77),
        glm::vec3(65.00, 0, 10),
        glm::vec3(63.87, 0, 6.54),
        glm::vec3(61.29, 0, 3.12),
        glm::vec3(58.96, 0, 1.97),
        glm::vec3(56.3, 0, 1.5),
        glm::vec3(52.77, 0, 2.42),
        glm::vec3(48.8, 0, 4.72),
        glm::vec3(45.87, 0, 6.81),
        glm::vec3(43.62, 0, 7.53),
        glm::vec3(40.96, 0, 7.29),
        glm::vec3(38.26, 0, 5.35),
        glm::vec3(35.68, 0, 1.93),
        glm::vec3(33.17, 0, 0.53),
        glm::vec3(31.91, 0, -2.05),
        glm::vec3(28.91, 0, -5.88),
        glm::vec3(26.33, 0, -8.54),
        glm::vec3(21.91, 0, -10.08),
        glm::vec3(17.21, 0, -10.54),
        glm::vec3(12.06, 0, -9.05),
        glm::vec3(9.58, 0, -6.23),
        glm::vec3(7.91, 0, -2.61),
        glm::vec3(7.07, 0, 0.26)
    };
    int roadSize = roadPoints.size();
    int pointNum = 1000;
    for (int i = 0; i < roadSize - 1; i++) {
        float diff_X = roadPoints.at(i + 1).x - roadPoints.at(i).x;
        float diff_Z = roadPoints.at(i + 1).z - roadPoints.at(i).z;
        

        float interval_X = diff_X / (pointNum + 1);
        float interval_Z = diff_Z / (pointNum + 1);

        for (int j = 1; j <= pointNum; j++)
        {
            (*roadPointsExt).push_back(glm::vec3(roadPoints.at(i).x + interval_X * j, 0, roadPoints.at(i).z + interval_Z * j));
        }
        
    }

    float diff_X = roadPoints.at(0).x - roadPoints.at(roadSize - 1).x;
    float diff_Z = roadPoints.at(0).z - roadPoints.at(roadSize - 1).z;

    float interval_X = diff_X / (pointNum + 1);
    float interval_Z = diff_Z / (pointNum + 1);

    for (int j = 1; j <= pointNum; j++)
    {
        (*roadPointsExt).push_back(glm::vec3(roadPoints.at(roadSize - 1).x + interval_X * j, 0, roadPoints.at(roadSize - 1).z + interval_Z * j));
    }

    int sizeExt = (*roadPointsExt).size();

    glm::vector<glm::vec3> r, a;

    for (int i = 0; i < sizeExt; i++) {
        glm::vec3 d;
        if (i + 1 == sizeExt) {
            d = (*roadPointsExt).at(i) - (*roadPointsExt).at(0);
        }
        else {
            d = (*roadPointsExt).at(i) - (*roadPointsExt).at(i + 1);
        }    
        glm::vec3 p = glm::normalize(cross(d, glm::vec3(0, 1, 0)));
        (*roadR).push_back((*roadPointsExt).at(i) + p * glm::vec3(width));
        (*roadA).push_back((*roadPointsExt).at(i) - p * glm::vec3(width));
    }

    std::vector<VertexFormat> vertices;
    for (int i = 0; i < sizeExt; i++) {
        vertices.push_back(VertexFormat((*roadR).at(i), color));
        vertices.push_back(VertexFormat((*roadA).at(i), color));
    }
    Mesh* road = new Mesh(name);
    std::vector<unsigned int> indices;
    int i = 0;
    while (i < sizeExt * 2 - 2) {
        indices.push_back(i);
        indices.push_back(i + 1);
        indices.push_back(i + 3);
        indices.push_back(i + 2);
        indices.push_back(i);
        indices.push_back(i + 3);
        i += 2;
    }
    indices.push_back(sizeExt * 2 - 2);
    indices.push_back(sizeExt * 2 - 1);
    indices.push_back(1);
    indices.push_back(0);
    indices.push_back(sizeExt * 2 - 2);
    indices.push_back(1);

    if (!fill) {
        road->SetDrawMode(GL_LINE_LOOP);
    }
    

    road->InitFromData(vertices, indices);
    return road;
}

Mesh* objects::CreateSquare(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    int squares = 100;
    glm::vector<glm::vec3> points;
    for (int i = 0; i <= squares; i++) {
        for (int j = 0; j <= squares; j++) {
            points.push_back(glm::vec3(((float)j / squares) * length, 0,((float)i / squares) * length));
        }
    }

    std::vector<VertexFormat> vertices;

    for (int i = 0; i <= squares * (squares + 2); i++) {
        vertices.push_back(VertexFormat(points.at(i), color));
    }

    std::vector<unsigned int> indices;

    for (int i = 0; i < squares; i++) {
        for (int j = 0; j < squares; j++) {
            indices.push_back(i * squares + i + j);
            indices.push_back(i * squares + i + j + 1);
            indices.push_back(i * squares + i + j + squares + 1);

            indices.push_back(i * squares + i + j + 1);
            indices.push_back(i * squares + i + j + squares + 2);
            indices.push_back(i * squares + i + j + squares + 1);
        }
    }
    
    Mesh* square = new Mesh(name);
    

    if (!fill) {
        square->SetDrawMode(GL_LINE_STRIP);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* objects::CreateCar(
    const std::string& name,
    glm::vec3 color,
    bool fill) {

    glm::vector<glm::vec3> points = {
        glm::vec3(-1, -0.5, -0.5),
        glm::vec3(1, -0.5, -0.5),
        glm::vec3(1, -0.5, 0.5),
        glm::vec3(-1, -0.5, 0.5),
        glm::vec3(-1, 0.5, -0.5),
        glm::vec3(1, 0.5, -0.5),
        glm::vec3(1, 0.5, 0.5),
        glm::vec3(-1, 0.5, 0.5)
    };

    int size = points.size();
    std::vector<VertexFormat> vertices;
    for (int i = 0; i < size; i++) {
        vertices.push_back(VertexFormat(points.at(i), color));
    }
    Mesh* road = new Mesh(name);
    std::vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3,
        0, 4, 3,
        3, 4, 7,
        0, 1, 4,
        1, 4, 5,
        1, 2, 5,
        2, 5, 6,
        2, 3, 6,
        3, 6, 7,
        4, 5, 6,
        4, 6, 7
    };

    if (!fill) {
        road->SetDrawMode(GL_LINE_LOOP);
    }


    road->InitFromData(vertices, indices);
    return road;
}