#include "DirectionalLight.h"

#include <string>

#include "OtterEngine/Imgui/imgui.h"

int DirectionalLight::s_numLight = 0;

DirectionalLight::DirectionalLight(const Game& game, const Graphics& graphics, const DirectX::XMFLOAT4& direction, const Color4& color) :
	m_lightBuffer({ direction, color, {0.15f, 0.15f, 0.15f, 1.0f}, 1.0f, 1.0f, 0.30f, 0.0015f }),
	m_constantBufferVertex(graphics, m_lightBuffer, VertexConstantBufferType::Light, L"DirectionalLight" + std::to_wstring(s_numLight)),
	m_constantBufferPixel(graphics, m_lightBuffer, PixelConstantBufferType::Light, L"DirectionalLight" + std::to_wstring(s_numLight)) {
}

void DirectionalLight::Update(const Graphics& graphics) {
	m_constantBufferVertex.Update(graphics, m_lightBuffer);
	m_constantBufferPixel.Update(graphics, m_lightBuffer);

	m_constantBufferVertex.Bind(graphics);
	m_constantBufferPixel.Bind(graphics);
}

void DirectionalLight::ShowControlWindow() {
	if (ImGui::Begin("Light control", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {

		ImGui::ColorEdit3("light color", &m_lightBuffer.lightColor.r);

		ImGui::SliderFloat("intensity", &m_lightBuffer.intensity, 0.0f, 1.0f, "%.1f");
		ImGui::SliderFloat("attenuation^0", &m_lightBuffer.attenuationConst, 0.0f, 1.0f, "%.1f");
		ImGui::SliderFloat("attenuation^1", &m_lightBuffer.attenuationLinear, 0.0f, 1.0f, "%.3f");
		ImGui::SliderFloat("attenuation^2", &m_lightBuffer.attenuationQuadratic, 0.0f, 1.0f, "%.4f");
	}
	ImGui::End();
}
