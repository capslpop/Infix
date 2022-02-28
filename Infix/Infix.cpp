#include "Infix_Engine/Infix_Engine.h"

int main() {
    InfixEngine infixEngine;

    try {
        while (!glfwWindowShouldClose(infixEngine.display.graphics.mainWindow.window))
        {
            infixEngine.run();
        }
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}