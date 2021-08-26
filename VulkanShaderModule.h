#pragma once
#include "VulkanTools.h"
#include <vector>


struct VulkanShaderModule : public VulkanBase
{
    VulkanShaderModule(VulkanDevice& device)
        : VulkanBase(device)
    {}

    VulkanShaderModule(VulkanDevice& device, const std::string& shaderPath);

    VulkanShaderModule(VulkanDevice& device, const std::vector<uint8_t>& shaderCode);
    
    VulkanShaderModule(VulkanShaderModule&&);

    ~VulkanShaderModule();

    operator VkShaderModule() const { return ShaderModule; }

    void Create(const std::string& shaderPath);
    
    void Create(const std::vector<uint8_t>& shaderCode);

    void Destroy();
    
    VkShaderModule ShaderModule;
};


