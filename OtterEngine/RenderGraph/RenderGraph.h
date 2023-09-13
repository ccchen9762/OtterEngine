#pragma once

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

class Graphics;
class GraphicsResource;
class DepthStencil;
class RenderTarget;

namespace RG {

	class Pass;
	class RenderPass;

	class RenderGraph
	{
	public:
		RenderGraph(const Graphics& graphics);
		virtual ~RenderGraph() = default;

		std::shared_ptr<RG::RenderPass> GetRenderPass(const std::wstring& passName) const;

		void AddPass(std::unique_ptr<Pass> pass);
		void AddResouce(const std::wstring& name, std::shared_ptr<GraphicsResource> pResource);

		void Execute(const Graphics& graphics);

	private:
		std::shared_ptr<RenderTarget> m_pBackBuffer;
		std::shared_ptr<DepthStencil> m_pZBuffer;
		std::unordered_map<std::wstring, std::shared_ptr<GraphicsResource>> m_pResourcesMap;
		std::vector<std::shared_ptr<Pass>> m_pPasses;
	};
}