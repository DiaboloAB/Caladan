/**********************************************************************************
 * Project: Caladan
 * Description: Multi-platform 2D and 3D game engine
 * Author: Alexis BOITEL
 * Date, Location: 2024, Rennes
 **********************************************************************************/

#ifndef GRAPHICSPIPELINE_H
#define GRAPHICSPIPELINE_H

// std
#include <string>
#include <vector>

namespace Caladan::Renderer
{
class GraphicsPipeline
{
   public:
    GraphicsPipeline(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    ~GraphicsPipeline();

   private:
    // Member variables
    static std::vector<char> ReadFile(const std::string& filePath);

    void createGraphicsPipeline(const std::string& vertexShaderPath,
                                const std::string& fragmentShaderPath);
};
}  // namespace Caladan::Renderer

#endif  // GRAPHICSPIPELINE_H