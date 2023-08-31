#include "PointLight.h"

#include <string>

#include "OtterEngine/Imgui/imgui.h"

PointLight::PointLight(const Graphics& graphics) : m_lightBuffer({}),
	m_pointBufferVertex(graphics, m_lightBuffer, VertexConstantBufferType::LightPoint, L"PointLight"),
	m_pointBufferPixel(graphics, m_lightBuffer, PixelConstantBufferType::LightPoint, L"PointLight") {
	m_sphereMeshs.clear();
}

void PointLight::AddLight(const Game* game, const Graphics& graphics, const DirectX::XMFLOAT4& position, const Color4& color) {
	assert("Point light should have w=1" && position.w == 1);
	assert("Max 10 Light" && m_lightBuffer.total < 10);

	m_lightBuffer.positions[m_lightBuffer.total] = position;
	m_lightBuffer.colors[m_lightBuffer.total] = color;

	++m_lightBuffer.total;

	// OMG
	m_sphereMeshs.emplace_back(
		game,
		graphics,
		Vector3(position.x, position.y, position.z),
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(0.2f, 0.2f, 0.2f),
		true);
}

void PointLight::Update(const Graphics& graphics) {
	m_pointBufferVertex.Update(graphics, m_lightBuffer);
	m_pointBufferPixel.Update(graphics, m_lightBuffer);

	m_pointBufferVertex.Bind(graphics);
	m_pointBufferPixel.Bind(graphics);
	
	for (int i = 0; i < m_sphereMeshs.size(); i++) {
		m_sphereMeshs[i].Translate({m_lightBuffer.positions[i].x, m_lightBuffer.positions[i].y, m_lightBuffer.positions[i].z});
		m_sphereMeshs[i].Update();
	}
}

void PointLight::Render(const Graphics& graphics) const {
	for (int i = 0; i < m_sphereMeshs.size(); i++) {
		m_sphereMeshs[i].Render(graphics);
	}
}

void PointLight::ShowControlWindow() {
	// ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
	if (m_lightBuffer.total > 0) {
		if (ImGui::Begin("Point Light control", 0)) {
			for (int i = 0; i < m_lightBuffer.total; i++) {
				std::string tag = "light" + std::to_string(i);
				ImGui::Text(tag.c_str());
				ImGui::SliderFloat("x", &m_lightBuffer.positions[i].x, -20.0f, 20.0f, "%.1f");
				ImGui::SliderFloat("y", &m_lightBuffer.positions[i].y, -20.0f, 20.0f, "%.1f");
				ImGui::SliderFloat("z", &m_lightBuffer.positions[i].z, -20.0f, 20.0f, "%.1f");

				ImGui::ColorEdit3(" color", &m_lightBuffer.colors[i].r);
			}

			ImGui::SliderFloat("intensity", &m_lightBuffer.intensity, 0.0f, 1.0f, "%.1f");
			ImGui::SliderFloat("attenuation^0", &m_lightBuffer.attenuationConst, 0.0f, 1.0f, "%.1f");
			ImGui::SliderFloat("attenuation^1", &m_lightBuffer.attenuationLinear, 0.0f, 1.0f, "%.3f");
			ImGui::SliderFloat("attenuation^2", &m_lightBuffer.attenuationQuadratic, 0.0f, 1.0f, "%.4f");
		}
		ImGui::End();
	}
}
