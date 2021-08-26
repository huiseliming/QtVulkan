#include "VulkanImageResource.h"
#include "VulkanDevice.h"
#include <vulkan/vulkan_core.h>


VulkanImageResource::VulkanImageResource(VulkanDevice& device) 
    :VulkanBase(device)
{
    
}

VulkanImageResource::~VulkanImageResource() 
{
    Destroy();    
}


void VulkanImageResource::CreateImage(
    uint32_t width,
    uint32_t height,
    VkFormat format,
    VkImageTiling tiling,
    VkImageUsageFlags usage,
    VkMemoryPropertyFlags memoryPropertyFlags,
    uint32_t mipLevels) 
{
    assert(ImageMemory = VK_NULL_HANDLE);
    assert(Image = VK_NULL_HANDLE);
    // create image
    VkImageCreateInfo imageCI{
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = format,
        .extent =
            {
                .width = width,
                .height = height,
                .depth = 1,
            },
        .mipLevels = 1,
        .arrayLayers = 1,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = tiling,
        .usage = usage,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = nullptr,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    };

    VK_ASSERT_SUCCESSED(vkCreateImage(Device, &imageCI, nullptr, &Image))

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(Device, Image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = Device.FindMemoryType(memRequirements.memoryTypeBits, memoryPropertyFlags);
    VK_ASSERT_SUCCESSED(vkAllocateMemory(Device, &allocInfo, nullptr, &ImageMemory))
    VK_ASSERT_SUCCESSED(vkBindImageMemory(Device, Image, ImageMemory, 0));
}

void VulkanImageResource::CreateImageView(VkFormat format, VkImageAspectFlags aspectFlags) 
{
    assert(ImageView = VK_NULL_HANDLE);
    VkImageViewCreateInfo imageViewCI{.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                                      .image = Image,
                                      .viewType = VK_IMAGE_VIEW_TYPE_2D,
                                      .format = format,
                                      .components = {},
                                      .subresourceRange{
                                          .aspectMask = aspectFlags,
                                          .baseMipLevel = 0,
                                          .levelCount = 1,
                                          .baseArrayLayer = 0,
                                          .layerCount = 1,
                                      }};
    VK_ASSERT_SUCCESSED(vkCreateImageView(Device, &imageViewCI, nullptr, &ImageView));
}

void VulkanImageResource::CreateDepthResource(VkExtent2D swapchainExtent)
{
    VkFormat depthFormat = Device.FindDepthFormat();
    CreateImage(swapchainExtent.width,
                swapchainExtent.height,
                depthFormat,
                VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    CreateImageView(depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
}


void VulkanImageResource::Destroy() 
{
    if (Image != VK_NULL_HANDLE) {
        vkDestroyImage(Device, Image, nullptr);
        ImageView = VK_NULL_HANDLE;

    }
    if (ImageView != VK_NULL_HANDLE) {
        vkDestroyImageView(Device, ImageView, nullptr);
        ImageView = VK_NULL_HANDLE;
    }
    if (ImageMemory != VK_NULL_HANDLE){
        vkFreeMemory(Device, ImageMemory, nullptr);
        ImageMemory = VK_NULL_HANDLE;
    }
}

