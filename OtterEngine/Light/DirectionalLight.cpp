#include "DirectionalLight.h"

#include <string>

#include "OtterEngine/Imgui/imgui.h"

DirectionalLight::DirectionalLight(const Graphics& graphics) : m_lightBuffer({}),
	m_directionalBufferVertex(graphics, m_lightBuffer, VertexConstantBufferType::LightDirectional, L"DirectionalLight"),
	m_directionalBufferPixel(graphics, m_lightBuffer, PixelConstantBufferType::LightDirectional, L"DirectionalLight") {
}

void DirectionalLight::AddLight(const Graphics& graphics, const DirectX::XMFLOAT4& direction, const Color4& color) {
	assert("Directional light should have w=0" && direction.w == 0);
	std::string warn = "Max" + std::to_string(kMaxLight) + " Light";
	assert(warn.c_str() && m_lightBuffer.total < kMaxLight);

	m_lightBuffer.directions[m_lightBuffer.total] = direction;
	m_lightBuffer.colors[m_lightBuffer.total] = color;

	++m_lightBuffer.total;
}

void DirectionalLight::Update(const Graphics& graphics) {
	m_directionalBufferVertex.Update(graphics, m_lightBuffer);
	m_directionalBufferPixel.Update(graphics, m_lightBuffer);

	m_directionalBufferVertex.Bind(graphics);
	m_directionalBufferPixel.Bind(graphics);
}

void DirectionalLight::ShowControlWindow() {
	// ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
	if (m_lightBuffer.total > 0) {
		if (ImGui::Begin("Directional light control", 0)) {

			for (int i = 0; i < m_lightBuffer.total; i++) {
				std::string tag = "light" + std::to_string(i) + " color";
				ImGui::ColorEdit3(tag.c_str(), &m_lightBuffer.colors[i].r);
			}

			ImGui::SliderFloat("intensity", &m_lightBuffer.intensity, 0.0f, 1.0f, "%.1f");
		}
		ImGui::End();
	}
}
