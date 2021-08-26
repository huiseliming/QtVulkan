#include "VulkanFramebuffer.h"
#include "VulkanTools.h"
#include "VulkanRenderPass.h"
#include "VulkanDevice.h"
#include <array>

VulkanFramebuffer::~VulkanFramebuffer() 
{
    Destroy();
}

void VulkanFramebuffer::Create()
{
    VkImageView swapchainImageViews;
    VkImageView depthImageView;    
    VkExtent2D swapchainExtent;
    VulkanRenderPass renderPass(Device);
    std::array<VkImageView, 2> attachments = { swapchainImageViews, depthImageView };
    VkFramebufferCreateInfo framebufferCI{
        .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .renderPass = renderPass,
        .attachmentCount = static_cast<uint32_t>(attachments.size()),
        .pAttachments = attachments.data(),
        .width = swapchainExtent.width,
        .height = swapchainExtent.height,
        .layers = 1,
    };
    VK_ASSERT_SUCCESSED(vkCreateFramebuffer(Device, &framebufferCI, nullptr, &Framebuffer));
}

void VulkanFramebuffer::Destroy() 
{
    if (Framebuffer != VK_NULL_HANDLE) {
        vkDestroyFramebuffer(Device, Framebuffer, nullptr);
        Framebuffer = VK_NULL_HANDLE;
    }
}
