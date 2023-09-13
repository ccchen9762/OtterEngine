#pragma once

#include "Pass.h"
#include "OtterEngine/Graphics/Resource/BufferResource.h"

namespace RG {
	class ClearBufferPass : public Pass
	{
	public:
		ClearBufferPass(const std::wstring& name);
		~ClearBufferPass() = default;

		void Execute(const Graphics& graphics) const override;

	private:
		std::shared_ptr<BufferResource> m_pBuffer;
	};
}