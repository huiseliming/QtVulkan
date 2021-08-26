#pragma once
#include "VulkanTools.h"


struct VulkanDevice;

struct VulkanImageResource : public VulkanBase
{
    VulkanImageResource(VulkanDevice& device);
    ~VulkanImageResource();

    void CreateImage(uint32_t width,
                    uint32_t height,
                    VkFormat format,
                    VkImageTiling tiling,
                    VkImageUsageFlags usage,
                    VkMemoryPropertyFlags memoryPropertyFlags,
                    uint32_t mipLevels = 1);
    void CreateImageView(VkFormat format, VkImageAspectFlags aspectFlags);
    void CreateDepthResource(VkExtent2D swapchainExtent);

    void Destroy();

    VkDeviceMemory ImageMemory{VK_NULL_HANDLE};
    VkImage Image{VK_NULL_HANDLE};
    VkImageView ImageView{VK_NULL_HANDLE};
};


