#pragma once

#include <DirectXMath.h>

#include "OtterEngine/Graphics/Resource/GraphicsResource.h"
#include "OtterEngine/Camera/Camera.h"
#include "OtterEngine/Common/Math/MathUtils.h"
#include "OtterEngine/Common/constants.h"
#include "OtterEngine/Common/Vertex.h"

class Entity
{
public:
	Entity(const Vector3& translation, const Vector3& rotation, const Vector3& scale, size_t indicesSize,
		const Camera& camera, bool isStatic);
	virtual ~Entity() = default; // make sure derived class destructors are called properly

	virtual void Update();
	void Render(const Graphics& graphics) const;

	DirectX::XMMATRIX GetTransformMatrix() const;
	const DirectX::XMMATRIX& GetViewMatrix() const;
	const DirectX::XMMATRIX& GetViewProjectionMatrix() const;

	void Translate(const Vector3& translation) { m_translation = translation; }
	
private:
	virtual const std::vector<std::unique_ptr<GraphicsResource>>& GetCommonResources() const = 0;

protected:
	bool m_isStatic;
	Vector3 m_translation;
	Vector3 m_rotation;
	Vector3 m_scale;
	size_t m_indicesSize;

	const Camera& m_camera;
	std::vector<std::unique_ptr<GraphicsResource>> m_uniqueResources;
	
	float m_speed; // for testing
};