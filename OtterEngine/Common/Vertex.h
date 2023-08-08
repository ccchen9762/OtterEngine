#pragma once

struct Color4 {
	float r, g, b, a;
};

struct Vertex {
	DirectX::XMVECTOR m_position;
	Color4 m_color;
};

struct VertexTexture {
	DirectX::XMVECTOR m_position;
	struct {
		float u, v;
	} m_texcoord;
};

