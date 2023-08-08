#pragma once

struct Vertex {
	DirectX::XMVECTOR m_position;
	struct {
		float r, g, b, a;
	} m_color;
};

struct VertexTexture {
	DirectX::XMVECTOR m_position;
	struct {
		float u, v;
	} m_texcoord;
};