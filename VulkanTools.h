#pragma once
#include <vulkan/vulkan.h>
#include <VulkanException.h>
 
#define VK_ASSERT_SUCCESSED(expression)                     \
    {                                                       \
        VkResult result = (expression);                     \
        if (result != VK_SUCCESS) VK_THROW_EXCEPT(result);  \
    }
const char *  ToString(VkPhysicalDeviceType physicalDeviceTyp);
const char *  ToString(VkResult result);


