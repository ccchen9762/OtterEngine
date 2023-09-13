#include "RenderGraph.h"

#include "Pass/Pass.h"
#include "Pass/ClearBufferPass.h"
#include "Pass/RenderPass.h"
#include "OtterEngine/Graphics/Graphics.h"
#include "OtterEngine/Graphics/Resource/GraphicsResource.h"
#include "OtterEngine/Graphics/Resource/DepthStencil.h"
#include "OtterEngine/Graphics/Resource/RenderTarget.h"
#include "OtterEngine/Common/constants.h"

RG::RenderGraph::RenderGraph(const Graphics& graphics) : 
	m_pBackBuffer(graphics.GetRenderTarget()),
	m_pZBuffer(std::make_unique<DepthStencil>(
		graphics, kRenderWidth, kRenderHeight
	)) {

	AddResouce(L"backbuffer", m_pBackBuffer);
	AddResouce(L"zbuffer", m_pZBuffer);
	
	{
		std::unique_ptr<ClearBufferPass> pass = std::make_unique<ClearBufferPass>(L"ClearRenderTarget");
		pass->RegisterResource(L"backbuffer");
		AddPass(std::move(pass));
	}
	{
		std::unique_ptr<ClearBufferPass> pass = std::make_unique<ClearBufferPass>(L"ClearDepthBuffer");
		AddPass(std::move(pass));
	}
	{
		//std::unique_ptr<ClearBufferPass> pass = std::make_unique<ClearBufferPass>(L"ClearRenderTarget");
		//AddPass(std::move(pass));
	}
}

std::shared_ptr<RG::RenderPass> RG::RenderGraph::GetRenderPass(const std::wstring& passName) const {
	for (const std::shared_ptr<RG::Pass>& pass : m_pPasses ) {
		if (pass->GetName() == passName) {
			return std::dynamic_pointer_cast<RG::RenderPass>(pass);
		}
	}
	
	assert("pass not found");
	return nullptr;
}

void RG::RenderGraph::AddPass(std::unique_ptr<Pass> pass) {
	m_pPasses.push_back(std::move(pass));
}

void RG::RenderGraph::AddResouce(const std::wstring& name, std::shared_ptr<GraphicsResource> pResource) {
	if (m_pResourcesMap.find(name) != m_pResourcesMap.end()) {
		assert("Render Graph resources cannot have same name");
	}

	m_pResourcesMap[name] = pResource;
}

void RG::RenderGraph::Execute(const Graphics& graphics) {
	for (const std::shared_ptr<RG::Pass>& pass : m_pPasses) {
		pass->Execute(graphics);
	}
}
