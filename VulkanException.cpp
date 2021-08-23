#include "VulkanException.h"
#include <VulkanTools.h>

VkException::VkException(int line, const char *file, VkResult result) noexcept : _Line(line), _File(file), _Message(VulkanTools::ToString(result))
{
    
}

VkException::VkException(int line, const char *file, std::string message) noexcept : _Line(line), _File(file), _Message(message)
{
}

const char* VkException::what() const noexcept
{
    std::ostringstream oss;
    oss << _File << "(" << _Line << ") <VkException : " << _Message << ">";
    _Buffer = oss.str();
    return _Buffer.c_str();
}
