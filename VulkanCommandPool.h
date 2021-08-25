#pragma once
#include <VulkanTools.h>

struct VulkanDevice;

struct VulkanCommandPool
{
    VulkanCommandPool() = delete;
    VulkanCommandPool(VulkanDevice& device);
    VulkanCommandPool(const VulkanCommandPool&) = delete;
    VulkanCommandPool(VulkanCommandPool&&) = delete;
    VulkanCommandPool& operator=(const VulkanCommandPool&) = delete;
    VulkanCommandPool& operator=(VulkanCommandPool&&) = delete;
    ~VulkanCommandPool();

    operator VkCommandPool() const { return CommandPool; }


    void Create(VkCommandPoolCreateFlags flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, uint32_t queueFamilyIndex = UINT32_MAX);
    void Destroy();


    VkCommandPool CommandPool;
    VulkanDevice& Device;
};
