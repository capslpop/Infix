#pragma once

//************************************************
// Command Pool - 
//************************************************

class CommandPool
{
public:
	CommandPool(DeviceContext* deviceContext);
	~CommandPool();

	VkCommandPool commandPool;

    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

private:
	DeviceContext *deviceContext;
};

CommandPool::CommandPool(DeviceContext *in_deviceContext)
{
	deviceContext = in_deviceContext;
	QueueFamilyIndices queueFamilyIndices = findQueueFamilies(deviceContext->physicalDevice, deviceContext->surface);
	
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
	// look into setting flags VK_COMMAND_POOL_CREATE_TRANSIENT_BIT vs.VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
	
	if (vkCreateCommandPool(deviceContext->device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
	    throw std::runtime_error("failed to create command pool!");
	}
}

CommandPool::~CommandPool()
{
	vkDestroyCommandPool(deviceContext->device, commandPool, nullptr);
}

VkCommandBuffer CommandPool::beginSingleTimeCommands() {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(deviceContext->device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void CommandPool::endSingleTimeCommands(VkCommandBuffer commandBuf) {
    vkEndCommandBuffer(commandBuf);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuf;

    vkQueueSubmit(deviceContext->graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(deviceContext->graphicsQueue);

    vkFreeCommandBuffers(deviceContext->device, commandPool, 1, &commandBuf);
}