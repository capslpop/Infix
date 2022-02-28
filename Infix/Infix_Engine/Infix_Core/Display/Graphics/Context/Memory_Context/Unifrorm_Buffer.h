#pragma once

class UniformBuffer
{
public:
	UniformBuffer(VkDevice in_device, VkPhysicalDevice in_physicalDevice);
	~UniformBuffer();

	void createUniformBuffers();
	void cleanupUniformBuffers();

	void updataUniformBuffer(uint32_t imageIndex);

private:
	void createDescriptorSetLayout();


	VkDescriptorSetLayout descriptorSetLayout;

	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	// these need to be vectors because we need as many instances as there are frames in flight
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;

	VkDevice device;

	VkPhysicalDevice physicalDevice;

	struct UniformBufferObject {
		glm::vec3 camDir;
		glm::vec3 camPos;
	};
};

UniformBuffer::UniformBuffer(VkDevice in_device, VkPhysicalDevice in_physicalDevice)
{
	device = in_device;
	physicalDevice = in_physicalDevice;

	createDescriptorSetLayout();
	createUniformBuffers();
}

UniformBuffer::~UniformBuffer()
{
	cleanupUniformBuffers();

	vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
}

void UniformBuffer::createUniformBuffers()
{
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
	uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		Buffer::createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			uniformBuffers[i], uniformBuffersMemory[i], device, physicalDevice);
	}
}

void UniformBuffer::cleanupUniformBuffers()
{
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroyBuffer(device, uniformBuffers[i], nullptr);
		vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
	}
}

void UniformBuffer::updataUniformBuffer(uint32_t imageIndex)
{
	UniformBufferObject ubo{};

	//ubo.camPos = 
}


void UniformBuffer::createDescriptorSetLayout()
{
	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &uboLayoutBinding;

	if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create descriptor set layout");
	}

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
}