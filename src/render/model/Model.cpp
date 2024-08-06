/**********************************************************************************
 * Project: Caladan
 * Description: Multi-platform 2D and 3D game engine
 * Author: Alexis BOITEL
 * Date, Location: 2024, Rennes
 **********************************************************************************/

#define TINYOBJLOADER_IMPLEMENTATION  // Define this in exactly one source file
#include <tiny_obj_loader.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "Model.hpp"
#include "utils.hpp"

// std
#include <cassert>
#include <cstring>
#include <iostream>
#include <unordered_map>

namespace std
{
template <>
struct hash<Caladan::Render::Model::Vertex>
{
    size_t operator()(Caladan::Render::Model::Vertex const& vertex) const
    {
        size_t seed = 0;
        Caladan::hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.texCoord);
        return seed;
    }
};
}  // namespace std

using namespace Caladan::Render;

Model::Model(Device& device, const Model::Data& Data) : _device(device)
{
    createVertexBuffer(Data.vertices);
    createIndexBuffer(Data.indices);
}

Model::~Model()
{
    vkDestroyBuffer(_device.device(), _vertexBuffer, nullptr);
    vkFreeMemory(_device.device(), _vertexBufferMemory, nullptr);

    if (_hasIndexBuffer)
    {
        vkDestroyBuffer(_device.device(), _indexBuffer, nullptr);
        vkFreeMemory(_device.device(), _indexBufferMemory, nullptr);
    }
}

void Model::createVertexBuffer(const std::vector<Vertex>& vertices)
{
    _vertexCount = static_cast<uint32_t>(vertices.size());
    assert(_vertexCount >= 3 && "Vertex count must be at least 3");
    VkDeviceSize bufferSize = sizeof(vertices[0]) * _vertexCount;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    _device.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                         stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(_device.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
    vkUnmapMemory(_device.device(), stagingBufferMemory);

    _device.createBuffer(bufferSize,
                         VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _vertexBuffer, _vertexBufferMemory);

    _device.copyBuffer(stagingBuffer, _vertexBuffer, bufferSize);

    vkDestroyBuffer(_device.device(), stagingBuffer, nullptr);
    vkFreeMemory(_device.device(), stagingBufferMemory, nullptr);
}

void Model::createIndexBuffer(const std::vector<uint32_t>& indices)
{
    _indexCount = static_cast<uint32_t>(indices.size());
    _hasIndexBuffer = _indexCount > 0;

    if (!_hasIndexBuffer)
    {
        return;
    }

    VkDeviceSize bufferSize = sizeof(indices[0]) * _indexCount;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    _device.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                         stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(_device.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), static_cast<size_t>(bufferSize));
    vkUnmapMemory(_device.device(), stagingBufferMemory);

    _device.createBuffer(bufferSize,
                         VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _indexBuffer, _indexBufferMemory);

    _device.copyBuffer(stagingBuffer, _indexBuffer, bufferSize);

    vkDestroyBuffer(_device.device(), stagingBuffer, nullptr);
    vkFreeMemory(_device.device(), stagingBufferMemory, nullptr);
}

void Model::bind(VkCommandBuffer commandBuffer)
{
    VkBuffer buffers[] = {_vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

    if (_hasIndexBuffer)
    {
        vkCmdBindIndexBuffer(commandBuffer, _indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    }
}

void Model::draw(VkCommandBuffer commandBuffer)
{
    if (_hasIndexBuffer)
    {
        vkCmdDrawIndexed(commandBuffer, _indexCount, 1, 0, 0, 0);
    }
    else
    {
        vkCmdDraw(commandBuffer, _vertexCount, 1, 0, 0);
    }
}

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
        {2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal)},
        {3, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, texCoord)},
    };
}

std::unique_ptr<Model> Model::createModelFromFile(Device& device, const std::string& filePath)
{
    Model::Data modelData{};
    modelData.loadModel(filePath);
    std::cout << "Model has " << modelData.vertices.size() << " vertices and "
              << modelData.indices.size() << " indices" << std::endl;
    return std::make_unique<Model>(device, modelData);
}

void Model::Data::loadModel(const std::string& filePath)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filePath.c_str()))
    {
        throw std::runtime_error(warn + err);
    }

    vertices.clear();
    indices.clear();

    std::unordered_map<Vertex, uint32_t> uniqueVertices{};
    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            Vertex vertex{};

            if (index.vertex_index >= 0)
            {
                vertex.position = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2],
                };

                vertex.color = {
                    attrib.colors[3 * index.vertex_index + 0],
                    attrib.colors[3 * index.vertex_index + 1],
                    attrib.colors[3 * index.vertex_index + 2],
                };
            }

            if (index.normal_index >= 0)
            {
                vertex.normal = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2],
                };
            }

            if (index.texcoord_index >= 0)
            {
                vertex.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1],
                };
            }

            if (uniqueVertices.count(vertex) == 0)
            {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }
            indices.push_back(uniqueVertices[vertex]);
        }
    }
}