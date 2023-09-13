#include "Operation.h"

#include "RenderGraph.h"
#include "Job.h"

#include "OtterEngine/Entity/Entity.h"

RG::Operation::Operation(const std::wstring& targetPass) : m_targetName(targetPass), m_pRenderPass(nullptr) {
}

void RG::Operation::Register(const RenderGraph& renderGraph) {
	assert("resource already point somewhere" && m_pRenderPass == nullptr);
	m_pRenderPass = renderGraph.GetRenderPass(m_targetName);
}

void RG::Operation::AddResource(std::shared_ptr<GraphicsResource> resource) {
	m_pResources.push_back(std::move(resource));
}

void RG::Operation::Bind(const Graphics& graphics) const {
	for (const auto& resource : m_pResources) {
		resource->Bind(graphics);
	}
}

void RG::Operation::AssignJob(const Entity& parentEntity) const {
	m_pRenderPass->TakeJob(RG::Job{ this, &parentEntity });
}