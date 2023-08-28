#include "Character.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "OtterEngine/Common/Utils.h"

std::vector<std::vector<VertexTexture>> Character::s_vertices;
std::vector<std::vector<unsigned short>> Character::s_indices;
std::vector<bool> Character::s_hasSpecularMap;
std::vector<std::wstring> Character::s_diffuseLocation;
std::vector<std::wstring> Character::s_specularLocation;
std::vector<float> Character::s_shiness;
std::wstring Character::s_path;

Character::Character(const Game& game, const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale, 
	bool isStatic, const std::string& path) : Model(game, graphics, translation, rotation, scale, isStatic, path) {

	static unsigned int numCharacter = 0;
	m_modelName += std::to_string(numCharacter++);

	Assimp::Importer imp;
	const aiScene* pModel = imp.ReadFile(path,
		aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenNormals | aiProcess_FlipWindingOrder);

	assert("Model file not found" && pModel);

	SetupMeshInformation(pModel, path, s_vertices, s_indices, s_hasSpecularMap, s_diffuseLocation, s_specularLocation, s_shiness, s_path);

	for (size_t i = 0; i < pModel->mNumMeshes; i++) {
		std::wstring meshName;
		StringToWString(pModel->mMeshes[i]->mName.C_Str(), meshName);

		m_pAllMeshes.push_back(std::make_unique<Mesh>(
			game, graphics, translation, rotation, scale, isStatic, i, s_path + meshName, m_pMeshInformation
		));
	}

	int index = 0;
	m_pRoot = ParseNode(index, pModel->mRootNode);

	m_translations.resize(m_totalNodes);
	m_rotations.resize(m_totalNodes);

	m_translations[0] = translation;
	m_rotations[0] = rotation;
}