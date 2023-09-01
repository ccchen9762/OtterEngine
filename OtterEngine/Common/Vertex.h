#pragma once

struct Color4 { float r, g, b, a; };
struct Normal { float x, y, z; };

struct Vertex {
	DirectX::XMVECTOR m_position;
	Color4 m_color;
	Normal m_normal;
};

struct VertexTexture {
	DirectX::XMVECTOR m_position;
	DirectX::XMFLOAT2 m_texcoord;
	Normal m_normal;
};

struct VertexBasic {
	DirectX::XMVECTOR m_position;
};

struct VertexTextureBasic {
	DirectX::XMVECTOR m_position;
	DirectX::XMFLOAT2 m_texcoord;
};

struct VertexNormalMap {
	DirectX::XMVECTOR m_position;
	DirectX::XMFLOAT2 m_texcoord;
	Normal m_normal;
	Normal m_tangent;
	Normal m_bitangent;
};

struct Attributes {
	float shiness;
	BOOL hasSpecularMap;
	BOOL hasNormalMap;
	BOOL hasDiffuseMap;
};
