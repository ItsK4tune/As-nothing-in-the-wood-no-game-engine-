#include "util/loadVertex.h"
#include "util/struct/struct.h"

void loadVertexFile(const std::string &filepath, std::vector<Vertex> &vertices)
{
    std::string fullPath = "../vertex/" + filepath;
    std::ifstream file(fullPath);
    if (!file.is_open())
    {
        std::cerr << "Cannot open file: " << fullPath << std::endl;
        return;
    }

    if (vertices.size())
    {
        vertices.clear();
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line.find("//") == 0)
            continue;

        std::istringstream iss(line);
        float x, y, z;
        if (!(iss >> x >> y >> z))
        {
            std::cerr << "Format error at: " << line << std::endl;
            continue;
        }

        Vertex vertex;
        vertex.pos = glm::vec3(x, y, z);
        vertex.uv = glm::vec2(0.0f);
        vertex.normal = glm::vec3(0.0f);
        vertex.color = glm::vec3(0.0f);

        vertices.push_back(vertex);
    }

    if (vertices.size() % 3 != 0)
    {
        std::cerr << "Vertex data is not a multiple of 3, possibly malformed file: " << fullPath << std::endl;
    }

    file.close();
    return;
}