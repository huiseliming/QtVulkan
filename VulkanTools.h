#pragma once
#include <vector>
#include <VulkanException.h>
 
#define VK_ASSERT_SUCCESSED(expression)                     \
    {                                                       \
        VkResult result = (expression);                     \
        if (result != VK_SUCCESS) VK_THROW_EXCEPT(result);  \
    }

struct VulkanDevice;

struct VulkanBase
{
    VulkanBase() = delete;
    VulkanBase(VulkanDevice& device)
        : Device(device)
    {}
    VulkanBase(const VulkanBase&) = delete;
    VulkanBase(VulkanBase&& other) : Device(other.Device) {}
    VulkanBase& operator=(const VulkanBase&) = delete;
    VulkanBase& operator=(VulkanBase&&) = delete;
    virtual ~VulkanBase() {}
    VulkanDevice& Device;
};

namespace VulkanTools
{
const std::string& GetShadersPath();
std::vector<uint8_t> ReadBytes(const std::string& filePath);

const char *  ToString(VkPhysicalDeviceType physicalDeviceTyp);
const char *  ToString(VkResult result);
uint32_t GetQueueFamilyIndex(const std::vector<VkQueueFamilyProperties> &queueFamilyProperties,
                             const VkQueueFlagBits queueFlags);

}
