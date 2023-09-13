#include "ClearBufferPass.h"

ClearBufferPass::ClearBufferPass(const std::wstring& name) : Pass(name) {
	RegisterResource();
}

void ClearBufferPass::Execute(const Graphics& graphics) const {
	m_pBuffer->Clear(graphics);
}
