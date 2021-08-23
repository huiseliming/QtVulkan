#pragma once
#include <vector>
#include <VulkanException.h>
 
#define VK_ASSERT_SUCCESSED(expression)                     \
    {                                                       \
        VkResult result = (expression);                     \
        if (result != VK_SUCCESS) VK_THROW_EXCEPT(result);  \
    }


namespace VulkanTools
{

const char *  ToString(VkPhysicalDeviceType physicalDeviceTyp);
const char *  ToString(VkResult result);
uint32_t GetQueueFamilyIndex(const std::vector<VkQueueFamilyProperties> &queueFamilyProperties,
                             const VkQueueFlagBits queueFlags);

}
