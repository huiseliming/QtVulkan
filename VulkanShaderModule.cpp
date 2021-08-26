#include "VulkanShaderModule.h"
#include "VulkanDevice.h"
#include <memory>

VulkanShaderModule::VulkanShaderModule(VulkanDevice& device, const std::string& shaderPath)
    : VulkanBase(device)
{
    Create(shaderPath);
}
VulkanShaderModule::VulkanShaderModule(VulkanDevice& device, const std::vector<uint8_t>& shaderCode)
    : VulkanBase(device)
{
    Create(shaderCode);
}

VulkanShaderModule::VulkanShaderModule(VulkanShaderModule&& other)
    : VulkanBase(other.Device)
{
    assert(std::addressof(other) != this);
    this->ShaderModule = other.ShaderModule;
    other.ShaderModule = VK_NULL_HANDLE;
}

VulkanShaderModule::~VulkanShaderModule()
{
    Destroy();
}

void VulkanShaderModule::Create(const std::string& shaderPath)
{
    return Create(VulkanTools::ReadBytes(shaderPath));
}

void VulkanShaderModule::Create(const std::vector<uint8_t> &shaderCode)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = shaderCode.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>(shaderCode.data());
    VkShaderModule shaderModule;
    VK_ASSERT_SUCCESSED(vkCreateShaderModule(Device, &createInfo, nullptr, &shaderModule));
}

void VulkanShaderModule::Destroy() 
{
    if (ShaderModule != VK_NULL_HANDLE) {
        vkDestroyShaderModule(Device, ShaderModule, nullptr);
        ShaderModule = VK_NULL_HANDLE;
    }
}
