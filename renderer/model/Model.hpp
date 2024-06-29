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
#include <vector>

namespace Caladan::Renderer
{

class Model
{
   public:
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 color;

        static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
    };

    Model(Device& device, const std::vector<Vertex>& vertices);
    ~Model();

    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;

    void bind(VkCommandBuffer commandBuffer);
    void draw(VkCommandBuffer commandBuffer);

    // Getters

    // Setters

   private:
    void createVertexBuffer(const std::vector<Vertex>& vertices);

    Device& _device;
    VkBuffer _vertexBuffer;
    VkDeviceMemory _vertexBufferMemory;
    uint32_t _vertexCount;
    // Member variables
};

}  // namespace Caladan::Renderer

#endif  // MODEL_H