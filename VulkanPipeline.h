#pragma once 
#include "VulkanTools.h"

struct VulkanShaderModule;



struct VulkanPipeline : public VulkanBase
{
    VulkanPipeline(VulkanDevice& device)
        :VulkanBase(device)
    {}
    ~VulkanPipeline();
    

    void Create();
    void Destroy();
    
    VkPipelineLayout PipelineLayout{VK_NULL_HANDLE};
    VkPipeline Pipeline{VK_NULL_HANDLE};
};


