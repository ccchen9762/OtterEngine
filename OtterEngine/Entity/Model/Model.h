#pragma once

#include "../Entity.h"
#include <assimp/scene.h>

struct MeshInformation {
	std::vector<std::vector<VertexTexture>> verticesTexture;
	std::vector<std::vector<VertexNormalMap>> verticesNormalMap;
	std::vector<std::vector<unsigned short>> indices;
	std::vector<bool> hasDiffuseMap;
	std::vector<bool> hasSpecularMap;
	std::vector<bool> hasNormalMap;
	std::vector<std::wstring> diffuseFile;
	std::vector<std::wstring> specularFile;
	std::vector<std::wstring> normalFile;
	std::vector<float> shiness;
	std::wstring directory;
};

class Mesh : public Entity
{
	friend class Model;
	friend class Node;

public:
	Mesh(const Game* game, const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
		bool isStatic, unsigned int meshIndex, const std::wstring& meshPath, const MeshInformation& meshInformation);
	~Mesh() = default;

private:
	void ApplyWorldTransformation(const DirectX::XMMATRIX& worldTransformation) { 
		m_transformation = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z) * worldTransformation; 
	}

private:
	unsigned int m_meshIndex;
	Attributes m_attributes;
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
	Model(const Game* game, const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
		bool isStatic, const std::string& path);
	~Model() = default;

	void Render(const Graphics& graphics) const;
	void ShowControlWindow();

protected:
	void SetupMeshInformation(const aiScene* pModel,
		const std::string& path,
		MeshInformation& s_meshInformation);
	void LoadMesh(unsigned int meshIndex, const aiMesh* pMesh, const aiMaterial* const* ppMaterials,
		MeshInformation& s_meshInformation);
	std::unique_ptr<Node> ParseNode(int& nodeIndex, const aiNode* node);

protected:
	std::string m_modelName;
	std::unique_ptr<Node> m_pRoot;
	std::vector<std::shared_ptr<Mesh>> m_pAllMeshes;

	// for ImGui
	int m_selectIndex;
	int m_totalNodes;
	std::vector<Vector3> m_translations;
	std::vector<Vector3> m_rotations;
};