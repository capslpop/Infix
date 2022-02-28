#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <optional>
#include <set>
#include <fstream>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

// image loading
#define STB_IMAGE_IMPLEMENTATION
#include "Includes/stb_image.h"

#define NDEBUG

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

#include "Infix_Core/Display/Display.h"

class InfixEngine
{
public:
    InfixEngine();
    ~InfixEngine();

    void run();

    Display display;

private:
};

InfixEngine::InfixEngine()
{
    
}

InfixEngine::~InfixEngine()
{

}

void InfixEngine::run()
{
    glfwPollEvents();
    display.draw();

    vkDeviceWaitIdle(display.graphics.deviceContext.device);
}