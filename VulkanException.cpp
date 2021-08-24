#include "VulkanException.h"
#include <VulkanTools.h>

VkException::VkException(int line, const char *file, VkResult result) noexcept : Line(line), File(file), Message(VulkanTools::ToString(result))
{
    
}

VkException::VkException(int line, const char *file, std::string message) noexcept : Line(line), File(file), Message(message)
{
}

const char* VkException::what() const noexcept
{
    std::ostringstream oss;
    oss << File << "(" << Line << ") <VkException : " << Message << ">";
    Buffer = oss.str();
    return Buffer.c_str();
}
