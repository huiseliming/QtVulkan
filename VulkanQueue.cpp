#include "VulkanQueue.h"
#include <memory>
#include <cassert>

VulkanQueue::VulkanQueue(VkQueue queue)
{
    _Queue = queue;
}

VulkanQueue::VulkanQueue(const VulkanQueue & other)
{
    this->_Queue = other._Queue;
}

VulkanQueue::VulkanQueue(VulkanQueue && other)
{
    assert(std::addressof(other) != this);
    this->_Queue = other._Queue;
    other._Queue = VK_NULL_HANDLE;
}

VulkanQueue &VulkanQueue::operator=(const VulkanQueue & other)
{
    this->_Queue = other._Queue;
    return *this;
}

VulkanQueue &VulkanQueue::operator=(VulkanQueue&& other)
{
    assert(std::addressof(other) != this);
    this->_Queue = other._Queue;
    other._Queue = VK_NULL_HANDLE;
    return *this;
}
