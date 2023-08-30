#include "PointLight.h"

#include <string>

#include "OtterEngine/Imgui/imgui.h"

int PointLight::s_numLight = 0;

PointLight::PointLight(const Game& game, const Graphics& graphics, const DirectX::XMFLOAT4& position, const Color4& color) :
	m_lightBuffer({ position, color, {0.15f, 0.15f, 0.15f, 1.0f}, 1.0f, 1.0f, 0.007f, 0.0002f }),
	m_constantBufferVertex(graphics, m_lightBuffer, VertexConstantBufferType::Light, L"PointLight" + std::to_wstring(s_numLight)),
	m_constantBufferPixel(graphics, m_lightBuffer, PixelConstantBufferType::Light, L"PointLight" + std::to_wstring(s_numLight)),
	m_sphereMesh(DebugSphere{
		game,
		graphics,
		Vector3(position.x, position.y, position.z),
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(0.2f, 0.2f, 0.2f),
		true
	}) {
}

void PointLight::Update(const Graphics& graphics) {
	m_constantBufferVertex.Update(graphics, m_lightBuffer);
	m_constantBufferPixel.Update(graphics, m_lightBuffer);

	m_constantBufferVertex.Bind(graphics);
	m_constantBufferPixel.Bind(graphics);
	
	m_sphereMesh.Translate({ m_lightBuffer.position.x, m_lightBuffer.position.y, m_lightBuffer.position.z });
	m_sphereMesh.Update();
}

void PointLight::Render(const Graphics& graphics) const {
	m_sphereMesh.Render(graphics);
}

void PointLight::ShowControlWindow() {
	if(ImGui::Begin("Light control", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)){
		ImGui::SliderFloat("x", &m_lightBuffer.position.x, -20.0f, 20.0f, "%.1f");
		ImGui::SliderFloat("y", &m_lightBuffer.position.y, -20.0f, 20.0f, "%.1f");
		ImGui::SliderFloat("z", &m_lightBuffer.position.z, -20.0f, 20.0f, "%.1f");

		ImGui::ColorEdit3("light color", &m_lightBuffer.lightColor.r);

		ImGui::SliderFloat("intensity", &m_lightBuffer.intensity, 0.0f, 1.0f, "%.1f");
		ImGui::SliderFloat("attenuation^0", &m_lightBuffer.attenuationConst, 0.0f, 1.0f, "%.2f");
		ImGui::SliderFloat("attenuation^1", &m_lightBuffer.attenuationLinear, 0.0f, 0.1f, "%.3f");
		ImGui::SliderFloat("attenuation^2", &m_lightBuffer.attenuationQuadratic, 0.0f, 0.01f, "%.4f");
	}
	ImGui::End();
}
