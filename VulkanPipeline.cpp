#include "VulkanPipeline.h"
#include "VulkanTools.h"
#include "VulkanShaderModule.h"
#include "VulkanDevice.h"
#include "VulkanRenderPass.h"

VulkanPipeline::~VulkanPipeline() 
{
    
}

void VulkanPipeline::Create() 
{
    VkExtent2D swapchainExtent;
    VulkanRenderPass renderPass(Device);
    std::vector<VulkanShaderModule> ShaderModules;
    ShaderModules.push_back(VulkanShaderModule(Device ,VulkanTools::GetShadersPath() + "test.vert.spv"));
    ShaderModules.push_back(VulkanShaderModule(Device ,VulkanTools::GetShadersPath() + "test.frag.spv"));

    std::vector<VkPipelineShaderStageCreateInfo> shaderStageCIs = {
        VkPipelineShaderStageCreateInfo{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .stage = VK_SHADER_STAGE_VERTEX_BIT,
            .module = ShaderModules[0],
            .pName = "main",
            .pSpecializationInfo = nullptr,
        },
        VkPipelineShaderStageCreateInfo{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
            .module = ShaderModules[1],
            .pName = "main",
            .pSpecializationInfo = nullptr,
        }
    };
    VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCI{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .vertexBindingDescriptionCount = 0,
        .pVertexBindingDescriptions = nullptr,
        .vertexAttributeDescriptionCount = 0,
        .pVertexAttributeDescriptions = nullptr,
    };
    VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCI{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE,
    }; 
    VkViewport viewport{
        .x = 0.0f,
        .y = 0.0f,
        .width = float(swapchainExtent.width),
        .height = float(swapchainExtent.height),
        .minDepth = 0.0f,
        .maxDepth = 1.0f,
    };
    VkRect2D scissor{
        .offset = {0, 0},
        .extent = swapchainExtent,
    };
    VkPipelineViewportStateCreateInfo pipelineViewportStateCI{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .viewportCount = 1,
        .pViewports = &viewport,
        .scissorCount = 1,
        .pScissors = &scissor,
    }; 
    VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCI{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .depthClampEnable = VK_FALSE,
        .rasterizerDiscardEnable = VK_FALSE,
        .polygonMode = VK_POLYGON_MODE_FILL,
        .cullMode = VK_CULL_MODE_BACK_BIT,
        .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
        .depthBiasEnable = VK_FALSE,
        .depthBiasConstantFactor = 0,
        .depthBiasClamp = 0,
        .depthBiasSlopeFactor = 0,
        .lineWidth = 1.0f,
    };
    VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCI{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
        .sampleShadingEnable = VK_FALSE,
        .minSampleShading = 0.f,
        .pSampleMask = nullptr,
        .alphaToCoverageEnable = VK_FALSE,
        .alphaToOneEnable = VK_FALSE,
    };
    VkPipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCI{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .depthTestEnable = VK_TRUE,
        .depthWriteEnable = VK_TRUE,
        .depthCompareOp = VK_COMPARE_OP_LESS,
        .depthBoundsTestEnable = VK_FALSE,
        .stencilTestEnable = VK_FALSE,
        .front = {},
        .back = {},
        .minDepthBounds = 0.f,
        .maxDepthBounds = 0.f,
    };
    VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCI{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .logicOpEnable = VK_FALSE,
        .logicOp = VK_LOGIC_OP_COPY,
        .attachmentCount = 0,
        .pAttachments = nullptr,
        .blendConstants[0] = 0.f,
        .blendConstants[1] = 0.f,
        .blendConstants[2] = 0.f,
        .blendConstants[3] = 0.f,
    };
    VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentState{
        .blendEnable = VK_FALSE,
        .srcColorBlendFactor = VK_BLEND_FACTOR_ZERO,
        .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
        .colorBlendOp = VK_BLEND_OP_ADD,
        .srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
        .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
        .alphaBlendOp = VK_BLEND_OP_ADD,
        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
    };
    VkPipelineDynamicStateCreateInfo pipelineDynamicStateCI{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .dynamicStateCount = 0,
        .pDynamicStates = nullptr,
    };

    VkPipelineLayoutCreateInfo pipelineLayoutCI{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .setLayoutCount = 0,
        .pSetLayouts = nullptr,
        .pushConstantRangeCount = 0,
        .pPushConstantRanges = nullptr,
    }; 

    VK_ASSERT_SUCCESSED(vkCreatePipelineLayout(Device, &pipelineLayoutCI, nullptr, &PipelineLayout));

    VkGraphicsPipelineCreateInfo GraphicsPipelineCI{
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount = static_cast<uint32_t>(shaderStageCIs.size()),
        .pStages = shaderStageCIs.data(),
        .pVertexInputState = &pipelineVertexInputStateCI,
        .pInputAssemblyState = &pipelineInputAssemblyStateCI,
        //.pTessellationState = &tessellationStateCI,
        .pViewportState = &pipelineViewportStateCI,
        .pRasterizationState = &pipelineRasterizationStateCI,
        .pMultisampleState = &pipelineMultisampleStateCI,
        .pDepthStencilState = &pipelineDepthStencilStateCI,
        .pColorBlendState = &pipelineColorBlendStateCI,
        .pDynamicState = &pipelineDynamicStateCI,
        .layout = PipelineLayout,
        .renderPass = renderPass,
        .subpass = 0,
        //.basePipelineHandle = basePipelineHandle,
        //.basePipelineIndex = basePipelineIndex,
    };
    VK_ASSERT_SUCCESSED(vkCreateGraphicsPipelines(Device, VK_NULL_HANDLE, 1, &GraphicsPipelineCI, nullptr, &Pipeline));
}

void VulkanPipeline::Destroy() 
{
    if (Pipeline != VK_NULL_HANDLE) {
        vkDestroyPipeline(Device, Pipeline, nullptr);
        Pipeline = VK_NULL_HANDLE;
    }
    if (PipelineLayout != VK_NULL_HANDLE) {
        vkDestroyPipelineLayout(Device, PipelineLayout, nullptr);
        PipelineLayout = VK_NULL_HANDLE;
    }
}
