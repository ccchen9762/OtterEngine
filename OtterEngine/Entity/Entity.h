#pragma once

#include "OtterEngine/Camera/Camera.h"
#include "OtterEngine/Common/Vertex.h"
#include "OtterEngine/Common/Math/MathUtils.h"

class Game;

class Entity
{
public:
	Entity(const Game& game, const Vector3& translation, const Vector3& rotation, const Vector3& scale, 
		size_t indicesSize, bool isStatic);
	virtual ~Entity() = default; // use virtual make sure derived class destructors are called properly

	const std::wstring& GetUID() const {
		assert("Entity UID not generated." && !m_UID.empty());
		return m_UID;
	}

	virtual void Update();
	virtual void Render(const Graphics& graphics) const;

	const DirectX::XMMATRIX& GetTransformMatrix() const { return m_transformation; }
	const DirectX::XMMATRIX& GetViewProjectionMatrix() const;
	
	void Translate(const Vector3& translation) { m_translation = translation; }

protected:
	void AddShadingResource(const Graphics& graphics);
	void AddTextureShadingResource(const Graphics& graphics, bool hasSpecularMap);
	void AddBasicResource(const Graphics& graphics);
	void AddTextureBasicResource(const Graphics& graphics);

protected:
	std::wstring m_UID;
	Vector3 m_translation;
	Vector3 m_rotation;
	Vector3 m_scale;
	size_t m_indicesSize;
	bool m_isStatic;
	const Game& m_parentGame;

	DirectX::XMMATRIX m_transformation;
	std::vector<std::shared_ptr<GraphicsResource>> m_graphicsResources;
	
	float m_speed; // for testing
};