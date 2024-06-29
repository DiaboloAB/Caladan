/**********************************************************************************
 * Project: Caladan
 * Description: Multi-platform 2D and 3D game engine
 * Author: Alexis BOITEL
 * Date, Location: 2024, Rennes
 **********************************************************************************/

#include "Model.hpp"

// std
#include <cassert>
#include <cstring>

using namespace Caladan::Renderer;

Model::Model(Device& device, const std::vector<Vertex>& vertices) : _device(device)
{
    createVertexBuffer(vertices);
}

Model::~Model()
{
    vkDestroyBuffer(_device.device(), _vertexBuffer, nullptr);
    vkFreeMemory(_device.device(), _vertexBufferMemory, nullptr);
}

void Model::createVertexBuffer(const std::vector<Vertex>& vertices)
{
    _vertexCount = static_cast<uint32_t>(vertices.size());
    assert(_vertexCount >= 3 && "Vertex count must be at least 3");
    VkDeviceSize bufferSize = sizeof(vertices[0]) * _vertexCount;
    _device.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                         _vertexBuffer, _vertexBufferMemory);

    void* data;
    vkMapMemory(_device.device(), _vertexBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
    vkUnmapMemory(_device.device(), _vertexBufferMemory);
}

void Model::bind(VkCommandBuffer commandBuffer)
{
    VkBuffer buffers[] = {_vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
}

void Model::draw(VkCommandBuffer commandBuffer) { vkCmdDraw(commandBuffer, _vertexCount, 1, 0, 0); }

std::vector<VkVertexInputBindingDescription> Model::Vertex::getBindingDescriptions()
{
    return {
        {0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX},
    };
}

std::vector<VkVertexInputAttributeDescription> Model::Vertex::getAttributeDescriptions()
{
    return {
        {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position)},
        {1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)},
    };
}
