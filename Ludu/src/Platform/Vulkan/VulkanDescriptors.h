#pragma once

#include "Platform/Vulkan/VulkanDevice.h"

// std
#include <memory>
#include <unordered_map>
#include <vector>

namespace Ludu {

    class VulkanDescriptorLayout {
    public:
        class Builder {
        public:
            Builder(VulkanDevice& device) : m_Device{ device } {}

            Builder& addBinding(
                uint32_t binding,
                VkDescriptorType descriptorType,
                VkShaderStageFlags stageFlags,
                uint32_t count = 1);
            std::unique_ptr<VulkanDescriptorLayout> build() const;

        private:
            VulkanDevice& m_Device;
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
        };

        VulkanDescriptorLayout(VulkanDevice& device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
        ~VulkanDescriptorLayout();
        VulkanDescriptorLayout(const VulkanDescriptorLayout&) = delete;
        VulkanDescriptorLayout& operator=(const VulkanDescriptorLayout&) = delete;

        VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

    private:
        VulkanDevice& m_Device;
        VkDescriptorSetLayout descriptorSetLayout;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

        friend class VulkanDescriptorWriter;
    };

    class VulkanDescriptorPool {
    public:
        class Builder {
        public:
            Builder(VulkanDevice& device) : m_Device{ device } {}

            Builder& addPoolSize(VkDescriptorType descriptorType, uint32_t count);
            Builder& setPoolFlags(VkDescriptorPoolCreateFlags flags);
            Builder& setMaxSets(uint32_t count);
            std::unique_ptr<VulkanDescriptorPool> build() const;

        private:
            VulkanDevice& m_Device;
            std::vector<VkDescriptorPoolSize> poolSizes{};
            uint32_t maxSets = 1000;
            VkDescriptorPoolCreateFlags poolFlags = 0;
        };

        VulkanDescriptorPool(
            VulkanDevice& device,
            uint32_t maxSets,
            VkDescriptorPoolCreateFlags poolFlags,
            const std::vector<VkDescriptorPoolSize>& poolSizes);
        ~VulkanDescriptorPool();
        VulkanDescriptorPool(const VulkanDescriptorPool&) = delete;
        VulkanDescriptorPool& operator=(const VulkanDescriptorPool&) = delete;

        bool allocateDescriptor(
            const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;

        void freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;

        void resetPool();

    private:
        VulkanDevice& m_Device;
        VkDescriptorPool descriptorPool;

        friend class VulkanDescriptorWriter;
    };

    class VulkanDescriptorWriter {
    public:
        VulkanDescriptorWriter(VulkanDescriptorLayout& setLayout, VulkanDescriptorPool& pool);

        VulkanDescriptorWriter& writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
        VulkanDescriptorWriter& writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

        bool build(VkDescriptorSet& set);
        void overwrite(VkDescriptorSet& set);

    private:
        VulkanDescriptorLayout& setLayout;
        VulkanDescriptorPool& pool;
        std::vector<VkWriteDescriptorSet> writes;
    };

}  // namespace lve