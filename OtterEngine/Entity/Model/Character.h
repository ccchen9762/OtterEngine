#pragma once

#include "Model.h"

class Character : public Model
{
public:
	Character(const Game& game, const Graphics& graphics, const Vector3& translation, const Vector3& rotation, const Vector3& scale,
		bool isStatic, const std::string& path);
	~Character() = default;

private:
	static std::vector<std::vector<VertexTexture>> s_vertices;
	static std::vector<std::vector<unsigned short>> s_indices;
	static std::vector<bool> s_hasSpecularMap;
	static std::vector<std::wstring> s_diffuseLocation;
	static std::vector<std::wstring> s_specularLocation;
	static std::vector<float> s_shiness;
	static std::wstring s_path;
};