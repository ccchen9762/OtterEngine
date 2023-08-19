#include "PointLight.h"

#include "OtterEngine/Imgui/imgui.h"

PointLight::PointLight(const Graphics& graphics, const DirectX::XMFLOAT4& position, const Color4& color, float scale) :
	m_lightBuffer({ position, color }),
	m_scale(scale),
	constantBufferPixel(graphics, m_lightBuffer){

}

void PointLight::Bind(const Graphics& graphics) const {
	constantBufferPixel.Update(graphics, m_lightBuffer);
}

void PointLight::Render(const Graphics& graphics) const {
}

void PointLight::ShowControlWindow() {
	if(ImGui::Begin("Light control")){
		ImGui::SliderFloat("x", &m_lightBuffer.m_position.x, -30.0f, 30.0f, "%.1f");
		ImGui::SliderFloat("y", &m_lightBuffer.m_position.y, -30.0f, 30.0f, "%.1f");
		ImGui::SliderFloat("z", &m_lightBuffer.m_position.z, -30.0f, 30.0f, "%.1f");

		ImGui::SliderFloat("r", &m_lightBuffer.m_color.r, 0.0f, 1.0f, "%.1f");
		ImGui::SliderFloat("g", &m_lightBuffer.m_color.g, 0.0f, 1.0f, "%.1f");
		ImGui::SliderFloat("b", &m_lightBuffer.m_color.b, 0.0f, 1.0f, "%.1f");
	}
	ImGui::End();
}
