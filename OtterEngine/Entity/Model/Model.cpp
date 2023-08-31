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

Mesh::Mesh(const Game* game, const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
	bool isStatic, unsigned int meshIndex, const std::wstring& meshPath, const MeshInformation& meshInformation)
	: Entity(game, translation, rotation, scale, 0, isStatic), m_meshIndex(meshIndex), m_attributes({ 0.0, true }) {

	// remember to set indice size!!!!!!!!!!!!!!!!!!
	m_indicesSize = meshInformation.indices[meshIndex].size();

	// buffers & textures
	std::shared_ptr<GraphicsResource> pVertexBuffer = ResourcePool::GetResource<VertexBuffer>(
		graphics, meshInformation.vertices[meshIndex].data(), sizeof(VertexTexture), meshInformation.vertices[meshIndex].size(),
		VertexBuffer::Topology::Triangle, L"#Mesh#" + meshPath);
	m_graphicsResources.push_back(std::move(pVertexBuffer));

	std::shared_ptr<GraphicsResource> pIndexBuffer = ResourcePool::GetResource<IndexBuffer>(
		graphics, meshInformation.indices[meshIndex], L"#Mesh#" + meshPath);
	m_graphicsResources.push_back(std::move(pIndexBuffer));

	m_graphicsResources.push_back(std::make_shared<ConstantBufferTransformation>(graphics, *this));

	std::shared_ptr<GraphicsResource> pDiffuse = ResourcePool::GetResource<Texture>(
		graphics, meshInformation.diffuseFile[meshIndex], Texture::Type::Diffuse);
	m_graphicsResources.push_back(std::move(pDiffuse));

	if (meshInformation.hasSpecularMap[meshIndex]) {
		std::shared_ptr<GraphicsResource> pSpecular = ResourcePool::GetResource<Texture>(
			graphics, meshInformation.specularFile[meshIndex], Texture::Type::Specular);
		m_graphicsResources.push_back(std::move(pSpecular));
		AddTextureShadingResource(graphics, true);
	}
	else {
		m_attributes.hasSpecularMap = false;

		std::shared_ptr<GraphicsResource> pSpecular = ResourcePool::GetResource<Texture>(
			graphics, L"", Texture::Type::Specular);
		m_graphicsResources.push_back(std::move(pSpecular));
		AddTextureShadingResource(graphics, false);
	}

	m_attributes.shiness = meshInformation.shiness[meshIndex];

	m_graphicsResources.push_back(std::make_shared<ConstantBufferVertex<Attributes>>(
		graphics, m_attributes, VertexConstantBufferType::Attributes, GetUID()));
	m_graphicsResources.push_back(std::make_shared<ConstantBufferPixel<Attributes>>(
		graphics, m_attributes, PixelConstantBufferType::Attributes, GetUID()));
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
		//mesh->Update();
		
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

Model::Model(const Game* game, const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
	bool isStatic, const std::string& path) : 
	m_modelName(path), m_selectIndex(-1), m_totalNodes(0) {
}

void Model::Render(const Graphics& graphics) const {
	m_pRoot->UpdateTree(graphics, DirectX::XMMatrixIdentity(), m_translations, m_rotations);
}

void Model::SetupMeshInformation(const aiScene* pModel,
	const std::string& path,
	MeshInformation& s_meshInformation) {

	size_t numMeshes = pModel->mNumMeshes;
	s_meshInformation.vertices.resize(numMeshes);
	s_meshInformation.indices.resize(numMeshes);
	s_meshInformation.hasSpecularMap.resize(numMeshes);
	s_meshInformation.diffuseFile.resize(numMeshes);
	s_meshInformation.specularFile.resize(numMeshes);
	s_meshInformation.shiness.resize(numMeshes);

	StringToWString(path.c_str(), s_meshInformation.directory);
	while (s_meshInformation.directory.back() != '\\' && s_meshInformation.directory.back() != '/')	// cut the last part of path
		s_meshInformation.directory.pop_back();

	for (size_t i = 0; i < numMeshes; i++) {
		LoadMesh(i, pModel->mMeshes[i], pModel->mMaterials, s_meshInformation);
	}
}

void Model::LoadMesh(unsigned int meshIndex, const aiMesh* pMesh, const aiMaterial* const* ppMaterials,
	MeshInformation& s_meshInformation) {
	
	s_meshInformation.vertices[meshIndex].reserve(pMesh->mNumVertices);
	for (unsigned int i = 0; i < pMesh->mNumVertices; i++) {
		s_meshInformation.vertices[meshIndex].emplace_back(
			DirectX::XMVECTOR{ pMesh->mVertices[i].x, pMesh->mVertices[i].y, pMesh->mVertices[i].z, 1.0f },
			DirectX::XMFLOAT2{ pMesh->mTextureCoords[0][i].x, pMesh->mTextureCoords[0][i].y },
			Normal{ pMesh->mNormals[i].x, pMesh->mNormals[i].y, pMesh->mNormals[i].z }
		);
	}

	s_meshInformation.indices[meshIndex].reserve(pMesh->mNumFaces * 3);
	for (unsigned int i = 0; i < pMesh->mNumFaces; i++) {
		const aiFace& face = pMesh->mFaces[i];
		assert("Model face are not all triangles" && face.mNumIndices == 3);

		s_meshInformation.indices[meshIndex].push_back(face.mIndices[0]);
		s_meshInformation.indices[meshIndex].push_back(face.mIndices[2]);
		s_meshInformation.indices[meshIndex].push_back(face.mIndices[1]);
	}

	const aiMaterial* pMaterial = ppMaterials[pMesh->mMaterialIndex];
	aiString textureName;
	// load diffuse map
	if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &textureName) == aiReturn_SUCCESS) {
		std::wstring materialName;
		StringToWString(textureName.C_Str(), materialName);
		std::wstring fileLocation = s_meshInformation.directory + materialName;

		s_meshInformation.diffuseFile[meshIndex] = fileLocation;
	}

	// load specular map
	if (pMaterial->GetTexture(aiTextureType_SPECULAR, 0, &textureName) == aiReturn_SUCCESS) {
		std::wstring materialName;
		StringToWString(textureName.C_Str(), materialName);
		std::wstring fileLocation = s_meshInformation.directory + materialName;

		s_meshInformation.hasSpecularMap[meshIndex] = true;
		s_meshInformation.specularFile[meshIndex] = fileLocation;
	}
	else {
		s_meshInformation.hasSpecularMap[meshIndex] = false;

		pMaterial->Get(AI_MATKEY_SHININESS, s_meshInformation.shiness[meshIndex]);
	}
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
	std::unique_ptr<Node> pNode = std::make_unique<Node>(m_totalNodes - 1, node->mName.C_Str(), pMeshes, localTransformation);
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