#pragma once

#include "OtterEngine/Camera/Camera.h"
#include "OtterEngine/Common/Vertex.h"
#include "OtterEngine/Common/Math/MathUtils.h"

#include "OtterEngine/Graphics/Resource/IndexBuffer.h"
#include "OtterEngine/Graphics/Resource/VertexBuffer.h"

class Game;

namespace RG {
	class RenderGraph;
	class Operation;
}

class Entity
{
public:
	Entity(const Game* game, const Vector3& translation, const Vector3& rotation, const Vector3& scale, 
		size_t indicesSize);
	virtual ~Entity() = default; // use virtual make sure derived class destructors are called properly

	const std::wstring& GetUID() const {
		assert("Entity UID not generated." && !m_UID.empty());
		return m_UID;
	}
	size_t GetIndicesSize() const { return m_indicesSize; }
	const DirectX::XMMATRIX& GetTransformMatrix() const { return m_transformation; }
	const DirectX::XMMATRIX& GetViewProjectionMatrix() const;
	
	void Register(const RG::RenderGraph& renderGraph);
	void Translate(const Vector3& translation) { m_translation = translation; }
	void Update();
	void AssignJob() const;
	void Bind(const Graphics& graphics) const;

protected:
	std::wstring m_UID;
	Vector3 m_translation;
	Vector3 m_rotation;
	Vector3 m_scale;
	size_t m_indicesSize;
	const Game* m_parentGame;

	DirectX::XMMATRIX m_transformation;
	std::shared_ptr<IndexBuffer> m_pIndicesBuffer;
	std::shared_ptr<VertexBuffer> m_pVerticesBuffer;
	std::shared_ptr<ConstantBufferTransformation> m_pTransformationBuffer;
	std::vector<RG::Operation> m_operations;

	float m_speed; // for testing
};