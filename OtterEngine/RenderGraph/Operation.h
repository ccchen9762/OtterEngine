#pragma once

#include <string>

#include "OtterEngine/RenderGraph/Pass/RenderPass.h"

class Entity;

namespace RG {
	
	class RenderGraph;

	class Operation
	{
	public:
		Operation(const std::wstring& m_renderPass);
		~Operation() = default;

		void Register(const RenderGraph& renderGraph);
		void AddResource(std::shared_ptr<GraphicsResource> resource);
		void Bind(const Graphics& graphics) const;
		void AssignJob(const Entity& parentEntity) const;

	private:
		std::wstring m_targetName;
		std::shared_ptr<RenderPass> m_pRenderPass;
		std::vector<std::shared_ptr<GraphicsResource>> m_pResources;
	};
}