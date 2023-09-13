#include "Job.h"

#include "OtterEngine/Entity/Entity.h"
#include "OtterEngine/RenderGraph/Operation.h"

RG::Job::Job(const RG::Operation* operation, const Entity* entity) :
	m_pParentEntity(entity), m_pOperation(operation) {
}

void RG::Job::Execute(const Graphics& graphics) const {
	m_pParentEntity->Bind(graphics);
	m_pOperation->Bind(graphics);
	graphics.RenderIndexed(m_pParentEntity->GetIndicesSize());
}