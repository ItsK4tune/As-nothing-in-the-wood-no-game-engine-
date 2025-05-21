#pragma once

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <glm/glm.hpp>

#include "util/struct/struct.h"

void loadVertexFile(const std::string &filepath, std::vector<Vertex> &vertices);