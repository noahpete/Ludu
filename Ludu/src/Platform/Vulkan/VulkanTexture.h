#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <vulkan/vulkan.h>

#include <string>

namespace Ludu {

    class VulkanTexture
    {
    public:
        VulkanTexture(const std::string& filepath);
        ~VulkanTexture();

        void CreateTextureImage(const std::string& filepath);
        void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
        void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

    private:
        VkImage m_Image;
        VkDeviceMemory m_ImageMemory;

    };
}