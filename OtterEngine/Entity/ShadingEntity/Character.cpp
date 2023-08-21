#include "Character.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "OtterEngine/Graphics/Resource/VertexBuffer.h"
#include "OtterEngine/Graphics/Resource/IndexBuffer.h"

std::vector<Vertex> Character::s_vertices;
std::vector<unsigned short> Character::s_indices;

std::vector<std::unique_ptr<GraphicsResource>> Character::s_commonResources;

std::string Character::s_modelPath = "Assets/Model/nanosuit.obj";

Character::Character(const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
	const Camera& camera, const std::string& path, bool isStatic)
	: ShadingEntity(graphics, translation, rotation, scale, s_indices.size(), camera, isStatic){

	if (s_commonResources.empty()) {
		LoadMesh();
		
		// buffers
		s_commonResources.push_back(std::make_unique<VertexBuffer>(graphics,
			s_vertices.data(), static_cast<unsigned int>(sizeof(Vertex)), s_vertices.size()));
		s_commonResources.push_back(std::make_unique<IndexBuffer>(graphics, s_indices));
	}
	m_indicesSize = s_indices.size(); // make sure size in Entity changes
}

void Character::LoadMesh() {
	Assimp::Importer imp;
	const aiScene* pModel = imp.ReadFile(s_modelPath, 
		aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

	unsigned int offset = 0;
	for (int i = 0; i < pModel->mNumMeshes; i++) {
		const aiMesh* pMesh = pModel->mMeshes[i];

		s_vertices.reserve(s_vertices.size() + pMesh->mNumVertices);
		for (unsigned int j = 0; j < pMesh->mNumVertices; j++) {
			s_vertices.emplace_back(
				DirectX::XMVECTOR{ pMesh->mVertices[j].x, pMesh->mVertices[j].y, pMesh->mVertices[j].z, 1.0f },
				Color4{ 1.0f, 1.0f, 1.0f, 1.0f },
				Normal{ pMesh->mNormals[j].x, pMesh->mNormals[j].y, pMesh->mNormals[j].z }
			);
		}

		s_indices.reserve(s_vertices.size() + pMesh->mNumFaces * 3);
		for (unsigned int j = 0; j < pMesh->mNumFaces; j++) {
			const aiFace& face = pMesh->mFaces[j];
			assert("Model face are not all triangles" && face.mNumIndices == 3);
			s_indices.push_back(face.mIndices[0] + offset);
			s_indices.push_back(face.mIndices[2] + offset);
			s_indices.push_back(face.mIndices[1] + offset);
		}
		offset += pMesh->mNumVertices;
	}
}
