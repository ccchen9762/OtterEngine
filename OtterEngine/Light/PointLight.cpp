#include "PointLight.h"

#include "OtterEngine/Imgui/imgui.h"

PointLight::PointLight(const Graphics& graphics, const DirectX::XMFLOAT4& position, const Color4& color, float scale,
	const Camera& camera) :
	m_lightBuffer({ position, color, {0.15f, 0.15f, 0.15f, 1.0f}, 1.0f, 1.0f, 0.45f, 0.0075f }),
	m_scale(scale),
	m_constantBufferVertex(graphics, m_lightBuffer, VertexConstantBufferType::Light),
	m_constantBufferPixel(graphics, m_lightBuffer, PixelConstantBufferType::Light),
	m_sphereMesh(DebugSphere{
		graphics,
		Vector3(position.x, position.y, position.z),
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(0.2f, 0.2f, 0.2f),
		camera,
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

		ImGui::SliderFloat("intensity", &m_lightBuffer.diffuseIntensity, 0.0f, 1.0f, "%.1f");
		ImGui::SliderFloat("attenuation^0", &m_lightBuffer.attenuationConst, 0.0f, 1.0f, "%.1f");
		ImGui::SliderFloat("attenuation^1", &m_lightBuffer.attenuationLinear, 0.0f, 1.0f, "%.3f");
		ImGui::SliderFloat("attenuation^2", &m_lightBuffer.attenuationQuadratic, 0.0f, 1.0f, "%.4f");
	}
	ImGui::End();
}
