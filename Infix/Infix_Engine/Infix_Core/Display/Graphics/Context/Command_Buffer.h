#pragma once

//************************************************
// Command Buffer - 
//************************************************

class CommandBuffer
{
public:
	CommandBuffer(SwapChain* in_swapChain, DeviceContext* in_deviceContext,
        CommandPool* in_commandPool, UniformBuffer* in_uniformBuffer);
	~CommandBuffer();

    std::vector<VkCommandBuffer> commandBuffers;

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, size_t currentFrame);
private:
    void createCommandBuffers();

    SwapChain *swapChain;
    DeviceContext *deviceContext;
    CommandPool* commandPool;
    UniformBuffer *uniformBuffer;
};

CommandBuffer::CommandBuffer(SwapChain *in_swapChain, DeviceContext *in_deviceContext,
    CommandPool *in_commandPool, UniformBuffer *in_uniformBuffer)
{
    swapChain = in_swapChain;
    deviceContext = in_deviceContext;
    commandPool = in_commandPool;
    uniformBuffer = in_uniformBuffer;

    createCommandBuffers();
}

CommandBuffer::~CommandBuffer()
{
    
}

void CommandBuffer::createCommandBuffers() {
    commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool->commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

    if (vkAllocateCommandBuffers(deviceContext->device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void CommandBuffer::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, size_t currentFrame) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = swapChain->renderPass;
    renderPassInfo.framebuffer = swapChain->swapChainFramebuffers[imageIndex];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = swapChain->swapChainExtent;

    VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, swapChain->graphicsPipeline);

        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, swapChain->pipelineLayout, 0, 1, &uniformBuffer->descriptorSets[currentFrame], 0, nullptr);

        vkCmdDraw(commandBuffer, 6, 1, 0, 0);

    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}
