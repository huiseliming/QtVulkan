#include "VulkanTools.h"
#include <VulkanInstance.h>
#include <cstdio>
#include <memory>
#include <vulkan/vulkan_core.h>
#include <QDebug>

#define MAKE_APPLICATION_VERSION(Major, Minor, Patch)  (\
static_cast<uint32_t>(Major && 0xF    ) << 28 ||        \
static_cast<uint32_t>(Minor && 0xFF   ) << 20 ||        \
static_cast<uint32_t>(Patch && 0xFFFFF))

#define APPLICATION_NAME "QtApplication"
#define VK_API_VERSION VK_MAKE_VERSION(1, 0, 0)
static VkBool32 DebugReportCallbackEXT( VkDebugReportFlagsEXT flags,
                                        VkDebugReportObjectTypeEXT objectType,
                                        uint64_t object,
                                        size_t location,
                                        int32_t messageCode,
                                        const char *pLayerPrefix,
                                        const char *pMessage,
                                        void *pUserData);


VulkanInstance::VulkanInstance(std::vector<const char*> enabledInstanceLayers, std::vector<const char*> enabledInstanceExtensions)
{
    Create(enabledInstanceLayers, enabledInstanceExtensions);
}

//VulkanInstance::VulkanInstance(VulkanInstance&& otherInstance)
//{
//    if(std::addressof(otherInstance) != this)
//    {
//        this->_Instance = otherInstance._Instance;
//        otherInstance._Instance = nullptr;
//        this->_EnabledInstanceLayers = std::move(otherInstance._EnabledInstanceLayers);
//        this->_EnabledInstanceExtenisons = std::move(otherInstance._EnabledInstanceExtenisons);
//    }
//}

//VulkanInstance& VulkanInstance::operator=(VulkanInstance&& otherInstance)
//{
//    if(std::addressof(otherInstance) != this)
//    {
//        this->_Instance = otherInstance._Instance;
//        otherInstance._Instance = nullptr;
//        this->_EnabledInstanceLayers = std::move(otherInstance._EnabledInstanceLayers);
//        this->_EnabledInstanceExtenisons = std::move(otherInstance._EnabledInstanceExtenisons);
//    }
//    return *this;
//}

VulkanInstance::~VulkanInstance() 
{
    Destroy();
}

bool VulkanInstance::IsEnableLayer(const char *LayerName)
{
    for (uint32_t i = 0; i < EnabledInstanceLayers.size(); i++)
    {
        if (strcmp(EnabledInstanceLayers[i], LayerName) == 0) {
            return true;
        }
    }
    return false;
}

bool VulkanInstance::IsEnableExtension(const char *ExtensionName)
{
    for (uint32_t i = 0; i < EnabledInstanceExtenisons.size(); i++)
    {
        if (strcmp(EnabledInstanceExtenisons[i], ExtensionName) == 0) {
            return true;
        }
    }
    return false;
}

bool VulkanInstance::IsEnableValidationLayer() 
{
    return IsEnableLayer("VK_LAYER_KHRONOS_validation");
}

bool VulkanInstance::IsEnableDebugReportExtension()
{
    return IsEnableExtension(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
}

void VulkanInstance::Create(std::vector<const char*>& enabledInstanceLayers, std::vector<const char*>& enabledInstanceExtensions)
{
    assert(Instance == VK_NULL_HANDLE);
    EnabledInstanceLayers = enabledInstanceLayers;
    EnabledInstanceExtenisons = enabledInstanceExtensions;
    uint32_t instanceVersion;
    vkEnumerateInstanceVersion(&instanceVersion);

    VkApplicationInfo applicationInfo{
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = 0,
        .pApplicationName = APPLICATION_NAME,
        .applicationVersion = MAKE_APPLICATION_VERSION(1, 0, 0),
        .pEngineName = APPLICATION_NAME,
        .engineVersion = MAKE_APPLICATION_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION,
    };
    VkInstanceCreateInfo instanceCI{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .pApplicationInfo = &applicationInfo,
        .enabledLayerCount = static_cast<uint32_t>(EnabledInstanceLayers.size()),
        .ppEnabledLayerNames = EnabledInstanceLayers.data(),
        .enabledExtensionCount = static_cast<uint32_t>(EnabledInstanceExtenisons.size()),
        .ppEnabledExtensionNames = EnabledInstanceExtenisons.data(),
    };
    VK_ASSERT_SUCCESSED(vkCreateInstance(&instanceCI, nullptr, &Instance));
    if(IsEnableDebugReportExtension()){
        CreateDebugReporter();
    }
}

void VulkanInstance::Destroy() 
{
    if(Instance != VK_NULL_HANDLE){
        DestroyDebugReporter();
        vkDestroyInstance(Instance, nullptr);
        Instance = VK_NULL_HANDLE;
    }
}

void VulkanInstance::CreateDebugReporter() 
{
    VkDebugReportCallbackCreateInfoEXT DebugReportCallbackCI{
        .sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT,
        .flags = VK_DEBUG_REPORT_DEBUG_BIT_EXT | VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
                 VK_DEBUG_REPORT_WARNING_BIT_EXT
        //| VK_DEBUG_REPORT_INFORMATION_BIT_EXT
        ,
        .pfnCallback = DebugReportCallbackEXT,
    };
    auto fpCreateDebugReportCallbackEXT = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(
        vkGetInstanceProcAddr(Instance, "vkCreateDebugReportCallbackEXT"));
    assert(fpCreateDebugReportCallbackEXT != nullptr);
    VK_ASSERT_SUCCESSED(fpCreateDebugReportCallbackEXT(Instance, &DebugReportCallbackCI, nullptr, &fpDebugReportCallbackEXT));
}

void VulkanInstance::DestroyDebugReporter() 
{
    if(fpDebugReportCallbackEXT != VK_NULL_HANDLE)
    {
        auto fpDestroyDebugReportCallbackEXT = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(
            vkGetInstanceProcAddr(Instance, "vkDestroyDebugReportCallbackEXT"));
        assert(fpDestroyDebugReportCallbackEXT != nullptr);
        fpDestroyDebugReportCallbackEXT(Instance, fpDebugReportCallbackEXT, nullptr);
        fpDebugReportCallbackEXT = VK_NULL_HANDLE;
    }
}

std::vector<VkLayerProperties> VulkanInstance::EnumerateInstanceLayerProperties() 
{
    std::vector<VkLayerProperties> layerProperties;
    uint32_t propertyCount = 0;
    VK_ASSERT_SUCCESSED(vkEnumerateInstanceLayerProperties(&propertyCount, nullptr));
    layerProperties.resize(propertyCount);
    VK_ASSERT_SUCCESSED(vkEnumerateInstanceLayerProperties(&propertyCount, layerProperties.data()));
    return layerProperties;
}

std::vector<VkExtensionProperties> VulkanInstance::EnumerateInstanceExtensionProperties(const char* pLayerName) 
{
    std::vector<VkExtensionProperties> extensionProperties;
    uint32_t propertyCount = 0;
    VK_ASSERT_SUCCESSED(vkEnumerateInstanceExtensionProperties(pLayerName, &propertyCount, nullptr));
    extensionProperties.resize(propertyCount);
    VK_ASSERT_SUCCESSED(vkEnumerateInstanceExtensionProperties(pLayerName, &propertyCount, extensionProperties.data()));
    return extensionProperties;
}

std::vector<const char*> VulkanInstance::GetRequiredInstanceExtensions() 
{
    std::vector<const char*> requiredInstanceExtensions;
    requiredInstanceExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
    std::vector<VkExtensionProperties> extensionProperties = EnumerateInstanceExtensionProperties();
    for (uint32_t i = 0; i < extensionProperties.size(); i++) {
        if (strcmp(extensionProperties[i].extensionName, VK_KHR_SURFACE_EXTENSION_NAME) == 0) 
        {
            requiredInstanceExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
        }   
#if defined (Q_OS_WINDOWS)
        else if (strcmp(extensionProperties[i].extensionName, "VK_KHR_win32_surface") == 0) 
        {
            requiredInstanceExtensions.push_back("VK_KHR_win32_surface");
        }
#elif defined (Q_OS_MACOS)
        else if (strcmp(extensionProperties[i].extensionName, "VK_MVK_macos_surface") == 0) 
        {
            requiredInstanceExtensions.push_back("VK_MVK_macos_surface");
        }
#endif
    }
    return requiredInstanceExtensions;
}

VkBool32 DebugReportCallbackEXT(VkDebugReportFlagsEXT flags,
                                          VkDebugReportObjectTypeEXT objectType,
                                          uint64_t object,
                                          size_t location,
                                          int32_t messageCode,
                                          const char *pLayerPrefix,
                                          const char *pMessage,
                                          void *pUserData)
{
    if ((flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) != 0) {
        qDebug() << "[VulkanDebugReport] " 
        << "<DebugReportFlag : Error>" 
        << "<LayerPrefix : " << pLayerPrefix << ">" 
        << "<messageCode : " << messageCode << ">" 
        << "<pMessage : " << pMessage << ">" ;
    }
    if ((flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) != 0 || (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) != 0) {
        qDebug() << "[VulkanDebugReport] " 
        << "<DebugReportFlag : Warning>" 
        << "<LayerPrefix : " << pLayerPrefix << ">" 
        << "<messageCode : " << messageCode << ">" 
        << "<pMessage : " << pMessage << ">" ;
    }
    if ((flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) != 0) {
        qDebug() << "[VulkanDebugReport] " 
        << "<DebugReportFlag : Info>" 
        << "<LayerPrefix : " << pLayerPrefix << ">" 
        << "<messageCode : " << messageCode << ">" 
        << "<pMessage : " << pMessage << ">" ;
    }
    if ((flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) != 0) {
        qDebug() << "[VulkanDebugReport] " 
        << "<DebugReportFlag : Debug>" 
        << "<LayerPrefix : " << pLayerPrefix << ">" 
        << "<messageCode : " << messageCode << ">" 
        << "<pMessage : " << pMessage << ">" ;
    }
    return VK_FALSE;
}


