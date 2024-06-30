/**********************************************************************************
 * Project: Caladan
 * Description: Multi-platform 2D and 3D game engine
 * Author: Alexis BOITEL
 * Date, Location: 2024, Rennes
 **********************************************************************************/

#ifndef GRAPHICSPIPELINE_H
#define GRAPHICSPIPELINE_H

#include "Device.hpp"

// std
#include <string>
#include <vector>

namespace Caladan::Render
{
struct PipelineConfigInfo
{
    PipelineConfigInfo(const PipelineConfigInfo&) = delete;
    PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    VkPipelineViewportStateCreateInfo viewportInfo;
    std::vector<VkDynamicState> dynamicStateEnables;
    VkPipelineDynamicStateCreateInfo dynamicStateInfo;
    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;
};
class GraphicsPipeline
{
   public:
    GraphicsPipeline(Device& device, const std::string& vertexShaderPath,
                     const std::string& fragmentShaderPath, const PipelineConfigInfo& configInfo);
    ~GraphicsPipeline();

    GraphicsPipeline(const GraphicsPipeline&) = delete;
    GraphicsPipeline& operator=(const GraphicsPipeline&) = delete;

    static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);
    void bind(VkCommandBuffer commandBuffer);

   private:
    static std::vector<char> ReadFile(const std::string& filePath);

    void createGraphicsPipeline(const std::string& vertexShaderPath,
                                const std::string& fragmentShaderPath,
                                const PipelineConfigInfo& configInfo);

    void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

    // Member variables
    Device& _device;
    VkPipeline _graphicsPipeline;
    VkShaderModule _vertexShaderModule;
    VkShaderModule _fragmentShaderModule;
};
}  // namespace Caladan::Render

#endif  // GRAPHICSPIPELINE_H