#pragma once
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <string>
#include <vector>


struct VulkanInstance
{
    VulkanInstance() = default;
    VulkanInstance(std::vector<const char*> enabledInstanceLayers, std::vector<const char*> enabledInstanceExtensions);
    VulkanInstance(const VulkanInstance&) = delete;
    VulkanInstance(VulkanInstance&&) = delete;

    VulkanInstance& operator=(const VulkanInstance&) = delete;
    VulkanInstance& operator=(VulkanInstance&&) = delete;

    ~VulkanInstance();

    operator VkInstance() const { return _Instance; }
    operator bool() const { return this->IsValid(); }

    bool IsValid() const { return _Instance != VK_NULL_HANDLE;}
    bool IsEnableLayer(const char * LayerName);
    bool IsEnableExtension(const char * ExtensionName);
    bool IsEnableValidationLayer();
    bool IsEnableDebugReportExtension();

private:
    void CreateInstance(std::vector<const char*>& enabledInstanceLayers, std::vector<const char*>& enabledInstanceExtensions);
    void DestroyInstance();

    void CreateDebugReporter();
    void DestroyDebugReporter();

    VkDebugReportCallbackEXT _fpDebugReportCallbackEXT{VK_NULL_HANDLE};

    //bool _EnableValidationLayer{true};
    std::vector<const char*> _EnabledInstanceLayers;
    std::vector<const char*> _EnabledInstanceExtenisons;
    VkInstance _Instance{VK_NULL_HANDLE};
public:
    static std::vector<VkLayerProperties> EnumerateInstanceLayerProperties();
    static std::vector<VkExtensionProperties> EnumerateInstanceExtensionProperties(const char* pLayerName = nullptr);
    static std::vector<const char*> GetRequiredInstanceExtensions();
};



