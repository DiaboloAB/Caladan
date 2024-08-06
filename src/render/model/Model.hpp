/**********************************************************************************
 * Project: Caladan
 * Description: Multi-platform 2D and 3D game engine
 * Author: Alexis BOITEL
 * Date, Location: 2024, Rennes
 **********************************************************************************/

#ifndef MODEL_H
#define MODEL_H

#include <Device.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <memory>
#include <vector>

namespace Caladan::Render
{

class Model
{
   public:
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 color;
        glm::vec3 normal;
        glm::vec2 texCoord;  // uv texture coordinates

        static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

        bool operator==(const Vertex& other) const
        {
            return position == other.position && color == other.color && normal == other.normal &&
                   texCoord == other.texCoord;
        }
    };

    struct Data
    {
        std::vector<Vertex> vertices{};
        std::vector<u_int32_t> indices{};

        void loadModel(const std::string& filePath);
    };

    Model(Device& device, const Model::Data& Data);
    ~Model();

    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;

    void bind(VkCommandBuffer commandBuffer);
    void draw(VkCommandBuffer commandBuffer);

    static std::unique_ptr<Model> createModelFromFile(Device& device, const std::string& filePath);

   private:
    void createVertexBuffer(const std::vector<Vertex>& vertices);
    void createIndexBuffer(const std::vector<u_int32_t>& indices);

    Device& _device;

    VkBuffer _vertexBuffer;
    VkDeviceMemory _vertexBufferMemory;
    uint32_t _vertexCount;

    bool _hasIndexBuffer = false;
    VkBuffer _indexBuffer;
    VkDeviceMemory _indexBufferMemory;
    uint32_t _indexCount;
};

}  // namespace Caladan::Render

#endif  // MODEL_H