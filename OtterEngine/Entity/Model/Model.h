#pragma once

#include "../ShadingEntity.h"
#include <assimp/scene.h>

class Mesh : public ShadingEntity
{
	friend class Model;

public:
	Mesh(const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
		const Camera& camera, bool isStatic, unsigned int meshIndex, const aiMesh* mesh);
	~Mesh() = default;

	static void LoadMesh(unsigned int meshIndex, const aiMesh* mesh);
	void ApplyWorldTransformation(const DirectX::XMMATRIX& worldTransformation) { m_transformation = worldTransformation; }

	const std::vector<std::unique_ptr<GraphicsResource>>& GetCommonResources() const override { return s_commonResources[m_meshIndex]; }

private:
	unsigned int m_meshIndex;

	static std::vector<std::vector<Vertex>> s_vertices;
	static std::vector<std::vector<unsigned short>> s_indices;

	static std::vector<std::vector<std::unique_ptr<GraphicsResource>>> s_commonResources;
};

class Node
{
	friend class Model;

public:
	Node(int nodeIndex, const std::string& name, std::vector<std::shared_ptr<Mesh>>& meshes, const DirectX::XMMATRIX& localTransformation);
	~Node() = default;

private:
	void UpdateTree(const Graphics& graphics, const DirectX::XMMATRIX& parentTransformation,
		std::vector<Vector3> translations, std::vector<Vector3> rotations);
	void AppendChild(std::unique_ptr<Node> pChild);
	void ShowTreeWindow(int& selectIndex);

private:
	int m_nodeIndex;
	std::string m_nodeName;
	DirectX::XMMATRIX m_localTransformation;

	std::vector<std::shared_ptr<Mesh>> m_pMeshes;
	std::vector<std::unique_ptr<Node>> m_pChildren;
};

class Model
{
public:
	Model(const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
		const Camera& camera, bool isStatic, const std::string& path);
	~Model()=default;

	void Render(const Graphics& graphics) const;
	std::unique_ptr<Node> ParseNode(int& nodeIndex, const aiNode* node);

	void ShowControlWindow();

private:
	std::string m_modelName;
	std::unique_ptr<Node> m_pRoot;
	std::vector<std::shared_ptr<Mesh>> m_pAllMeshes;

	// for ImGui use
	int m_selectIndex;
	int m_totalNodes;
	std::vector<Vector3> m_translations;
	std::vector<Vector3> m_rotations;
};