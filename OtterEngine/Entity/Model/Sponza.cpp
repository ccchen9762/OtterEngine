#include "Sponza.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "OtterEngine/Common/Utils.h"

MeshInformation Sponza::s_meshInformation;

Sponza::Sponza(const Game* game, const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
	bool isStatic, const std::string& path) : Model(game, graphics, translation, rotation, scale, isStatic, path) {

	static unsigned int numCharacter = 0;
	m_modelName += std::to_string(numCharacter++);

	Assimp::Importer imp;
	const aiScene* pModel = imp.ReadFile(path,
		aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenNormals | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);

	assert("Model file not found" && pModel);

	if (s_meshInformation.indices.empty()) {
		SetupMeshInformation(pModel, path, s_meshInformation);
	}

	for (size_t i = 0; i < pModel->mNumMeshes; i++) {
		std::wstring meshName;
		StringToWString(pModel->mMeshes[i]->mName.C_Str(), meshName);

		m_pAllMeshes.push_back(std::make_unique<Mesh>(
			game, graphics, translation, rotation, scale, isStatic, i, s_meshInformation.directory + meshName, s_meshInformation
		));
	}

	int index = 0;
	m_pRoot = ParseNode(index, pModel->mRootNode);

	m_translations.resize(m_totalNodes);
	m_rotations.resize(m_totalNodes);

	m_translations[0] = translation;
	m_rotations[0] = rotation;
}