#include "VulkanCommandPool.h"
#include "VulkanDevice.h"
VulkanCommandPool::VulkanCommandPool(VulkanDevice &device)
    :Device(device)
{

}

VulkanCommandPool::~VulkanCommandPool()
{
    Destroy();
}

void VulkanCommandPool::Create(VkCommandPoolCreateFlags flags, uint32_t queueFamilyIndex)
{
    VkCommandPoolCreateInfo CommandPoolCI{
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext = nullptr,
        .flags = flags,
        .queueFamilyIndex = (queueFamilyIndex != UINT32_MAX ? queueFamilyIndex : Device.QueueFamilyIndices.graphics),
    };
    VK_ASSERT_SUCCESSED(vkCreateCommandPool(Device, &CommandPoolCI, nullptr, &CommandPool));
}

void VulkanCommandPool::Destroy()
{
    if(CommandPool != VK_NULL_HANDLE){
        vkDestroyCommandPool(Device, CommandPool, nullptr);
        CommandPool = VK_NULL_HANDLE;
    }
}
