#pragma once
#include <exception>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#define VK_EXCEPT(...) VkException(__LINE__, __FILE__, ##__VA_ARGS__)
#define VK_THROW_EXCEPT(...) throw VK_EXCEPT(__VA_ARGS__)

class VkException : public std::exception
{
public:
    explicit VkException(int line, const char *file, VkResult result) noexcept;
    explicit VkException(int line, const char *file, std::string message) noexcept;
    const char *what() const noexcept override;

private:
    int Line;
    std::string File;
    std::string Message;

protected:
    mutable std::string Buffer;
};



