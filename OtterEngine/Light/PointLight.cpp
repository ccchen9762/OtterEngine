#include "PointLight.h"

#include "OtterEngine/Imgui/imgui.h"

PointLight::PointLight(const Graphics& graphics, const DirectX::XMFLOAT4& position, const Color4& color, float scale,
	const DirectX::XMMATRIX& viewProjectionMatrix) :
	m_lightBuffer({ position, color }),
	m_scale(scale),
	m_constantBufferVertex(graphics, m_lightBuffer, VertexConstantBufferType::Light),
	m_constantBufferPixel(graphics, m_lightBuffer, PixelConstantBufferType::Light),
	m_sphereMesh(DebugSphere{
		graphics,
		Vector3(position.x, position.y, position.z),
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(0.2f, 0.2f, 0.2f),
		viewProjectionMatrix,
		true
	}) {
}

void PointLight::Update(const Graphics& graphics) const {
	m_constantBufferVertex.Update(graphics, m_lightBuffer);
	m_constantBufferPixel.Update(graphics, m_lightBuffer);
}

void PointLight::Render(const Graphics& graphics) {
	m_sphereMesh.Translate({ m_lightBuffer.m_position.x, m_lightBuffer.m_position.y, m_lightBuffer.m_position.z });
	m_sphereMesh.Render(graphics);
}

void PointLight::ShowControlWindow() {
	if(ImGui::Begin("Light control")){
		ImGui::SliderFloat("x", &m_lightBuffer.m_position.x, -10.0f, 10.0f, "%.1f");
		ImGui::SliderFloat("y", &m_lightBuffer.m_position.y, -10.0f, 10.0f, "%.1f");
		ImGui::SliderFloat("z", &m_lightBuffer.m_position.z, -10.0f, 10.0f, "%.1f");

		ImGui::ColorEdit3("light color", & m_lightBuffer.m_color.r);

		/*ImGui::SliderFloat("r", &m_lightBuffer.m_color.r, 0.0f, 1.0f, "%.1f");
		ImGui::SliderFloat("g", &m_lightBuffer.m_color.g, 0.0f, 1.0f, "%.1f");
		ImGui::SliderFloat("b", &m_lightBuffer.m_color.b, 0.0f, 1.0f, "%.1f");*/
	}
	ImGui::End();
}
