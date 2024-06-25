/*
** EPITECH PROJECT, 2024
** Caladan
** File description:
** Window
*/

#ifndef WINDOW_H_
#define WINDOW_H_

#include <GLFW/glfw3.h>

namespace Caladan::Renderer {
    class Window {
        public:
            Window();
            ~Window();
            void CreateWindow();
            void DestroyWindow();
            GLFWwindow *GetWindow() const { return _window; }

        protected:
        private:
            GLFWwindow *_window;
    };
}

#endif /* !WINDOW_H_ */
