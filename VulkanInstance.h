#pragma once
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <string>
#include <vector>


struct VulkanInstance
{
    VulkanInstance();
    VulkanInstance(bool EnableValidationLayer);
    VulkanInstance(const VulkanInstance&) = delete;
    VulkanInstance(VulkanInstance&&);

    VulkanInstance& operator=(const VulkanInstance&) = delete;
    VulkanInstance& operator=(VulkanInstance&&);

    ~VulkanInstance();

    operator VkInstance() const { return _Instance; }
    operator bool() const { return this->IsValid(); }

    bool IsValid() const { return _Instance != VK_NULL_HANDLE;}

    void CreateInstance(bool EnableValidationLayer = true);
    void DestroyInstance();

    void CreateDebugReporter();
    void DestroyDebugReporter();

    VkDebugReportCallbackEXT _fpDebugReportCallbackEXT{VK_NULL_HANDLE};

    bool _EnableValidationLayer{true};
    std::vector<const char*> _EnabledInstanceLayers;
    std::vector<const char*> _EnabledInstanceExtenisons;
    VkInstance _Instance{VK_NULL_HANDLE};

    static std::vector<VkLayerProperties> EnumerateInstanceLayerProperties();
    static std::vector<VkExtensionProperties> EnumerateInstanceExtensionProperties(const char* pLayerName = nullptr);
    static std::vector<const char*> GetRequiredInstanceExtensions();
};



