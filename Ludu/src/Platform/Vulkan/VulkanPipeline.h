#pragma once

#include "Core/Core.h"

namespace Ludu
{

    class VulkanPipeline
    {
    public:
        const std::string VERT_FILEPATH = "../simple_shader.vert.spv";
        const std::string FRAG_FILEPATH = "../simple_shader.frag.spv";

        VulkanPipeline();

        void CreateGraphicsPipeline();
        
    };
}