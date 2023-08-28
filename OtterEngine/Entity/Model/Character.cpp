#include "Character.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "OtterEngine/Common/Utils.h"

std::vector<std::vector<VertexTexture>> Character::s_vertices;
std::vector<std::vector<unsigned short>> Character::s_indices;
std::wstring Character::s_path;

Character::Character(const Game& game, const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale, 
	bool isStatic, const std::string& path) : Model(game, graphics, translation, rotation, scale, isStatic, path){

	/*static unsigned int numCharacter = 0;
	m_modelName += std::to_string(numCharacter++);

	Assimp::Importer imp;
	const aiScene* pModel = imp.ReadFile(path,
		aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenNormals | aiProcess_FlipWindingOrder);

	assert("Model file not found" && pModel);

	size_t numMeshes = pModel->mNumMeshes;
	Character::s_vertices.resize(numMeshes);
	Character::s_indices.resize(numMeshes);
	StringToWString(path.c_str(), Character::s_path);
	while (Character::s_path.back() != '\\' && Character::s_path.back() != '/')	// cut the last part of path
		Character::s_path.pop_back();
	
	for (size_t i = 0; i < numMeshes; i++) {
		m_pAllMeshes.push_back(std::make_unique<Mesh>(
			game, graphics, translation, rotation, scale, isStatic, i, pModel->mMeshes[i], pModel->mMaterials
		));
	}

	int index = 0;
	m_pRoot = ParseNode(index, pModel->mRootNode);

	m_translations.resize(m_totalNodes);
	m_rotations.resize(m_totalNodes);

	m_translations[0] = translation;
	m_rotations[0] = rotation;*/
}
