#pragma once

#include "Pass.h"

class RenderTarget;
class DepthStencil;

namespace RG {
	
	class Job;
	
	class RenderPass : public Pass
	{
	public:
		void AddBind();
		void BindBufferResources(const Graphics& graphics) const;
		void TakeJob(const Job& job);
		void Execute(const Graphics& graphics) const override;

	private:
		std::shared_ptr<RenderTarget> renderTarget;
		std::shared_ptr<DepthStencil> depthStencil;
		std::vector<Job> m_jobs;
	};
}
