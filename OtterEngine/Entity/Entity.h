#pragma once

#include "OtterEngine/Graphics/Resource/GraphicsResource.h"
#include "OtterEngine/Camera/Camera.h"
#include "OtterEngine/Common/Vertex.h"
#include "OtterEngine/Common/Math/MathUtils.h"
#include "OtterEngine/Common/constants.h"

class Entity
{
public:
	Entity(const Vector3& translation, const Vector3& rotation, const Vector3& scale, size_t indicesSize,
		const Camera& camera, bool isStatic);
	virtual ~Entity() = default; // use virtual make sure derived class destructors are called properly

	virtual void Update();
	virtual void Render(const Graphics& graphics) const;

	const DirectX::XMMATRIX& GetTransformMatrix() const { return m_transformation; }
	const DirectX::XMMATRIX& GetViewMatrix() const { return m_camera.GetViewProjectionMatrix(); }
	const DirectX::XMMATRIX& GetViewProjectionMatrix() const { return m_camera.GetViewProjectionMatrix(); }
	
	void Translate(const Vector3& translation) { m_translation = translation; }
	
private:
	virtual const std::vector<std::unique_ptr<GraphicsResource>>& GetShadingResources() const = 0;
	virtual const std::vector<std::unique_ptr<GraphicsResource>>& GetCommonResources() const = 0;

protected:
	Vector3 m_translation;
	Vector3 m_rotation;
	Vector3 m_scale;
	size_t m_indicesSize;
	const Camera& m_camera;
	bool m_isStatic;

	DirectX::XMMATRIX m_transformation;
	std::vector<std::unique_ptr<GraphicsResource>> m_uniqueResources;
	
	float m_speed; // for testing
};