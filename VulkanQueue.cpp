#include "VulkanQueue.h"
#include <memory>
#include <cassert>

VulkanQueue::VulkanQueue(VkQueue queue)
{
    Queue = queue;
}

VulkanQueue::VulkanQueue(const VulkanQueue & other)
{
    this->Queue = other.Queue;
}

VulkanQueue::VulkanQueue(VulkanQueue && other)
{
    assert(std::addressof(other) != this);
    this->Queue = other.Queue;
    other.Queue = VK_NULL_HANDLE;
}

VulkanQueue &VulkanQueue::operator=(const VulkanQueue & other)
{
    this->Queue = other.Queue;
    return *this;
}

VulkanQueue &VulkanQueue::operator=(VulkanQueue&& other)
{
    assert(std::addressof(other) != this);
    this->Queue = other.Queue;
    other.Queue = VK_NULL_HANDLE;
    return *this;
}
