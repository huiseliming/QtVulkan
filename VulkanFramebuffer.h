#pragma once
#include "VulkanTools.h"

struct VulkanFramebuffer : public VulkanBase
{
    VulkanFramebuffer(VulkanDevice& device)
        :VulkanBase(device)
    {}
    ~VulkanFramebuffer();
    operator VkFramebuffer() const { return Framebuffer; }

    void Create();

    void Destroy();

    VkFramebuffer Framebuffer{VK_NULL_HANDLE};
};


