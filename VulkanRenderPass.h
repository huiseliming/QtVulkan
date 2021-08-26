#pragma once
#include "VulkanTools.h"
#include <vulkan/vulkan_core.h>


struct VulkanRenderPass : public VulkanBase
{
    VulkanRenderPass(VulkanDevice& device)
        : VulkanBase(device)
        {}
    ~VulkanRenderPass();
    operator VkRenderPass() const { return RenderPass; }

    void Create(VkFormat SwapchainImageFormat);

    void Destroy();

    VkRenderPass RenderPass{VK_NULL_HANDLE};
};




