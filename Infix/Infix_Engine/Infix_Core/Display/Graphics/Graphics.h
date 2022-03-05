#pragma once

// find a place for this
static struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

        if (presentSupport) {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) {
            break;
        }

        i++;
    }

    return indices;
}

const int MAX_FRAMES_IN_FLIGHT = 2; // number of frames that should be processed concurrently

#include "Debug/Debug.h"
#include "Window/Main_Window/Main_Window.h"
#include "Context/Device_Context.h"
#include "Context/Application_Context.h"
#include "GraphicsCode/Shaders.h"

#include "Context/Swap_Chain.h"
#include "Context/Command_Pool.h"

#include "Context/Memory_Context/Buffer.h"
#include "Context/Memory_Context/Circular_Buffer.h"
#include "Context/Memory_Context/Texture_Context.h"
#include "Context/Memory_Context/Unifrorm_Buffer.h"

#include "Context/Command_Buffer.h"
#include "Context/Sync_Object.h"


//************************************************
// Graphics - Debuging And Graphics Creation
//************************************************

class Graphics
{
public:
	Graphics();
	~Graphics();

    void drawFrame();
    void recreateFrame();

    MainWindow mainWindow;
    ApplicationContext applicationContext;
    Debug debug;
    DeviceContext deviceContext;
    //DescriptorSetLayout descriptorSetLayout;
    UniformBuffer uniformBuffer;

    SwapChain swapChain;
    CommandPool commandPool;

    CircularBuffer circularBuffer;
    TextureContext textureContext;
    //
    CommandBuffer commandBuffer;
    SyncObject syncObject;

private:
};

Graphics::Graphics() :
    mainWindow(),
    applicationContext(),
    debug(applicationContext.instance),
    deviceContext(applicationContext.instance, mainWindow.window),

    uniformBuffer(deviceContext.device, deviceContext.physicalDevice),

    swapChain(&deviceContext, mainWindow.window, uniformBuffer.descriptorSetLayout),
    commandPool(&deviceContext),

    circularBuffer(),
    textureContext(deviceContext.physicalDevice, deviceContext.device, &commandPool),

    commandBuffer(&swapChain, &deviceContext, &commandPool, &uniformBuffer),
    syncObject(deviceContext.device, swapChain.swapChainImages)
{
}

Graphics::~Graphics()
{
}

void Graphics::drawFrame()
{
    vkWaitForFences(deviceContext.device, 1, &syncObject.inFlightFences[syncObject.currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    
    VkResult result = vkAcquireNextImageKHR(deviceContext.device, swapChain.swapChain, UINT64_MAX,
        syncObject.imageAvailableSemaphores[syncObject.currentFrame], VK_NULL_HANDLE, &imageIndex);
    
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateFrame();
        return;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    uniformBuffer.updataUniformBuffer(syncObject.currentFrame);

    vkResetFences(deviceContext.device, 1, &syncObject.inFlightFences[syncObject.currentFrame]);

    vkResetCommandBuffer(commandBuffer.commandBuffers[syncObject.currentFrame], /*VkCommandBufferResetFlagBits*/ 0);
    commandBuffer.recordCommandBuffer(commandBuffer.commandBuffers[syncObject.currentFrame], imageIndex, syncObject.currentFrame);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { syncObject.imageAvailableSemaphores[syncObject.currentFrame] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer.commandBuffers[syncObject.currentFrame];

    VkSemaphore signalSemaphores[] = { syncObject.renderFinishedSemaphores[syncObject.currentFrame] };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(deviceContext.graphicsQueue, 1, &submitInfo, syncObject.inFlightFences[syncObject.currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = { swapChain.swapChain };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(deviceContext.presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || mainWindow.frameBufferResized) {
        mainWindow.frameBufferResized = false;
        recreateFrame();
    }
    else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    syncObject.currentFrame = (syncObject.currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Graphics::recreateFrame()
{
    swapChain.recreateSwapChain();
    //uniformBuffer.cleanupUniformBuffers();
    //uniformBuffer.createUniformBuffers();
    //uniformBuffer.createDescriptorPool();
    //uniformBuffer.createDescriptorSet();
}

void MainWindow::frameBufferResizedCallback(GLFWwindow* window, int width, int height)
{
    auto graphics = reinterpret_cast<Graphics*>(glfwGetWindowUserPointer(window));
    graphics->mainWindow.frameBufferResized = true;

    graphics->drawFrame();
}