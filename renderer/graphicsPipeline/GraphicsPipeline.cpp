/**********************************************************************************
 * Project: Caladan
 * Description: Multi-platform 2D and 3D game engine
 * Author: Alexis BOITEL
 * Date, Location: 2024, Rennes
 **********************************************************************************/

#include "GraphicsPipeline.hpp"

// std
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace Caladan::Renderer;

GraphicsPipeline::GraphicsPipeline(const std::string& vertexShaderPath,
                                   const std::string& fragmentShaderPath)
{
    createGraphicsPipeline(vertexShaderPath, fragmentShaderPath);
}

GraphicsPipeline::~GraphicsPipeline()
{
    // Destructor implementation
}

std::vector<char> GraphicsPipeline::ReadFile(const std::string& filePath)
{
    std::ifstream file(filePath, std::ios::ate | std::ios::binary);
    size_t fileSize = 0;

    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file: " + filePath);
    }
    fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    return buffer;
}

void GraphicsPipeline::createGraphicsPipeline(const std::string& vertexShaderPath,
                                              const std::string& fragmentShaderPath)
{
    auto vertexShaderCode = ReadFile(vertexShaderPath);
    auto fragmentShaderCode = ReadFile(fragmentShaderPath);

    std::cout << "Vertex shader code size: " << vertexShaderCode.size() << std::endl;
    std::cout << "Fragment shader code size: " << fragmentShaderCode.size() << std::endl;
}