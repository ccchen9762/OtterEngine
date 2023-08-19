#pragma once

struct Normal {
	float x, y, z;
};

struct Color4 {
	float r, g, b, a;
};

struct Vertex {
	DirectX::XMVECTOR m_position;
	Color4 m_color;
	Normal m_normal;
};

struct VertexTexture {
	DirectX::XMVECTOR m_position;
	struct {
		float u, v;
	} m_texcoord;
	Normal m_normal;
};

struct VertexBasic {
	DirectX::XMVECTOR m_position;
	Color4 m_color;
};

struct VertexTextureBasic {
	DirectX::XMVECTOR m_position;
	struct {
		float u, v;
	} m_texcoord;
};
