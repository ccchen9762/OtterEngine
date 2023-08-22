#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "OtterEngine/Graphics/Resource/VertexBuffer.h"
#include "OtterEngine/Graphics/Resource/IndexBuffer.h"

std::vector<std::vector<Vertex>> Mesh::s_vertices;
std::vector<std::vector<unsigned short>> Mesh::s_indices;

std::vector<std::vector<std::unique_ptr<GraphicsResource>>> Mesh::s_commonResources;

Mesh::Mesh(const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale, 
	const Camera& camera, bool isStatic, unsigned int meshIndex, const aiMesh* mesh)
	: ShadingEntity(graphics, translation, rotation, scale, 0, camera, isStatic), m_meshIndex(meshIndex) {

	if (s_commonResources[meshIndex].empty()) {
		LoadMesh(meshIndex, mesh);

		// buffers
		s_commonResources[meshIndex].push_back(std::make_unique<VertexBuffer>(graphics,
			s_vertices[meshIndex].data(), static_cast<unsigned int>(sizeof(Vertex)), s_vertices[meshIndex].size()));
		s_commonResources[meshIndex].push_back(std::make_unique<IndexBuffer>(graphics, s_indices[meshIndex]));
	}
	m_indicesSize = s_indices[meshIndex].size(); // make sure size in Entity changes
}

void Mesh::LoadMesh(unsigned int meshIndex, const aiMesh* mesh) {
	s_vertices[meshIndex].reserve(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		s_vertices[meshIndex].emplace_back(
			DirectX::XMVECTOR{ mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1.0f },
			Color4{ 1.0f, 1.0f, 1.0f, 1.0f },
			Normal{ mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z }
		);
	}

	s_indices[meshIndex].reserve(mesh->mNumFaces * 3);
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		const aiFace& face = mesh->mFaces[i];
		assert("Model face are not all triangles" && face.mNumIndices == 3);

		s_indices[meshIndex].push_back(face.mIndices[0]);
		s_indices[meshIndex].push_back(face.mIndices[2]);
		s_indices[meshIndex].push_back(face.mIndices[1]);
	}
}

void Mesh::ApplyWorldTransformation(const DirectX::XMMATRIX& worldTransformation) {
	m_transformation = worldTransformation;
}

// ========================= Node =========================

Node::Node(std::vector<std::shared_ptr<Mesh>>& meshes, const DirectX::XMMATRIX& localTransformation)
	: m_pMeshes(std::move(meshes)), m_localTransformation(localTransformation) {
}

void Node::Update(const Graphics& graphics, const DirectX::XMMATRIX& parentTransformation) {

	const DirectX::XMMATRIX worldTransformation = m_localTransformation * parentTransformation;

	for (const std::shared_ptr<Mesh>& mesh : m_pMeshes) {
		mesh->ApplyWorldTransformation(worldTransformation);
		mesh->Update(graphics);
	}

	for (const std::unique_ptr<Node>& node : m_pChildren) {
		node->Update(graphics, worldTransformation);
	}
}

void Node::AppendChild(std::unique_ptr<Node> pChild) {
	assert("Child node is empty" && pChild);
	m_pChildren.push_back(std::move(pChild));
}

// ========================= Model =========================

Model::Model(const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
	const Camera& camera, bool isStatic, const std::string& path) {
	Assimp::Importer imp;
	const aiScene* pModel = imp.ReadFile(path, 
		aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

	size_t numMeshes = pModel->mNumMeshes;
	Mesh::s_vertices.resize(numMeshes);
	Mesh::s_indices.resize(numMeshes);
	Mesh::s_commonResources.resize(numMeshes);

	for (size_t i = 0; i < numMeshes; i++) {
		m_pAllMeshes.push_back(std::make_unique<Mesh>(
			graphics, translation, rotation, scale, camera, isStatic, i, pModel->mMeshes[i]
		));
	}

	m_pRoot = ParseNode(pModel->mRootNode);
}

void Model::Update(const Graphics& graphics) const {
	m_pRoot->Update(graphics, DirectX::XMMatrixIdentity());
}

std::unique_ptr<Node> Model::ParseNode(const aiNode* node) {

	std::vector<std::shared_ptr<Mesh>> pMeshes;
	pMeshes.reserve(node->mNumMeshes);
	for (size_t i = 0; i < node->mNumMeshes; i++) {
		const unsigned int meshIndex = node->mMeshes[i];
		pMeshes.push_back(m_pAllMeshes[meshIndex]);
	}

	const DirectX::XMMATRIX localTransformation = DirectX::XMMatrixTranspose(
		DirectX::XMLoadFloat4x4(reinterpret_cast<const DirectX::XMFLOAT4X4*>(&node->mTransformation)));
	std::unique_ptr<Node> pNode = std::make_unique<Node>(pMeshes, localTransformation);
	for (size_t i = 0; i < node->mNumChildren; i++) {
		pNode->AppendChild(ParseNode(node->mChildren[i]));
	}

	return pNode;
}
