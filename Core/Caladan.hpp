/**********************************************************************************
 * Project: Caladan
 * Description: Multi-platform 2D and 3D game engine
 * Author: Alexis BOITEL
 * Date, Location: 2024, Rennes
 **********************************************************************************/

#ifndef CALADAN_HPP_
#define CALADAN_HPP_

#include <Renderer.hpp>

// std

namespace Caladan
{
class Caladan
{
   public:
    Caladan();
    ~Caladan();

   protected:
   private:
    Renderer::Renderer _renderer;
};

}  // namespace Caladan

#endif /* !CALADAN_HPP_ */
