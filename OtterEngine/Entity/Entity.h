#pragma once

#include <DirectXMath.h>

#include "OtterEngine/Graphics/Resource/GraphicsResource.h"
#include "OtterEngine/Camera/Camera.h"
#include "OtterEngine/Common/Math/Vector3.h"
#include "OtterEngine/Common/constants.h"

struct Vertex;

class Entity
{
public:
	Entity(Vector3 rotation, Vector3 translation, Vector3 revolution, Vector3 scale, size_t indicesSize, 
		const DirectX::XMMATRIX& viewProjectionMatrix, float speed);
	virtual ~Entity() = default; // make sure derived class destructors are called properly

	virtual void Update();
	void Render(const Graphics& graphics) const;

	DirectX::XMMATRIX GetTransformMatrix() const;
	const DirectX::XMMATRIX& GetViewProjectionMatrix() const;
	
private:
	virtual const std::vector<std::unique_ptr<GraphicsResource>>& GetCommonResources() const = 0;

protected:
	Vector3 m_rotation;
	Vector3 m_translation;
	Vector3 m_revolution;
	Vector3 m_scale;
	size_t m_indicesSize;
	const DirectX::XMMATRIX& m_viewProjectionMatrix;
	std::vector<std::unique_ptr<GraphicsResource>> m_uniqueResources;
	
	float m_speed; // for testing
};