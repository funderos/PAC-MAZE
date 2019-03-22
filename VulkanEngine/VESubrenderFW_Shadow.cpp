/**
* The Vienna Vulkan Engine
*
* (c) bei Helmut Hlavacs, University of Vienna
*
*/


#include "VEInclude.h"


namespace ve {

	/**
	* \brief Initialize the subrenderer
	*
	* Create descriptor set layout, pipeline layout and the PSO
	*
	*/
	void VESubrenderFW_Shadow::initSubrenderer() {
		VESubrender::initSubrenderer();

		//per object resources, set 0
		vh::vhRenderCreateDescriptorSetLayout(getRendererForwardPointer()->getDevice(),
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER },
			{ VK_SHADER_STAGE_VERTEX_BIT, },
			&m_descriptorSetLayout);

		vh::vhPipeCreateGraphicsPipelineLayout(getRendererForwardPointer()->getDevice(),
			{ m_descriptorSetLayout, getRendererForwardPointer()->getDescriptorSetLayoutPerFrame(), getRendererForwardPointer()->getDescriptorSetLayoutShadow() },
			&m_pipelineLayout);

		vh::vhPipeCreateGraphicsPipeline(getRendererForwardPointer()->getDevice(),
			"shader/Forward/Shadow/vert.spv", "", //"shader/Forward/Shadow/frag.spv",
			getRendererForwardPointer()->getShadowMapExtent(),
			m_pipelineLayout, getRendererForwardPointer()->getRenderPassShadow(),
			&m_pipeline);
	}

	/**
	*
	* \brief Add an entity to the list of associated entities.
	*
	* \param[in] pEntity Pointer to the entity to include into the list.
	* \returns the index of the entity in the list.
	*
	*/
	void VESubrenderFW_Shadow::addEntity(VEEntity *pEntity) {
		m_entities.push_back(pEntity);
	}

	/**
	*
	* \brief Bind default descriptor sets - 0...per object 1...per frame
	*
	* The function binds the default descriptor sets -  0...per object 1...per frame.
	* Can be overloaded.
	*
	* \param[in] commandBuffer The command buffer to record into all draw calls
	* \param[in] imageIndex Index of the current swap chain image
	* \param[in] entity Pointer to the entity to draw
	*
	*/
	void VESubrenderFW_Shadow::bindDescriptorSets(VkCommandBuffer commandBuffer, uint32_t imageIndex, VEEntity *entity) {
		//set 0...per object
		//set 1...per frame
		//set 2...shadow
		std::vector<VkDescriptorSet> sets =
		{
			entity->m_descriptorSets[imageIndex],
			getRendererForwardPointer()->getDescriptorSetsPerFrame()[imageIndex],
			getRendererForwardPointer()->getDescriptorSetsShadow()[imageIndex]
		};

		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, (uint32_t)sets.size(), sets.data(), 0, nullptr);
	}


	/**
	* \brief Draw all associated entities for the shadow pass
	*
	* The subrenderer maintains a list of all associated entities. In this function it goes through all of them
	* and draws them. A vector is used in order to be able to parallelize this in case thousands or objects are in the list
	*
	* \param[in] commandBuffer The command buffer to record into all draw calls
	* \param[in] imageIndex Index of the current swap chain image
	*
	*/
	void VESubrenderFW_Shadow::draw(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);	//bind the PSO

		//go through all entities and draw them
		for (auto pEntity : getSceneManagerPointer()->m_entities) {
			if (pEntity.second->m_drawEntity && pEntity.second->m_castsShadow) {
				drawEntity(commandBuffer, imageIndex, pEntity.second);
			}
		}
	}



}
