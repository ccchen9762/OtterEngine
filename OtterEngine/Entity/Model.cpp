#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "OtterEngine/Imgui/imgui.h"

#include "OtterEngine/Graphics/ResourcePool.h"
#include "OtterEngine/Graphics/Resource/VertexBuffer.h"
#include "OtterEngine/Graphics/Resource/IndexBuffer.h"
#include "OtterEngine/Graphics/Resource/Texture.h"
#include "OtterEngine/Common/constants.h"
#include "OtterEngine/Common/Utils.h"

std::vector<std::vector<VertexTexture>> Mesh::s_vertices;
std::vector<std::vector<unsigned short>> Mesh::s_indices;
std::wstring Mesh::s_path;

Mesh::Mesh(const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale, 
	const Camera& camera, bool isStatic, unsigned int meshIndex, const aiMesh* pMesh, const aiMaterial* const* ppMaterials)
	: Entity(translation, rotation, scale, 0, camera, isStatic), m_meshIndex(meshIndex), m_attributes({ 5.0, true }) {

	if (s_indices[meshIndex].empty()) {
		LoadMesh(graphics, meshIndex, pMesh, ppMaterials);

		// remember to set indice size!!!!!!!!!!!!!!!!!!
		m_indicesSize = s_indices[meshIndex].size();
	}

	std::wstring meshName;
	StringToWString(pMesh->mName.C_Str(), meshName);

	// buffers & textures
	std::shared_ptr<GraphicsResource> pVertexBuffer = ResourcePool::GetResource<VertexBuffer>(
		graphics, s_vertices[meshIndex].data(), sizeof(VertexTexture), s_vertices[meshIndex].size(),
		VertexBuffer::Topology::Triangle, L"#Mesh#" + s_path + meshName);
	m_graphicsResources.push_back(std::move(pVertexBuffer));

	std::shared_ptr<GraphicsResource> pIndexBuffer = ResourcePool::GetResource<IndexBuffer>(
		graphics, s_indices[meshIndex], L"#Mesh#" + s_path + meshName);
	m_graphicsResources.push_back(std::move(pIndexBuffer));

	std::shared_ptr<GraphicsResource> pConstantBufferTransformation = ResourcePool::GetResource<ConstantBufferTransformation>(
		graphics, *this);
	m_graphicsResources.push_back(std::move(pConstantBufferTransformation));

	AddTextureShadingResource(graphics);
}

void Mesh::LoadMesh(const Graphics& graphics, unsigned int meshIndex, const aiMesh* pMesh, const aiMaterial* const* ppMaterials) {
	s_vertices[meshIndex].reserve(pMesh->mNumVertices);
	for (unsigned int i = 0; i < pMesh->mNumVertices; i++) {
		s_vertices[meshIndex].emplace_back(
			DirectX::XMVECTOR{ pMesh->mVertices[i].x, pMesh->mVertices[i].y, pMesh->mVertices[i].z, 1.0f },
			DirectX::XMFLOAT2{ pMesh->mTextureCoords[0][i].x, pMesh->mTextureCoords[0][i].y },
			Normal{ pMesh->mNormals[i].x, pMesh->mNormals[i].y, pMesh->mNormals[i].z }
		);
	}

	s_indices[meshIndex].reserve(pMesh->mNumFaces * 3);
	for (unsigned int i = 0; i < pMesh->mNumFaces; i++) {
		const aiFace& face = pMesh->mFaces[i];
		assert("Model face are not all triangles" && face.mNumIndices == 3);

		s_indices[meshIndex].push_back(face.mIndices[0]);
		s_indices[meshIndex].push_back(face.mIndices[1]);
		s_indices[meshIndex].push_back(face.mIndices[2]);
	}

	const aiMaterial* pMaterial = ppMaterials[pMesh->mMaterialIndex];
	aiString textureName;
	// load diffuse map
	if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &textureName) == aiReturn_SUCCESS) {
		std::wstring materialName;
		StringToWString(textureName.C_Str(), materialName);
		std::wstring fileLocation = s_path + materialName;

		std::shared_ptr<GraphicsResource> pTexture = ResourcePool::GetResource<Texture>(
			graphics, fileLocation, 0u);
		m_graphicsResources.push_back(std::move(pTexture));
	}
	// load specular map
	if (pMaterial->GetTexture(aiTextureType_SPECULAR, 0, &textureName) == aiReturn_SUCCESS) {
		std::wstring materialName;
		StringToWString(textureName.C_Str(), materialName);
		std::wstring fileLocation = s_path + materialName;

		std::shared_ptr<GraphicsResource> pTexture = ResourcePool::GetResource<Texture>(
			graphics, fileLocation, 1u);
		m_graphicsResources.push_back(std::move(pTexture));
	}
	else {
		pMaterial->Get(AI_MATKEY_SHININESS, m_attributes.shiness);
		m_attributes.hasSpecularMap = false;
		
		std::shared_ptr<GraphicsResource> pConstantBufferVertex = ResourcePool::GetResource<ConstantBufferVertex<Attributes>>(
			graphics, m_attributes, VertexConstantBufferType::Attributes, GetUID());
		m_graphicsResources.push_back(std::move(pConstantBufferVertex));

		std::shared_ptr<GraphicsResource> pConstantBufferPixel = ResourcePool::GetResource<ConstantBufferPixel<Attributes>>(
			graphics, m_attributes, PixelConstantBufferType::Attributes, GetUID());
		m_graphicsResources.push_back(std::move(pConstantBufferPixel));
		
		std::shared_ptr<GraphicsResource> pTexture = ResourcePool::GetResource<Texture>(
			graphics, L"", 1u);
		m_graphicsResources.push_back(std::move(pTexture));
	}
}

// ========================= Node =========================

Node::Node(int nodeIndex, const std::string& name, std::vector<std::shared_ptr<Mesh>>& meshes, const DirectX::XMMATRIX& localTransformation)
	:m_nodeIndex(nodeIndex), m_nodeName(name), m_pMeshes(meshes), m_localTransformation(localTransformation) {
}

void Node::UpdateTree(const Graphics& graphics, const DirectX::XMMATRIX& parentTransformation,
	std::vector<Vector3> translations, std::vector<Vector3> rotations) {

	const DirectX::XMMATRIX controlTransformation =
		DirectX::XMMatrixRotationRollPitchYaw(rotations[m_nodeIndex].x, rotations[m_nodeIndex].y, rotations[m_nodeIndex].z) *
		DirectX::XMMatrixTranslation(translations[m_nodeIndex].x, translations[m_nodeIndex].y, translations[m_nodeIndex].z);
	const DirectX::XMMATRIX worldTransformation = controlTransformation * m_localTransformation * parentTransformation;

	for (const std::shared_ptr<Mesh>& mesh : m_pMeshes) {
		mesh->Update();
		
		mesh->ApplyWorldTransformation(worldTransformation);
		mesh->Render(graphics);
	}

	for (const std::unique_ptr<Node>& node : m_pChildren) {
		node->UpdateTree(graphics, worldTransformation, translations, rotations);
	}
}

void Node::AppendChild(std::unique_ptr<Node> pChild) {
	assert("Child node is empty" && pChild);
	m_pChildren.push_back(std::move(pChild));
}

void Node::ShowTreeWindow(int& selectIndex) {
	const auto flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
		((m_nodeIndex == selectIndex) ? ImGuiTreeNodeFlags_Selected : 0) |
		(m_pChildren.empty() ? ImGuiTreeNodeFlags_Leaf : 0);
	
	const bool imguiNode = ImGui::TreeNodeEx(std::to_string(m_nodeIndex).c_str(), flags, m_nodeName.c_str());
	selectIndex = ImGui::IsItemClicked() ? m_nodeIndex : selectIndex;

	if (imguiNode) {
		for (const std::unique_ptr<Node>& pChild : m_pChildren) {
			pChild->ShowTreeWindow(selectIndex);
		}
		ImGui::TreePop();
	}
}

// ========================= Model =========================

Model::Model(const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
	const Camera& camera, bool isStatic, const std::string& path) : 
	m_modelName(path), m_selectIndex(-1) {

	Assimp::Importer imp;
	const aiScene* pModel = imp.ReadFile(path,
		aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenNormals | aiProcess_FlipWindingOrder);

	assert("Model file not found" && pModel);

	size_t numMeshes = pModel->mNumMeshes;
	Mesh::s_vertices.resize(numMeshes);
	Mesh::s_indices.resize(numMeshes);
	StringToWString(path.c_str(), Mesh::s_path);
	while (Mesh::s_path.back() != '\\' && Mesh::s_path.back() != '/')	// cut the last part of path
		Mesh::s_path.pop_back();

	for (size_t i = 0; i < numMeshes; i++) {
		m_pAllMeshes.push_back(std::make_unique<Mesh>(
			graphics, translation, rotation, scale, camera, isStatic, i, pModel->mMeshes[i], pModel->mMaterials
		));
	}

	int index = 0;
	m_pRoot = ParseNode(index, pModel->mRootNode);

	m_translations.resize(m_totalNodes);
	m_rotations.resize(m_totalNodes);

	m_translations[0] = translation;
	m_rotations[0] = rotation;
}

void Model::Render(const Graphics& graphics) const {
	m_pRoot->UpdateTree(graphics, DirectX::XMMatrixIdentity(), m_translations, m_rotations);
}

std::unique_ptr<Node> Model::ParseNode(int& nodeIndex, const aiNode* node) {
	++m_totalNodes;

	std::vector<std::shared_ptr<Mesh>> pMeshes;
	pMeshes.reserve(node->mNumMeshes);
	for (size_t i = 0; i < node->mNumMeshes; i++) {
		const unsigned int meshIndex = node->mMeshes[i];
		pMeshes.push_back(m_pAllMeshes[meshIndex]);
	}

	const DirectX::XMMATRIX localTransformation = DirectX::XMMatrixTranspose(
		DirectX::XMLoadFloat4x4(reinterpret_cast<const DirectX::XMFLOAT4X4*>(&node->mTransformation)));
	std::unique_ptr<Node> pNode = std::make_unique<Node>(m_totalNodes-1, node->mName.C_Str(), pMeshes, localTransformation);
	for (size_t i = 0; i < node->mNumChildren; i++) {
		pNode->AppendChild(ParseNode(nodeIndex, node->mChildren[i]));
	}

	return pNode;
}

void Model::ShowControlWindow() {
	//ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
	if (ImGui::Begin(m_modelName.c_str(), 0)) {
		ImGui::Columns(2, nullptr, true);
		int nodeIndex = 0;
		m_pRoot->ShowTreeWindow(m_selectIndex);
		
		ImGui::NextColumn();
		if (m_selectIndex > -1) {
			ImGui::Text("Orientation");
			ImGui::SliderAngle("Roll", &m_rotations[m_selectIndex].x, -180.0f, 180.0f);
			ImGui::SliderAngle("Pitch", &m_rotations[m_selectIndex].y, -180.0f, 180.0f);
			ImGui::SliderAngle("Yaw", &m_rotations[m_selectIndex].z, -180.0f, 180.0f);
			ImGui::Text("Position");
			ImGui::SliderFloat("X", &m_translations[m_selectIndex].x, -20.0f, 20.0f);
			ImGui::SliderFloat("Y", &m_translations[m_selectIndex].y, -20.0f, 20.0f);
			ImGui::SliderFloat("Z", &m_translations[m_selectIndex].z, -20.0f, 20.0f);
		}
	}
	ImGui::End();
}