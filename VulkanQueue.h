#ifndef VULKANQUEUE_H
#define VULKANQUEUE_H

#include <vulkan/vulkan.h>

struct VulkanQueue
{
    VulkanQueue() = default;
    VulkanQueue(VkQueue queue);

    VulkanQueue(const VulkanQueue&);
    VulkanQueue(VulkanQueue&&);

    VulkanQueue& operator=(const VulkanQueue&);
    VulkanQueue& operator=(VulkanQueue&&);

    operator VkQueue() const { return Queue; }
    VkQueue* AddressOf() { return &Queue; }

    VkQueue Queue{VK_NULL_HANDLE};
};

#endif // VULKANQUEUE_H
